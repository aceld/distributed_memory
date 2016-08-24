#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

#include <errno.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    char buf[1024] = {0};
    int pfd[2];
    int n = 0;

    if (pipe(pfd) < 0) {
        perror("pipe\n");
        exit(1);
    }

    pid = fork();

    if (pid == 0) {
        //child
        //关闭读管道
        close(pfd[0]);

        

        //将标准输出重定向到写管道
        //dup2(STDOUT_FILENO, pfd[1]);
        dup2(pfd[1], STDOUT_FILENO);

        //execlp("fdfs_upload_file", "fdfs_upload_file", "./conf/client.conf", "123.png", NULL);
        //execlp("ls", "ls", "-l", NULL);
        
        execlp("fdfs_file_info", "fdfs_file_info", "./conf/client.conf", "group1/M00/00/00/wKgCbFenJ-CAC8UPAAGWA47EGKY638.png\n", NULL);
    }
    //关闭写管道
    close(pfd[1]);

    //read from 读管道
    if ((n=read(pfd[0], buf, 1024)) < 0) {
        perror("read error");
    }
    printf("n=%d, %s\n",n, buf);

    printf("wait end\n");

    close(pfd[0]);

    return 0;
}
