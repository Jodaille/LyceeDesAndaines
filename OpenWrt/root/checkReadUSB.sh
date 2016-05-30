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
SCRIPT="/root/readUSB.sh"

readUSBCount=`ps  | grep -v grep | grep readUSB.sh | wc -l`

LOGFILE="/tmp/checkReadUSB.log"

echo $readUSBCount;

if [ $readUSBCount -ge 1 ]
then
  echo "Count is greater or equal than 2"
else
  echo $SHELL
  echo $NOW starting $SCRIPT >> $LOGFILE

  /usr/sbin/screen -Sdm readArduino -s $SCRIPT
  echo "launching screen readArduino"
fi
