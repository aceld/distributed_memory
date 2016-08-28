
#data
gcc -Wall data_cgi.c make_log.c cJSON.c dao_mysql.c util_cgi.c  redis_op.c  -o data -I./include/ -I/usr/include/mysql/ -lfcgi -lm -lmysqlclient -lhiredis

#reg
gcc -Wall reg_cgi.c cJSON.c dao_mysql.c util_cgi.c -o reg -I./include/ -I/usr/include/mysql/ -lfcgi -lm -lmysqlclient

#upload
gcc -Wall upload_cgi.c cJSON.c dao_mysql.c util_cgi.c  make_log.c  redis_op.c  -o upload -I./include/ -I/usr/include/mysql/ -lfcgi -lm -lmysqlclient -lhiredis


#test_redis
gcc -Wall test_redis_main.c make_log.c  redis_op.c  -I./include/ -lhiredis
