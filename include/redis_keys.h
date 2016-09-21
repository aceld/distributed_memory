/**
 * @brief  redis表 KEYS 值相关定义 
 * @author liu_danbing
 */

#ifndef _REDIS_KEYS_H_
#define _REDIS_KEYS_H_

#define REDIS_SERVER_IP     "127.0.0.1"
#define REDIS_SERVER_PORT   "6379"


#define KEY_NAME_SIZ                (50)
#define REDIS_DILIMT                "|"
#define REDIS_DILIMT_SIZ            (3)



/*-----------------------------------------------------------.
| 文件信息表(LIST)                                           |
| Key:     FILE_INFO_LIST                                    |
| Value:   file_id||url||filename||create time||user||type   |
| redis 语句                                                 |
|    插入 LPUSH key value                                    |
|    查询个数 LLEN key                                       |
|    大于最大需要截断 LTRIM key 0 max-1                      |
|    查询链表数据    LRANGE key 0 max-1                      |
`-----------------------------------------------------------*/
#define FILE_INFO_LIST                  "FILE_INFO_LIST"


/*------------------------------------------.
| 点击量文件表 (ZSET)                       |
| Key:     FILE_HOT_ZSET                    |
| Member:  file_id                          |
| Score:   pv                               |
| redis 语句                                |
|    ZINCRBY key increment member           |
`------------------------------------------*/
#define FILE_HOT_ZSET                   "FILE_HOT_ZSET"


/*------------------------------------------.
| 文件ID和文件名对应表 (HASH)               |
| Key:     FILEID_NAME_HASH                 |
| field:  file_id                           |
| value:  file_name                         |
| redis 语句                                |
|    hset key field value                   |
|    hget key field                         |
`------------------------------------------*/
#define FILEID_NAME_HASH                "FILEID_NAME_HASH"

/*------------------------------------------.
| 文件ID和文件创建时间对应表 (HASH)         |
| Key:     FILEID_TIME_HASH                 |
| field:  file_id                           |
| value:  create_time                       |
| redis 语句                                |
|    hset key field value                   |
|    hget key field                         |
`------------------------------------------*/
#define FILEID_TIME_HASH                "FILEID_TIME_HASH"

/*------------------------------------------.
| 文件ID和文件URL对应表 (HASH)              |
| Key:     FILEID_URL_HASH                  |
| field:  file_id                           |
| value:  url                               |
| redis 语句                                |
|    hset key field value                   |
|    hget key field                         |
`------------------------------------------*/
#define FILEID_URL_HASH                 "FILEID_URL_HASH"

/*------------------------------------------.
| 文件ID和文件所属用户对应表 (HASH)         |
| Key:     FILEID_USER_HASH                 |
| field:  file_id                           |
| value:  username                          |
| redis 语句                                |
|    hset key field value                   |
|    hget key field                         |
`------------------------------------------*/
#define FILEID_USER_HASH                 "FILEID_USER_HASH"


/*--------------------------------.
| 文件类型                        |
`--------------------------------*/
#define FILE_TYPE_BMP           "1"
#define FILE_TYPE_ZIP           "2"
#define FILE_TYPE_VEDIO         "3"
#define FILE_TYPE_MEDIA         "4"
#define EVENT_TYPE_TXT          "5"
#define EVENT_TYPE_OTHER        "6"


/*------------------------------------------.
| 文件ID和文件类型对应表 (HASH)             |
| Key:     FILEID_TYPE_HASH                 |
| field:  file_id                           |
| value:  file_type                         |
| redis 语句                                |
|    hset key field value                   |
|    hget key field                         |
`------------------------------------------*/
#define FILEID_TYPE_HASH                 "FILEID_TYPE_HASH"

/*------------------------------------------.
| 文件共享状态表 (HASH)                     |
| Key:     FILE_PUBLIC_LIST                 |
| field:  fileid                            |
| value:  shared_status                     |
| redis 语句                                |
|    hset field value                       |
`------------------------------------------*/
#define FILEID_SHARED_STATUS_HASH           "FILEID_SHARED_STATUS_HASH"

/*------------------------------------------.
| 文件ID 引用计数  表 (HASH)                |
| Key:     FILEID_REFERENCE_COUNT_HASH      |
| field:  file_id                           |
| value:  count                             |
| redis 语句                                |
|    hset key field value                   |
|    hget key field                         |
`------------------------------------------*/
#define FILE_REFERENCE_COUNT_HASH        "FILE_REFERENCE_COUNT_HASH"


/*------------------------------------------.
| 用户名 用户ID  表 (HASH)                  |
| Key:    USER_USERID_HASH                  |
| field:  username                          |
| value:  userid                            |
| redis 语句                                |
|    hset key field value                   |
|    hget key field                         |
`------------------------------------------*/
#define USER_USERID_HASH                 "USER_USERID_HASH"

/*------------------------------------------.
| 个人用户文件列表表 (LIST)                 |
| Key:     FILE_USER_LIST_userid            |
| value:  fileid                            |
| redis 语句                                |
|    lpush key value                        |
`------------------------------------------*/
#define FILE_USER_LIST                   "FILE_USER_LIST_"



/*------------------------------------------.
| 共享用户文件列表表 (LIST)                 |
| Key:     FILE_PUBLIC_LIST                 |
| value:  fileid                            |
| redis 语句                                |
|    lpush key value                        |
`------------------------------------------*/
#define FILE_PUBLIC_LIST                  "FILE_PUBLIC_LIST"







#endif

