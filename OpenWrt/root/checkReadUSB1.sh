#!/bin/ash
#
# Check that screen is running
#
# Should be run by crontab :
#
# */5 * * * * (. /etc/profile; /checkReadusb.sh > /dev/null)
#

NOW=$(/bin/date +"%H:%M %d/%m/%Y")
#~/.profile
SCRIPT="/root/readUSB1.sh"

readUSBCount=`ps  | grep -v grep | grep readUSB1.sh | wc -l`

sleep 1

LOGFILE="/tmp/checkReadUSB.log"

echo $readUSBCount;

if [ $readUSBCount -ge 1 ]
then
  echo "Count is greater or equal than 2"
else
  echo $SHELL
  echo $NOW starting $SCRIPT >> $LOGFILE

  /usr/sbin/screen -Sdm readArduino1 -s $SCRIPT
  echo "launching screen readArduino1"
fi
