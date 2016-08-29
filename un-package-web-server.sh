
cd ./Package/

#fastDFS-common
tar -zxvf libfastcommon-1.0.7.tar.gz
cd libfastcommon-1.0.7/
./make.sh
sudo ./make.sh install

sudo ln -s /usr/lib64/libfastcommon.so /usr/lib/libfastcommon.so
sudo ln -s /usr/lib64/libfdfsclient.so /usr/lib/libfdfsclient.so


cd ..
#fastDFS
tar -zxvf libfastcommon-1.0.7.tar.gz
tar -zxvf fastdfs-5.05.tar.gz
cd fastdfs-5.05/
./make.sh
sudo ./make.sh install

#fdfs-tracker
sudo mkdir -p /usr/fastdfs/tracker/
#fdfs-client
sudo mkdir -p /usr/fastdfs/client/

cd ..
#redis-server
tar -zxvf redis-3.2.1.tar.gz 
cd redis-3.2.1/
make
sudo make install

cd ..
#redis-C-API hiredis
unzip redis-hiredis-v0.13.3-52-g5f98e1d.zip
cd redis-hiredis-5f98e1d/
make
sudo make install

cd ..
#pcre
tar -zxvf pcre-8.39.tar.gz 
cd pcre-8.39/
./configure
make
sudo make install

cd ..
#zlib
tar -zxvf zlib-1.2.8.tar.gz
cd zlib-1.2.8/
./configure
make
sudo make install

cd ..
#openssl
tar -zxvf openssl-1.0.1t.tar.gz
cd openssl-1.0.1t/
./Configure
./config
make
sudo make install


cd ..
#nginx
tar -zxvf nginx-1.10.1.tar.gz
cd nginx-1.10.1/
./configure
make
sudo make install

cd ..
cd ..
#nginx.conf
sudo cp ./conf/web-server/nginx.conf /usr/local/nginx/conf/
#html web
sudo cp -r ./conf/web-server/html/ /usr/local/nginx/

cd ./Package/


#fastcgi
tar -zxvf fcgi-2.4.1-SNAP-0910052249.tar.gz
cd fcgi-2.4.1-SNAP-0910052249/
./configure
make
#maybe error, should add "include <cstdio>" --> ./include/fcgio.h
sudo make install
sudo ln -s /usr/local/lib/libfcgi.so.0 /usr/lib/libfcgi.so.0

cd ..
#spawn-fcgi
tar -zxvf spawn-fcgi-1.6.4.tar.gz 
cd spawn-fcgi-1.6.4/
./configure
make
sudo make install

cd ..
#mysql-server
sudo apt-get install mysql-server
#mysql-client
sudo apt-get install libmysqlclient-dev

#d-storage  web-server
make
