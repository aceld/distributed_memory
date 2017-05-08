#ifndef __FDFS_API_H__
#define __FDFS_API_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

#include "fdfs_client.h"
#include "logger.h"

#include "make_log.h"

#define FILEID_LEN (256)

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
int fdfs_upload_by_filename(const char* filename, char *fileid);

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
int fdfs_upload_by_filename1(const char *local_filename, char *fileid);

#endif
