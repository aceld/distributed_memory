
CC=gcc
CPPFLAGS=-I./include -I./ -I/usr/include/mysql/ -I/usr/include/fastdfs/ -I/usr/include/fastcommon/
CFLAGS=-Wall 
LIBS= -lfcgi -lm -lmysqlclient -lhiredis -lfdfsclient

#找到当前目录下所有的.c文件
src = $(wildcard *.c)

#将当前目录下所有的.c  转换成.o给obj
obj = $(patsubst %.c, %.o, $(src))


#test
test_config=test/test_config
test_redis=test/test_redis

#cgi
upload=cgi_bin/upload
data=cgi_bin/data
reg=cgi_bin/reg
login=cgi_bin/login

target=$(upload) $(data) $(reg) $(login) $(test_config) $(test_redis)


ALL:$(target)


#生成所有的.o文件
$(obj):%.o:%.c
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS) 


#upload cgi程序
$(upload):upload_cgi.o \
          cJSON.o \
          dao_mysql.o \
          util_cgi.o \
          make_log.o \
          redis_op.o \
          config.o

	$(CC) $^ -o $@ $(LIBS)


#data cgi程序
$(data): data_cgi.o \
         cJSON.o \
         dao_mysql.o \
         util_cgi.o \
         make_log.o \
         redis_op.o \
         config.o

	$(CC) $^ -o $@ $(LIBS)


#reg cgi程序
$(reg): reg_cgi.o \
        cJSON.o \
        dao_mysql.o \
        util_cgi.o \
        make_log.o \
        redis_op.o \
        config.o

	$(CC) $^ -o $@ $(LIBS)


#login cgi程序
$(login): login_cgi.o \
          cJSON.o \
          dao_mysql.o \
          util_cgi.o \
          make_log.o \
          redis_op.o \
          config.o

	$(CC) $^ -o $@ $(LIBS)


#test_config 程序
$(test_config): test/test_config.o \
                config.o

	$(CC) $^ -o $@ $(LIBS)

#test_redis 程序
$(test_redis): test/test_redis_main.o \
                redis_op.o \
                make_log.o 

	$(CC) $^ -o $@ $(LIBS)




#clean指令

clean:
	-rm -rf $(obj) $(target) ./test/*.o

distclean:
	-rm -rf $(obj) $(target) ./test/*.o

#将clean目标 改成一个虚拟符号
.PHONY: clean ALL distclean
