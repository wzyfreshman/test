#!/bin/bash

dir=/home/cnc-install
if [ ! -d $dir ];then
sudo mkdir $dir
fi
sudo rm /home/cnc-install/* -rf
sudo rm /home/cnc-install/.cnc -rf
sudo rm /home/cnc-install/.config -rf
sudo cp /home/.cnc $dir -rf
sudo cp /home/.config $dir -rf
sudo cp /home/comp $dir -rf
sudo cp /home/.cnc/bin/update.sh $dir -rf
sudo cp /home/.cnc/bin/updater $dir -rf
sudo cp /home/.cnc/bin/modules $dir -rf

sudo cp /home/cnext/Title $dir -rf

sudo cp /home/ecnc/icon $dir -rf
sudo touch /home/cnc-install/pkg_version_v3

sudo tar czvfP /home/cnc-install/install_package.tgz  /home/.cam/ /usr/lib/python2.7/dist-packages/area.so


cd /home/
time=$(date +%F)
file="CNEXT-CAM-A809("$time").cnc"

sudo tar -cf ALL.tar ./cnc-install
sudo zip -rP CNEXT2018 /home/$file ALL.tar
sudo rm ALL.tar 
sudo mv /home/$file ~/桌面/

echo "finish getInstall"
sleep 2
