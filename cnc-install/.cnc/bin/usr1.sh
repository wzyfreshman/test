#!/bin/bash

# Letting user input some values
zenity --question --title="关机选项" --text="是否关闭系统" --no-wrap  --ok-label '确认' --cancel-label '取消'  --width=200 --height=80

if [ $? -eq 0 ]
then
zenity --question --title="关机选项" --text="请选择关机和重启" --no-wrap  --ok-label '关机' --cancel-label '重启' --width=200 --height=80
if [ $? -eq 0 ]
then
echo "in shutdown"
sudo poweroff
else
echo "in reboot"
sudo reboot
fi
fi



