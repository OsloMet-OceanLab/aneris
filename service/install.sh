#!/bin/bash
#create /etc/aneris
#setup service

if [ "$EUID" -ne 0 ]
  then echo "This script needs to be run as root\n"
  exit
fi

mkdir /etc/aneris

cp aneris /etc/aneris/
cp -r Web_Server/ /etc/aneris/
cp -r gpio/ /etc/aneris/
cp -r Logger/ /etc/aneris/
cp -r web/ /etc/aneris/

cp aneris.service /etc/systemd/system/
systemctl daemon-reload

read -n1 -p "Would you like to enable the aneris service right now? [y,n]" enable
case $enable in
  y|Y)
    systemctl enable aneris
    ;;
  n|N)
    ;;
  *)
    ;;
esac
