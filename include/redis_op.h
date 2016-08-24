/**
 * @file   redis_api.h
 * @brief  redis 封装接口
 */

#ifndef _REDIS_OP_H_
#define _REDIS_OP_H_

#include "hiredis.h"
#include "redis_keys.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "make_log.h"


#define REDIS_LOG_MODULE          "database"
#define REDIS_LOG_PROC            "redis"

#define REDIS_COMMAND_SIZE        300            /* redis Command 指令最大长度 */
#define FIELD_ID_SIZE            100            /* redis hash表field域字段长度 */
#define VALUES_ID_SIZE           1024            /* redis        value域字段长度 */
typedef char (*RCOMMANDS)[REDIS_COMMAND_SIZE];/* redis 存放批量 命令字符串数组类型 */
typedef char (*RFIELDS)[FIELD_ID_SIZE];        /* redis hash表存放批量field字符串数组类型 */
typedef char (*RVALUES)[VALUES_ID_SIZE];    /* redis 表存放批量value字符串数组类型 */


/* -------------------------------------------*/
/**
 * @brief  redis tcp模式链接
 *
 * @param ip_str	redis服务器ip
 * @param port_str	redis服务器port
 *
 * @returns   
 *			成功返回链接句柄 
 *			失败返回NULL
 */
/* -------------------------------------------*/
redisContext* rop_connectdb_nopwd(char *ip_str, char* port_str);


/* -------------------------------------------*/
/**
 * @brief  redis tcp模式链接
 *
 * @param ip_str    redis服务器ip
 * @param port_str  redis服务器port
 * @param pwd       redis服务器密码
 *
 * @returns   
 *            成功返回链接句柄 
 *            失败返回NULL
 */
/* -------------------------------------------*/
redisContext* rop_connectdb(char *ip_str, char* port_str, char *pwd);

/* -------------------------------------------*/
/**
 * @brief  redis unix域模式链接
 *
 * @param ip_str    unix域sock文件
 * @param  pwd      redis服务器密码
 *
 * @returns   
 *            成功返回链接句柄 
 *            失败返回NULL
 */
/* -------------------------------------------*/
redisContext* rop_connectdb_unix(char *sock_path, char *pwd);

/* -------------------------------------------*/
/**
 * @brief  tcp 链接redis超时等待模式，timeval链接超时
 *            返回
 *
 * @param ip_str        redis 服务器ip
 * @param port_str        redis 服务器端口
 * @param timeval        最大超时等待时间
 *
 * @returns   
 *        成功返回链接句柄
 *        失败返回NULL
 */
/* -------------------------------------------*/
redisContext* rop_connectdb_timeout(char* ip_str, char *port_str, struct timeval *timeout);


/* -------------------------------------------*/
/**
 * @brief  关闭指定的链接句柄
 *
 * @param conn    已建立好的链接
 */
/* -------------------------------------------*/
void rop_disconnect(redisContext* conn);

/* -------------------------------------------*/
/**
 * @brief  选择redis中 其中一个数据库
 *
 * @param conn        已链接的数据库链接
 * @param db_no        redis数据库编号
 *
 * @returns   
 *            -1 失败
 *            0  成功
 */
/* -------------------------------------------*/
int rop_selectdatabase(redisContext *conn, unsigned int db_no);

/* -------------------------------------------*/
/**
 * @brief            清空当前数据库所有信息(慎用)
 *
 * @param conn        已链接的数据库链接
 *
 * @returns   
 *            -1 失败
 *            0  成功
 */
/* -------------------------------------------*/
int rop_flush_database(redisContext *conn);

/* -------------------------------------------*/
/**
 * @brief  判断key值是否存在
 *
 * @param conn        已经建立的链接
 * @param key        需要寻找的key值
 *
 * @returns   
 *                -1 失败
 *                1 存在
 *                0 不存在
 */
/* -------------------------------------------*/
int rop_is_key_exist(redisContext *conn, char* key);

/* -------------------------------------------*/
/**
 * @brief            删除一个key
 *
 * @param conn        已经建立的链接
 * @param key        
 *
 * @returns   
 *                -1 失败
 *                0 成功
 */
/* -------------------------------------------*/
int rop_del_key(redisContext *conn, char *key);

/* -------------------------------------------*/
/**
 * @brief  打印库中所有匹配pattern的key
 *
 * @param conn        已建立好的链接
 * @param pattern    匹配模式，pattern支持glob-style的通配符格式，
 *                    如 *表示任意一个或多个字符，
 *                       ?表示任意字符，
 *                    [abc]表示方括号中任意一个字母。
 */
/* -------------------------------------------*/
void rop_show_keys(redisContext *conn, char* pattern);

/* -------------------------------------------*/
/**
 * @brief  设置一个key的删除时间 ，系统到达一定时间
 *            将会自动删除该KEY
 *
 * @param conn                已经建立好的链接
 * @param delete_time        到期事件 time_t 日历时间
 *
 * @returns   
 *        0    SUCC
 *        -1  FAIL
 */
/* -------------------------------------------*/
int rop_set_key_lifecycle(redisContext *conn, char *key, time_t delete_time);


/* -------------------------------------------*/
/**
 * @brief            创建或者覆盖一个HASH表
 *
 * @param conn                已建立好的链接
 * @param key                hash 表名
 * @param element_num        hash 表区域个数
 * @param fields            hash 表区域名称数组char(*)[FIELD_ID_SIZE]
 * @param values            hash 表区域值数组  char(*)[VALUES_ID_SIZE]
 *
 * @returns   
 *            0   成功    
 *            -1  失败
 */
