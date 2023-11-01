#!/bin/bash

if [ $# -gt 0 ];then
input=$1
else
input=0
fi

sudo rm cnc-install/.cnc/lib/libccd* -rf
sudo rm cnc-install/.cnc/lib/libcamera* -rf
#sudo cp emc/gui/ccd/lib/libcamera.so.1.0.0 cnc-install/.cnc/lib/libcamera.so.1 -rf
#sudo cp emc/gui/ccd/lib/libccd.so.1.0.0 cnc-install/.cnc/lib/libccd.so.1 -rf 
#sudo cp emc/gui/ccd/lib/libccdcore.so.1 cnc-install/.cnc/lib/libccdcore.so.1 -rf

if [ $input -eq 1 ];then
sudo rm /home/.cnc -rf
sudo cp ./cnc-install/.cnc /home/ -rf
else

var=$(/usr/bin/pickconfig ./configs)
sudo cp ./emc/gui/ui/tran_zh_CN.qm ./cnc-install/.cnc/gui/
sudo rm ./cnc-install/.config -rf
sudo cp $var ./cnc-install/.config -rf

nowdate=$(date +%Y%m%d)

sudo tar -cf ALL.tar ./cnc-install
sudo zip -rP CNEXT2018 cnc-install.zip ALL.tar
sudo rm ALL.tar 
sudo mv  cnc-install.zip  ../CNEXT-CAM-A809-$nowdate.cnc
fi

