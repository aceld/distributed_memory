#启动 必要的业务后台cgi应用程序
spawn-fcgi -a 127.0.0.1 -p 8085 -f ./cgi_bin/data
spawn-fcgi -a 127.0.0.1 -p 8081 -f ./cgi_bin/reg
spawn-fcgi -a 127.0.0.1 -p 8082 -f ./cgi_bin/upload
spawn-fcgi -a 127.0.0.1 -p 8084 -f ./cgi_bin/login


#启动redis服务器
redis-server ./conf/redis.conf

#启动mySQL服务器

#启动nginx服务器
sudo /usr/local/nginx/nginx

if [ $? != 0 ]; then
    echo "reload"
    sudo /usr/local/nginx/nginx -s reload
fi

#强制开启防火墙 端口
sudo iptables -I INPUT -p tcp -m state --state NEW -m tcp --dport 22122 -j ACCEPT
sudo iptables -I INPUT -p tcp -m state --state NEW -m tcp --dport 23000 -j ACCEPT
#sudo iptables -A INPUT -p tcp --dport 3306 -j ACCEPT

#启动本地tracker
sudo /usr/bin/fdfs_trackerd ./conf/tracker.conf restart
#启动本地storage
#sudo /usr/bin/fdfs_storaged ./conf/storage.conf restart

