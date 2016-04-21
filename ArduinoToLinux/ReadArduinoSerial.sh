#!/home/jody/LyceeDesAndaines/ash
# ash is a symlink to /bin/busybox
# in order to "recreate" OpenWRT environment
# Notes:
# to know which shell is in use:
# readlink -f $(which sh)
#
# script is launched as follow:
# /home/jody/LyceeDesAndaines/ArduinoToLinux/ReadArduinoSerial.sh < /dev/ttyUSB2


searchstring=';'
replacestring='&'

while read line; do
    if [ ! -z "$line" ]; then # we do not want empty line
        query=/a.php?${line//$searchstring/$replacestring} # test HTTP GET query
        printf "GET $query HTTP/1.0\r\n\r\n"
    fi
done

# https://forum.openwrt.org/viewtopic.php?pid=223959#p223959
# printf "GET /a.php?id=nano&t=12&h=22&l=123 HTTP/1.0\r\n\r\n" |nc  jodaille.org 80