/* -------------------------------------------*/
int rop_create_or_replace_hash_table(redisContext* conn,
                                     char* key, 
                                     unsigned int element_num, 
                                     RFIELDS fields, 
                                     RVALUES values);

/* -------------------------------------------*/
/**
 * @brief  给指定的hash表 指定的field对应的value自增num
 *
 * @param conn            已建立好的链接
 * @param key            hash表名
 * @param field            hash表下的区域名    
 *
 * @returns   
 *            0        succ
 *            -1        fail
 */
/* -------------------------------------------*/
int rop_hincrement_one_field(redisContext *conn, char *key, char *field, unsigned int num);


/* -------------------------------------------*/
/**
 * @brief  批量执行链表插入命令 插入链表头部
 *
 * @param conn        已建立好的链接
 * @param key        链表名
 * @param values    封装好的域名
 * @param values    封装好的值数组
 * @param val_num    值个数
 *
 * @returns   
 *            0        succ
 *            -1        FAIL
 */
/* -------------------------------------------*/
int rop_hash_set_append(redisContext *conn, char *key, RFIELDS fields, RVALUES values, int val_num);

/* -------------------------------------------*/
/**
 * @brief        将指定的zset表，对应的成员，值自增1
 *                （key 或 成员不存在 则创建）
 *
 * @param conn        已建立的链接
 * @param key        zset表名
 * @param member    zset成员名
 *
 * @returns   
 *            0            succ
 *            -1            fail
 */
/* -------------------------------------------*/
int rop_zset_increment(redisContext *conn, char* key, char* member);


//得到zset一个member的score
int rop_zset_get_score(redisContext *conn, char *key, char *member);

/* -------------------------------------------*/
/**
 * @brief     批量将指定的zset表，对应的成员，值自增1
 *                （key 或 成员不存在 则创建）
 *
 * @param conn        已建立好的链接
 * @param key        有序集合名称
 * @param values    封装好的成员数组
 * @param val_num    数据个数
 *
 * @returns   
 *            0        succ
 *            -1        FAIL
 */
/* -------------------------------------------*/
int rop_zset_increment_append(redisContext *conn, char *key, RVALUES values, int val_num);

/* -------------------------------------------*/
/**
 * @brief  批量执行链表插入命令 插入链表头部
 *
 * @param conn        已建立好的链接
 * @param key        链表名
 * @param values    封装好的值数组
 * @param val_num    值个数
 *
 * @returns   
 *            0        succ
 *            -1        FAIL
 */
/* -------------------------------------------*/
int rop_list_push_append(redisContext *conn, char *key, RVALUES values, int val_num);

/* -------------------------------------------*/
/**
 * @brief  单条数据插入链表
 *
 * @param conn        已建立好的链接
 * @param key        链表名
 * @param value        数据
 *
 * @returns   
 */
/* -------------------------------------------*/
int rop_list_push(redisContext *conn, char *key, char *value);

/* -------------------------------------------*/
/**
 * @brief  得到链表中元素的个数
 *
 * @param conn    链接句柄
 * @param key    链表名
 *
 * @returns   
 *            >=0 个数
 *            -1 fail
 */
/* -------------------------------------------*/
int rop_get_list_cnt(redisContext *conn, char *key);

/* -------------------------------------------*/
/**
 * @brief  按照一定范围截断链表中的数据
 *
 * @param conn        已经建立的链接
 * @param key        链表名
 * @param begin        阶段启示位置 从 0 开始
 * @param end        阶段结束位置 从 -1 开始
 *
 *                    这里的范围定义举例 
 *                    如果得到全部范围(0, -1)
 *                    除了最后一个元素范围(0, -2)
 *                    前20各数据范围(0, 19)
 *
 * @returns   
 *            0  SUCC
 *            -1 FAIL
 */
/* -------------------------------------------*/
int rop_trim_list(redisContext *conn, char *key, int begin, int end);

/* -------------------------------------------*/
/**
 * @brief          得到链表中的数据
 *
 * @param conn		已经建立的链接
 * @param key		链表名
 *
 * @returns   
 *			0  SUCC
 *			-1 FAIL
 */
/* -------------------------------------------*/
int rop_range_list(redisContext *conn, char *key, int from_pos, int count, RVALUES values, int *get_num);


/* -------------------------------------------*/
/**
 * @brief  批量执行已经封装好的redis 命令
 *
 * @param conn        已建立好的链接
 * @param cmds        封装好的命令数组
 * @param cmd_num    命令个数
 *
 * @returns   
 *            0        succ
 *            -1        FAIL
 */
/* -------------------------------------------*/
int rop_redis_append(redisContext *conn, RCOMMANDS cmds, int cmd_num);


/* -------------------------------------------*/
/**
 * @brief  执行单向命令 无返回值 命令自行输入
 *
 * @param conn        已建立的链接
 * @param cmd        封装好的命令
 *
 * @returns   
 *            0        succ
 *            -1        FAIL
 */
/* -------------------------------------------*/
int rop_redis_command(redisContext *conn, char *cmd);

/* -------------------------------------------*/
/**
 * @brief  测试一个reply的结果类型
 *            得到对应的类型用对应的方法获取数据
 *
 * @param reply        返回的命令结果
 */
/* -------------------------------------------*/
void rop_test_reply_type(redisReply *reply);


#endif
