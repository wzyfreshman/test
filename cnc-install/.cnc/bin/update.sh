#!/bin/bash

#sudo killall ui
getker=$(uname -r)
kerver=$(echo $getker | awk -F '-' '{print $1}')


sleep 2

input=$1
cd /home/.tmp/updater/cnc-install

sudo rm /home/.cam -rf
sudo tar zxvfP ./install_package.tgz

sudo chmod 777 /home/.cnc -R
sudo chmod 777 /home/.config -R
sudo chmod 777 /home/comp -R

if [ $input -eq 2 ];then
sudo rm /home/.cnc -rf
sudo rm /home/.config -rf
sudo cp ./.cnc /home/ -rf
sudo cp ./.config /home/ -rf
sudo cp ./comp /home/ -rf
elif [ $input -eq 1 ];then
sudo cp /home/.cnc/info* /home/ 
sudo rm /home/.cnc -rf
sudo cp ./.cnc /home/ -rf
sudo rm /home/.cnc/info
sudo mv /home/info* /home/.cnc/

sudo cp ./.config/io.hal ./.config/cfg.hal ./.config/tool.tbl ./.config/alarm /home/.config 
sudo cp ./.config/subNgc /home/.config/ -rf

if [ ! -f /home/.config/alarm ];then
    sudo cp ./.config/alarm /home/.config/
fi 

if [ ! -f /home/.config/preference ];then
    sudo cp ./.config/preference /home/.config/
fi 

cd ./.cnc/bin/modules
sudo ./update.sh 
cd ../../../
else
echo '升级失败,请重新选择模式升级'
sleep 2
exit
fi


sudo mkdir /home/cnext/Title
sudo cp ./Title/icon.png ./Title/title.txt /home/cnext/Title -rf
sudo chmod 777 /home/cnext/Title -R

sudo cp ./.config/0 /home/.config/ -rf
sudo cp ./.config/1 /home/.config/ -rf

sudo cp /home/.cnc/bin/meter /home/.cnc/bin/halmeter -rf

sudo cp ./icon ~/ -rf
sudo chmod 777 ~/icon -R

sudo chmod 755 /home/.cnc -R
sudo chmod 755 /home/.config -R
sudo chmod 777 /home/comp -R
sudo chown root /home/.config/*
sudo chown root /home/.config/subNgc/*

sudo chown root /home/.cnc/bin/*
sudo chmod 4755 /home/.cnc/bin/*

sudo chown root /home/.cnc/gui/ui
sudo chmod 4755 /home/.cnc/gui/ui
sudo chown root /home/.cnc/gui/backup
sudo chmod 4755 /home/.cnc/gui/backup

sudo chown root /dev/mem
sudo chmod 4755 /dev/mem

sudo ldconfig

sudo sync

sleep 1

echo '升级完毕！'
 
