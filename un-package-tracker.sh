
cd ./Package/

#fastDFS-common
tar -zxvf libfastcommon-1.0.7.tar.gz
cd libfastcommon-1.0.7/
./make.sh
sudo ./make.sh install

sudo ln -s /usr/lib64/libfastcommon.so /usr/lib/libfastcommon.so


cd ..
#fastDFS
tar -zxvf libfastcommon-1.0.7.tar.gz
tar -zxvf fastdfs-5.05.tar.gz
cd fastdfs-5.05/
./make.sh
sudo ./make.sh install

#fdfs-tracker
sudo mkdir -p /usr/fastdfs/tracker/


#fdfs-nginx-module
cd ..
#nginx
tar -zxvf nginx-1.10.1.tar.gz
cd nginx-1.10.1/
./configure --add-module
make
sudo make install
