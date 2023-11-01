#!/bin/bash

msgfmt ./zh_CN.po 
cp ./messages.mo ../cnc-install/.cnc/share/locale/zh_CN/LC_MESSAGES/linuxcnc_zh.mo -rf
