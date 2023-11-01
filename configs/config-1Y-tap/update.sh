#!/bin/bash

#sudo killall ecnc

sleep 2

input=$1
#cd /home/.tmp/updater/cnc-install


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
sudo cp /home/.cnc/ecnc_info* /home/ 
sudo rm /home/.cnc -rf
sudo cp ./.cnc /home/ -rf
sudo rm /home/.cnc/ecnc_info
sudo mv /home/ecnc_info* /home/.cnc/

sudo cp ./.config/ecnc_io.hal ./.config/ecnc_mm.hal ./.config/ecnc_mm.tbl ./.config/ecnc_alarm /home/.config 
sudo cp ./.config/subNgc /home/.config/ -rf

if [ ! -f /home/.config/ecnc_alarm ];then
    sudo cp ./.config/ecnc_alarm /home/.config/
fi 

if [ ! -f /home/.config/ecnc_preference ];then
    sudo cp ./.config/ecnc_preference /home/.config/
fi 

cd ./modules
sudo ./update.sh 
cd ..
else
echo '升级失败,请重新选择模式升级'
sleep 2
exit
fi

sudo ./.cnc/bin/recovery.run

sudo chmod 755 /home/.cnc -R
sudo chmod 755 /home/.config -R
sudo chmod 777 /home/comp -R
sudo chown root /home/.config/*
sudo chown root /home/.config/subNgc/*

sudo chown root /home/.cnc/bin/*
sudo chmod 4755 /home/.cnc/bin/*

sudo chown root /home/.cnc/gui/ecnc
sudo chmod 4755 /home/.cnc/gui/ecnc
sudo chown root /home/.cnc/gui/backup
sudo chmod 4755 /home/.cnc/gui/backup

sudo chown root /dev/mem
sudo chmod 4755 /dev/mem

sudo cp -af ./home/.cnc/bin/RunWin.sh /home/.ScriptStart/
sudo chmod 555 /home/.ScriptStart/RunWin.sh
sync

echo '升级完毕！'
 
