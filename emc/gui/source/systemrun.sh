#!/bin/bash
input=$1

echo ' '
if [ $input -eq 1 ];then
echo '正在备份MasterCam文件...'
sudo cp /home/cnext/MasterCam /home/.ScriptStart/ -rf
sudo chmod -R 777 /home/.ScriptStart/MasterCam
sync

elif [ $input -eq 2 ];then
echo '正在删除文件...'
sudo rm /home/.ScriptStart/ -rf
echo '正在重装Windows XP...'
sudo tar -zxvf /home/cnext/ScriptStart.tar  -C /
sudo chmod 777 /home/.ScriptStart -R
echo '正在恢复出厂文件...'
sudo rm /home/cnext/MasterCam/ -rf
sudo cp /home/.ScriptStart/MasterCam /home/cnext/ -rf
sudo chmod 777 /home/cnext/MasterCam/ -R
sync

elif [ $input -eq 3 ];then
echo '启动传送软件...'
sudo /home/.cnc/bin/qipmsg
sync

else
sleep 1
exit 
fi
