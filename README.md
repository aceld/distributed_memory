# distributed_memory


##1. 配置文件

### 应用服务器配置

配置文件在 ./conf/web-server/
* nginx.conf ----nginx服务器配置文件
* redis.conf ----redis服务器配置文件

### 前端界面

./conf/web-server/html/ 路径下为前端界面html、js、css代码

将html直接部署在安装好的web服务器下即可.
例如：/usr/local/nginx/html/

### fastDFS分布式存储

* storage 

存储服务器配置文件 ./conf/storage.conf  

nginx中fastDFS模块配置文件 ./conf/mod_fastdfs.conf ---> /etc/fdfs/下
                           ./conf/mime.types       ---> /etc/fdfs/下
                           ./conf/http.conf        ---> /etc/fdfs/下

* tracker ./conf/tracker.conf
* client  ./conf/client.conf


##2. 生成CGI程序

make 后会生成 
cgi_bin/upload    ----处理文件上传后台cgi程序

cgi_bin/data      ----处理展示主界面后台cgi程序

cgi_bin/reg       ----处理用户注册后台cgi程序

cgi_bin/login     ----处理文件登陆后台cgi程序

##3. 启动脚本

start_up.sh

