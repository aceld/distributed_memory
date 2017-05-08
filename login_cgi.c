/**
 * @file login_cgi.c
 * @brief   登陆后台CGI程序  
 * @author liu_danbing
 * @version 1.0
 * @date 2016-08-29
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "fcgi_stdio.h"
#include "fcgi_config.h"
#include "util_cgi.h"
#include "util_cgi.h"
#include "cJSON.h"
#include "dao_mysql.h"

#define LOGIN_LOG_MODULE "cgi"
#define LOGIN_LOG_PROC   "login"

/*
   {
      "code": "000"
   } 

 */
int return_login_status(char *status_num)
{
    char *out;
    cJSON *root = cJSON_CreateObject(); 
    cJSON_AddStringToObject(root, "code", status_num);
    out = cJSON_Print(root);

    printf(out);
    free(out);
    return 0;
}

int process_result(MYSQL *conn, MYSQL_RES *res_set, char *pwd)
{
    MYSQL_ROW row;
    uint i;
    ulong line = 0;


    if (mysql_errno(conn) != 0) {
        LOG(LOGIN_LOG_MODULE, LOGIN_LOG_PROC, "mysql_fetch_row() failed");
        return -1;
    }

    line = mysql_num_rows(res_set);
    LOG(LOGIN_LOG_MODULE, LOGIN_LOG_PROC, "%lu rows returned \n", line);
    if (line == 0) {
        return -1;
    }
    

    while ((row = mysql_fetch_row(res_set)) != NULL) {
        for (i = 0; i<mysql_num_fields(res_set); i++)  {
            if (row[i] != NULL) {
                LOG(LOGIN_LOG_MODULE, LOGIN_LOG_PROC, "%d row is %s", i, row[i]);
                if (strcmp(row[i], pwd) == 0) {
                    return 0;
                }
            }
        }
    }

    

    return -1;
}

/* -------------------------------------------*/
/**
 * @brief  判断用户名是否在数据中
 *
 * @param username
 * @param pwd
 *
 * @returns   
 *          0  success， -1 fail;
 */
/* -------------------------------------------*/
int check_username(char *username, char *pwd)
{
    char sql_cmd[SQL_MAX_LEN] = {0};
    int retn = 0;

    MYSQL *conn = msql_conn(MYSQL_USER, MYSQL_PWD, MYSQL_DATABASE);
    if (conn == NULL) {
        return -1;
    }

    sprintf(sql_cmd, "select password from user where u_name=\"%s\"", username);

    if (mysql_query(conn, sql_cmd)) {
        LOG(LOGIN_LOG_MODULE, LOGIN_LOG_PROC,"[-]%s error!", sql_cmd);
        retn = -1;
        goto END;
    }
    else {
        MYSQL_RES *res_set;
        res_set = mysql_store_result(conn);/*生成结果集*/
        if (res_set == NULL) {
            LOG(LOGIN_LOG_MODULE, LOGIN_LOG_PROC,"mysql_store_result error!", sql_cmd);
            retn = -1;
            goto END;
        }

        retn = process_result(conn, res_set, pwd);
    }

END:
    mysql_close(conn);

    return retn;
}


int main(int argc, char *argv[])
{
    char login_type[10] ={0};
    char username[USER_NAME_LEN] = {0};
    char password[PWD_LEN] = {0};
    int retn = 0;


    while (FCGI_Accept() >= 0) {

        printf("Content-type: text/html\r\n");
        printf("\r\n");

        memset(username, 0, USER_NAME_LEN);
        memset(password, 0, PWD_LEN);
        memset(login_type, 0, 10);

        char *query = getenv("QUERY_STRING");

        query_parse_key_value(query, "user", username, NULL);
        query_parse_key_value(query, "pwd", password, NULL);
        query_parse_key_value(query, "type", login_type, NULL);

        LOG(LOGIN_LOG_MODULE, LOGIN_LOG_PROC, "login:[user=%s,pwd=%s,type=%s]", username, password, login_type);


        //做登陆判断
        retn = check_username(username, password);
        if (retn == 0) {
            //返回前段是否登陆成功
            return_login_status("000");
        }
        else {
            return_login_status("001");
        }
        

    }

    
	return 0;
}
