#include "fcgi_config.h"

#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/wait.h>

#include "fcgi_stdio.h"
#include "util_cgi.h"
#include "redis_keys.h"
#include "redis_op.h"

#define UPLOAD_LOG_MODULE "cgi"
#define UPLOAD_LOG_PROC   "upload"

int main ()
{
    char *file_buf = NULL;
    char boundary[256] = {0};
    char content_text[256] = {0};
    char filename[256] = {0};
    char fdfs_file_path[256] = {0};
    char fdfs_file_stat_buf[256] = {0};
    char fdfs_file_host_name[30] = {0};
    char fdfs_file_url[512] = {0};
    char *redis_value_buf = NULL;
    time_t now;;
    char create_time[25];
    char suffix[10];
    char user[128] = {0};
    


    while (FCGI_Accept() >= 0) {
        char *contentLength = getenv("CONTENT_LENGTH");
        int len;

        char *query_string = getenv("QUERY_STRING");

        //得到用户名
        query_parse_key_value(query_string, "user", user, NULL);
        if (strlen(user) == 0) {
            LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "get user has no value!!", user);
            break;                
        }
        else {
            LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "get user = [%s]", user);
        }



        printf("Content-type: text/html\r\n"
                "\r\n");

        if (contentLength != NULL) {
            len = strtol(contentLength, NULL, 10);
        }
        else {
            len = 0;
        }

        if (len <= 0) {
            printf("No data from standard input\n");
        }
        else {
            int i, ch;
            char *begin = NULL;
            char *end = NULL;
            char *p, *q, *k;

            //==========> 开辟存放文件的 内存 <===========

            file_buf = malloc(len);
            if (file_buf == NULL) {
                printf("malloc error! file size is to big!!!!\n");
                return -1;
            }

            begin = file_buf;
            p = begin;
            for (i = 0; i < len; i++) {
                if ((ch = getchar()) < 0) {
                    printf("Error: Not enough bytes received on standard input<p>\n");
                    break;
                }
                //putchar(ch);
                *p = ch;
                p++;
            }

            //===========> 开始处理前端发送过来的post数据格式 <============
            //begin deal
            end = p;

            p = begin;

            //get boundary
            p = strstr(begin, "\r\n");
            if (p == NULL) {
                printf("wrong no boundary!\n");
                goto END;
            }

            strncpy(boundary, begin, p-begin);
            boundary[p-begin] = '\0';
            //printf("boundary: [%s]\n", boundary);

            p+=2;//\r\n
            //已经处理了p-begin的长度
            len -= (p-begin);

            //get content text head
            begin = p;

            p = strstr(begin, "\r\n");
            if(p == NULL) {
                printf("ERROR: get context text error, no filename?\n");
                goto END;
            }
            strncpy(content_text, begin, p-begin);
            content_text[p-begin] = '\0';
            //printf("content_text: [%s]\n", content_text);

            p+=2;//\r\n
            len -= (p-begin);

            //get filename
            // filename="123123.png"
            //           ↑
            q = begin;
            q = strstr(begin, "filename=");
            
            q+=strlen("filename=");
            q++;

            k = strchr(q, '"');
            strncpy(filename, q, k-q);
            filename[k-q] = '\0';

            trim_space(filename);
            //printf("filename: [%s]\n", filename);

            //get file
            begin = p;     
            p = strstr(begin, "\r\n");
            p+=4;//\r\n\r\n
            len -= (p-begin);

            begin = p;
            // now begin -->file's begin
            //find file's end
            p = memstr(begin, len, boundary);
            if (p == NULL) {
                p = end-2;    //\r\n
            }
            else {
                p = p -2;//\r\n
            }
        
            //begin---> file_len = (p-begin)

            //=====> 此时begin-->p两个指针的区间就是post的文件二进制数据
            //======>将数据写入文件中,其中文件名也是从post数据解析得来  <===========

            int fd = 0;
            fd = open(filename, O_CREAT|O_WRONLY, 0644);
            if (fd < 0) {
                printf("open %s error\n", filename);
                goto END;
            }

            ftruncate(fd, (p-begin));
            write(fd, begin, (p-begin));
            close(fd);

            LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "upload [%s] succ!", filename);

            //===============> 将该文件存入fastDFS中,并得到文件的file_id <============
            pid_t pid;
            int pfd[2];

            if (pipe(pfd) < 0) {
                printf("pip error\n");
            }

            pid = fork();
            if (pid < 0) {
                printf("fork error\n");
            }

            if (pid == 0) {
                //child
                //关闭读管道
                close(pfd[0]);

                //将标准输出重定向到写管道
                dup2(pfd[1], STDOUT_FILENO);

                execlp("fdfs_upload_file", "fdfs_upload_file", "./conf/client/client.conf", filename, NULL);
                LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "execlp fdfs_upload_file error\n");
                close(pfd[1]);
            }

            //关闭写管道
            close(pfd[1]);


            //read from 读管道
            read(pfd[0], fdfs_file_path, 256);
            trim_space(fdfs_file_path);
            
            if (strlen(fdfs_file_path) == 0) {
                LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC,"[upload FAILED!]");
                goto END;
            }
            wait(NULL);
            close(pfd[0]);
            LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "get [%s] succ!\n", fdfs_file_path);


            //================ > 得到文件所存放storage的host_name <=================
            pipe(pfd);

            pid = fork();

            if (pid == 0) {
                //child
                //关闭读管道
                close(pfd[0]);

                //将标准输出重定向到写管道
                dup2(pfd[1], STDOUT_FILENO);
                execlp("fdfs_file_info", "fdfs_file_info", "./conf/client/client.conf", fdfs_file_path, NULL);
            }

            //关闭写管道
            close(pfd[1]);
            //read from 读管道
            read(pfd[0], fdfs_file_stat_buf, 256);
            LOG(UPLOAD_LOG_MODULE, UPLOAD_LOG_PROC, "get file_ip [%s] succ\n", fdfs_file_stat_buf);
            wait(NULL);
            close(pfd[0]);

            //拼接上传文件的完整url地址--->http://host_name/group1/M00/00/00/D12313123232312.png
            p = strstr(fdfs_file_stat_buf, "source ip address: ");

            //192.168.123.123
            //q              k
            q = p + strlen("source ip address: ");
            k = strstr(q, "\n");
            strncpy(fdfs_file_host_name, q, k-q);

            fdfs_file_host_name[k-q] = '\0';

            //printf("host_name:[%s]\n", fdfs_file_host_name);

            strcat(fdfs_file_url, "http://");
            strcat(fdfs_file_url, fdfs_file_host_name);
            strcat(fdfs_file_url, "/");
            strcat(fdfs_file_url, fdfs_file_path);

            printf("[%s]\n", fdfs_file_url);


            


            

            //================> 删除本地临时存放的上传文件 <===============
            unlink(filename);

            //===============> 将该文件的fastDFS路径和名称和上传者 存入redis中 <==============

            redisContext * redis_conn = NULL;

            redis_conn = rop_connectdb_nopwd("127.0.0.1", "6379");
            if (redis_conn == NULL) {
                goto END;
            }

            //make file value
            redis_value_buf = malloc(VALUES_ID_SIZE);
            if (redis_value_buf == NULL) {
                LOG(REDIS_LOG_MODULE, REDIS_LOG_PROC, "[-]malloc redis value error\n");
                goto END;
            }

            memset(redis_value_buf, 0, VALUES_ID_SIZE);

            //file_id
            strcat(redis_value_buf, fdfs_file_path);
            strcat(redis_value_buf, REDIS_DILIMT);
            //url
            strcat(redis_value_buf, fdfs_file_url);
            strcat(redis_value_buf, REDIS_DILIMT);
            //filename
            strcat(redis_value_buf, filename);
            strcat(redis_value_buf, REDIS_DILIMT);
            //create time
            now = time(NULL);
            strftime(create_time, 24, "%Y-%m-%d %H:%M:%S", localtime(&now));
            strcat(redis_value_buf, create_time);
            strcat(redis_value_buf, REDIS_DILIMT);
            strcat(redis_value_buf, user);
            strcat(redis_value_buf, REDIS_DILIMT);
            //type
            get_file_suffix(filename, suffix);
            strcat(redis_value_buf, suffix);

            //将文件插入到FILE_INFO_LIST表中
            rop_list_push(redis_conn, FILE_INFO_LIST, redis_value_buf);

            //将文件插入到FILE_HOT_ZSET中，默认的权值是1
            rop_zset_increment(redis_conn, FILE_HOT_ZSET, fdfs_file_path);


            free(redis_value_buf);

            
END:
            rop_disconnect(redis_conn);

            memset(boundary, 0, 256);
            memset(content_text, 0, 256);
            memset(filename, 0, 256);
            memset(fdfs_file_path, 0, 256);
            memset(fdfs_file_stat_buf, 0, 256);
            memset(fdfs_file_host_name, 0, 30);
            memset(fdfs_file_url, 0, 512);
            memset(user, 0, 128);

            free(file_buf);
            //printf("date: %s\r\n", getenv("QUERY_STRING"));
        }
    } /* while */

    return 0;
}
