#include "fdfs_api.h"

#define FDFS_TEST_MODULE "api"
#define FDFS_TEST_PROC   "fdfs"



/* -------------------------------------------*/
/**
 * @brief  上传到fdfs一个文件
 *
 * @param filename
 * @param fileid OUT
 *
 * @returns   
 *   0 succ -1 fail
 */
/* -------------------------------------------*/
int fdfs_upload_by_filename(const char* filename, char *fileid)
{
    int retn = 0;
    pid_t pid;
    int pfd[2];

    //创建管道
    retn = pipe(pfd);
    if (retn < 0) {
        perror("pipe error");
        return -1;
    }

    //创建进程
    pid = fork();
    if (pid < 0) {
        perror("fork error");
        return -1;
    }
    else if (pid == 0) {
        //子进程
        //执行指令
        //关闭 读管道
        close(pfd[0]);

        //将标准输出重定向到 管道
        dup2(pfd[1], STDOUT_FILENO);

        //执行指令
        execlp("fdfs_upload_file", "fdfs_upload_file", "./conf/client.conf", filename, NULL);
        LOG(FDFS_TEST_MODULE, FDFS_TEST_PROC, "execl upload cmd error");
        return -1;
    }
    else if (pid > 0) {
        //父进程 读管道
        //关闭写管道
        close(pfd[1]);

        //wait
        wait(NULL);

        //从管道中去读数据
        read(pfd[0], fileid, FILEID_LEN);
        
        //将fileid 记入日志 
//        LOG(FDFS_TEST_MODULE, FDFS_TEST_PROC, "fileid=[%s]", fileid);
        
        close(pfd[0]);
    }

	return 0;
}



/* -------------------------------------------*/
/**
 * @brief  fdfs_upload_by_filename1 
 *
 * @param local_filename 要上传的文件路径
 * @param fileid OUT 得到的fileid
 *
 * @returns   
 *  0 succ !0 fail
 */
/* -------------------------------------------*/
int fdfs_upload_by_filename1(const char *local_filename, char *file_id)
{
	char *conf_filename;
	char group_name[FDFS_GROUP_NAME_MAX_LEN + 1];
	ConnectionInfo *pTrackerServer;
	int result;
	int store_path_index;
	ConnectionInfo storageServer;

	log_init();
	g_log_context.log_level = LOG_ERR;
	ignore_signal_pipe();

	conf_filename = "./conf/client.conf";
	if ((result=fdfs_client_init(conf_filename)) != 0)
	{
		return result;
	}

    //获取tracker的链接句柄
	pTrackerServer = tracker_get_connection();
	if (pTrackerServer == NULL)
	{
		fdfs_client_destroy();
		return errno != 0 ? errno : ECONNREFUSED;
	}

	*group_name = '\0';
    //根据tracker 句柄 得到一个可以上传的storage句柄
	if ((result=tracker_query_storage_store(pTrackerServer, \
	                &storageServer, group_name, &store_path_index)) != 0)
	{
		fdfs_client_destroy();
		fprintf(stderr, "tracker_query_storage fail, " \
			"error no: %d, error info: %s\n", \
			result, STRERROR(result));
		return result;
	}

    //将文件传给storage
	result = storage_upload_by_filename1(pTrackerServer, \
			&storageServer, store_path_index, \
			local_filename, NULL, \
			NULL, 0, group_name, file_id);
	if (result == 0)
	{
        //成功将fileid打印到屏幕
		//printf("%s\n", file_id);
	}
	else
	{
		fprintf(stderr, "upload file fail, " \
			"error no: %d, error info: %s\n", \
			result, STRERROR(result));
	}

    //回收资源
	tracker_disconnect_server_ex(pTrackerServer, true);
	fdfs_client_destroy();

	return result;
}

