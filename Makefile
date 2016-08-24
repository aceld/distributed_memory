
CC=gcc
CPPFLAGS=-I./include -I./ -I/usr/include/mysql/
CFLAGS=-Wall 
LIBS= -lfcgi -lm -lmysqlclient -lhiredis

#找到当前目录下所有的.c文件
src = $(wildcard *.c)

#将当前目录下所有的.c  转换成.o给obj
obj = $(patsubst %.c, %.o, $(src))



upload=cgi_bin/upload
data=cgi_bin/data
reg=cgi_bin/reg

target=$(upload) $(data) $(reg)


ALL:$(target)


#生成所有的.o文件
$(obj):%.o:%.c
	$(CC) -c $< -o $@ $(CPPFLAGS) $(CFLAGS) 


#upload cgi程序
$(upload):upload_cgi.o cJSON.o dao_mysql.o util_cgi.o make_log.o redis_op.o
	$(CC) $^ -o $@ $(LIBS)

#data cgi程序
$(data): data_cgi.o cJSON.o dao_mysql.o util_cgi.o make_log.o redis_op.o
	$(CC) $^ -o $@ $(LIBS)

#reg cgi程序
$(reg): reg_cgi.o cJSON.o dao_mysql.o util_cgi.o make_log.o redis_op.o
	$(CC) $^ -o $@ $(LIBS)




#clean指令

clean:
	-rm -rf $(obj) $(target)

distclean:
	-rm -rf $(obj) $(target)

#将clean目标 改成一个虚拟符号
.PHONY: clean ALL distclean
