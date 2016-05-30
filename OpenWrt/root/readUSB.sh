#!/bin/ash
#echo "Starting"
#tail -f /dev/ttyUSB0  &

/root/ReadArduinoSerial.sh < /dev/ttyUSB0 > /dev/null

