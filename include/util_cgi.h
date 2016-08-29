#ifndef _UTIL_CGI_H_
#define _UTIL_CGI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "cJSON.h"

#include "make_log.h"

#define USER_NAME_LEN       (128)
#define PWD_LEN             (256)
#define HOST_NAME_LEN       (30)
#define FILE_NAME_LEN       (256)
#define PIC_URL_LEN         (256)
#define PIC_NAME_LEN        (10)
#define FILE_URL_LEN        (512)
#define TEMP_BUF_MAX_LEN    (256)
#define SUFFIX_LEN          (8)
#define TIME_STRING_LEN     (25)

#define FDFS_CLIENT_CONF    "./conf/client/client.conf"

#define UTIL_LOG_MODULE     "cgi"
#define UTIL_LOG_PROC       "util"



void cgi_init(void);

void str_replace(char* strSrc, char* strFind, char* strReplace);


char* memstr(char* full_data, int full_data_len, char* substr);

/**
 * @brief  得到文件后缀字符串 如果非法文件后缀,返回"null"
 */
int get_file_suffix(const char *file_name, char *suffix);

/**
 * @brief  解析url query 类似 abc=123&bbb=456 字符串
 *          传入一个key,得到相应的value
 * @returns   
 *          0 成功, -1 失败
 */
int query_parse_key_value(const char *query, const char *key, char *value, int *value_len_p);


/**
 * @brief  去掉一个字符串两边的空白字符
 *
 * @param inbuf确保inbuf可修改
 *
 * @returns   
 *      0 成功
 *      -1 失败
 */
int trim_space(char *inbuf);

/* -------------------------------------------*/
/**
 * @brief  得到本行的制定字段数据 字段号从1开始
 *
 * @param line_head        本行首地址
 * @param col            字段号 
 * @param value            得到的数据
 * @param max_len        数据最大长度
 * @param with_quote    该字段是否有“”包括 1 是 其他否
 *
 * @returns   
 *        得到的数据
 */
/* -------------------------------------------*/
char* get_value_by_col(char *line_head, int col, char *value, int max_len, char with_quote);

#endif
