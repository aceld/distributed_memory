#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "redis_op.h"


int main(int argc, char **argv)
{
	int retn = 0;

	redisContext* conn = NULL;
#if 0
	struct timeval tval = {1, 500000};	//1.5 seconds
#endif
	
	/* 链接数据库 等待1.5s */
	conn = rop_connectdb_nopwd("127.0.0.1", "6379");
	if (conn == NULL) {
		goto EXIT_WITHOUT_DISCONN;
	}


	/* 打印库中已经存在的key */
	rop_show_keys(conn, "*");

    RVALUES show_values = NULL;
    int value_num = 0;
    int i = 0;

    show_values = malloc(20 * VALUES_ID_SIZE);


    retn = rop_range_list(conn, "T_PM_EVENTLOG_ATTACK_ATTACK",0, 20, show_values, &value_num);
    if (retn < 0) {
        goto EXIT;
    }

    char *a_str = NULL;
    char *b_str = NULL;

    int col = 1;
    int count = 0;
    char get_value[VALUES_ID_SIZE];
    memset(get_value, 0, VALUES_ID_SIZE);

    for (i = 0; i < value_num; ++i) {
        memset(get_value, 0, VALUES_ID_SIZE);

        count = 0; 
        printf("%s\n", show_values[i]);
        a_str=NULL;
        b_str=NULL;
        b_str = show_values[i];

        while ((a_str = strtok_r(b_str, "||", &b_str)) != NULL) {
            
            if (count == col) {
                if (strncmp(get_value, a_str, 20) < 0) {
                    strncpy(get_value, a_str, 20);
                }
            }
            ++count;
        }

    }

    printf("==============%s============\n", get_value);

    free(show_values);

    int score = 0;
    score = rop_zset_get_score(conn, "FILE_HOT_ZSET", "group1/M00/00/00/wKgCbFepTp2AUnd1AFRAAvu2TSg259.mp3");
    printf("score = %d\n", score);

#if 0
	/* 清空当前数据库所有数据 */
	retn = rop_flush_database(conn);
	if (retn < 0) {
		goto EXIT;
	}
#endif
		
#if 0
	/* 查看制定key 是否存在？*/
	retn = rop_is_key_exist(conn, "foo");
	if (retn < 0) {
		printf("is key exist error\n");
	}
	else if (retn == 0){
		printf("foo is not exist \n");
	}
	else if (retn == 1) {
		printf("foo is exist\n");
	}
#endif

	/* 创建一个hash表 */
#if 0
	RFIELDS hfields = malloc(2 * FIELD_ID_SIZE);
	RVALUES hvalues = malloc(2 * VALUES_ID_SIZE);
	strncpy(hfields[0], "event_type1", FIELD_ID_SIZE);
	strncpy(hfields[1], "event_type2", FIELD_ID_SIZE);
	strncpy(hvalues[0], "0", VALUES_ID_SIZE);
	strncpy(hvalues[1], "0", VALUES_ID_SIZE);
	retn = rop_create_or_replace_hash_table(conn, "ffff", 2, hfields, hvalues);
	if (retn < 0) {
		printf("create error\n");
	}
	free(hfields);
	free(hvalues);
#endif

#if 0

	/* 给hash相应的hash表 对应的字段值增1 */
	retn = rop_hincrement_one_field(conn, "ffff", "event_type1");
	if (retn < 0) {
		//...
	}

	/* 给zset 表相应的 字段值自增1 */
	retn = rop_increment_sort_set(conn, "T_PM_EVENTLOG_ATTACK_TOP5", "1.1.1.1");
	if (retn  < 0) {

	}
#endif

#if 0
	retn = rop_get_list_cnt(conn, "T_PM_EVENTLOG_AV");
	if (retn < 0) {
		goto EXIT;
	}
	printf("cnt = %d\n", retn);

	retn = rop_trim_list(conn, "T_PM_EVENTLOG_AV", 0, 1);


	retn = rop_get_list_cnt(conn, "T_PM_EVENTLOG_AV");
	if (retn < 0) {
		goto EXIT;
	}
	printf("after cnt = %d\n", retn);
#endif

    



EXIT:
	rop_disconnect(conn);	

EXIT_WITHOUT_DISCONN:

	return 0;
}

#if 0

#include <hiredis.h>
int main(int argc, char **argv) {
    unsigned int j;
    redisContext *c;
    redisReply *reply;
    const char *hostname = (argc > 1) ? argv[1] : "127.0.0.1";
    int port = (argc > 2) ? atoi(argv[2]) : 6379;

    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    c = redisConnectWithTimeout(hostname, port, timeout);
    if (c == NULL || c->err) {
        if (c) {
            printf("Connection error: %s\n", c->errstr);
            redisFree(c);
        } else {
            printf("Connection error: can't allocate redis context\n");
        }
        exit(1);
    }

    /* PING server */
    reply = redisCommand(c,"PING");
    printf("PING: %s\n", reply->str);
    freeReplyObject(reply);

    /* Set a key */
    reply = redisCommand(c,"SET %s %s", "foo", "hello world");
    printf("SET: %s\n", reply->str);
    freeReplyObject(reply);

    /* Set a key using binary safe API */
    reply = redisCommand(c,"SET %b %b", "bar", (size_t) 3, "hello", (size_t) 5);
    printf("SET (binary API): %s\n", reply->str);
    freeReplyObject(reply);

    /* Try a GET and two INCR */
    reply = redisCommand(c,"GET foo");
    printf("GET foo: %s\n", reply->str);
    freeReplyObject(reply);

    reply = redisCommand(c,"INCR counter");
    printf("INCR counter: %lld\n", reply->integer);
    freeReplyObject(reply);
    /* again ... */
    reply = redisCommand(c,"INCR counter");
    printf("INCR counter: %lld\n", reply->integer);
    freeReplyObject(reply);

    /* Create a list of numbers, from 0 to 9 */
    reply = redisCommand(c,"DEL mylist");
    freeReplyObject(reply);
    for (j = 0; j < 10; j++) {
        char buf[64];

        snprintf(buf,64,"%d",j);
        reply = redisCommand(c,"LPUSH mylist element-%s", buf);
        freeReplyObject(reply);
    }

    /* Let's check what we have inside the list */
    reply = redisCommand(c,"LRANGE mylist 0 -1");
    if (reply->type == REDIS_REPLY_ARRAY) {
        for (j = 0; j < reply->elements; j++) {
            printf("%u) %s\n", j, reply->element[j]->str);
        }
    }
    freeReplyObject(reply);

    /* Disconnects and frees the context */
    redisFree(c);

    return 0;
}
#endif
