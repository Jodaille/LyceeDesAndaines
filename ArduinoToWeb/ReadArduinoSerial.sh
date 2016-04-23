#!/bin/ash
#
# /bin/ash is a symlink to /bin/busybox
# in order to "recreate" OpenWRT environment
#
# Notes:
# to know which shell is in use:
# readlink -f $(which sh)
#
# script is launched as follow:
# /home/jody/LyceeDesAndaines/ArduinoToLinux/ReadArduinoSerial.sh < /dev/ttyUSB2
# where /dev/ttyUSB2 is Arduino serial port

# Replace semicolumn character to &
replaceSemicolumn()
{
    local searchstring=';'
    local replacestring='&'
    local line=$1
    local query=/a.php?${line//$searchstring/$replacestring}
    echo $query
}

# Check that we have id in string
hasIdString()
{
    case $1 in
    *id*) return 0 ;;
    *) return 1 ;;
    esac
}

while read line; do
    if [ ! -z "$line" ]; then # we do not want empty line
        if hasIdString $line ; then
            query=$(replaceSemicolumn $line)

            printf "GET $query HTTP/1.0\r\n\r\n"  |nc  jodaille.org 80

        fi
    fi
done

# cf: https://forum.openwrt.org/viewtopic.php?pid=223959#p223959
# command to query a GET HTTP request with OpenWRT
# printf "GET /a.php?id=nano&t=12&h=22&l=123 HTTP/1.0\r\n\r\n" |nc  jodaille.org 80
