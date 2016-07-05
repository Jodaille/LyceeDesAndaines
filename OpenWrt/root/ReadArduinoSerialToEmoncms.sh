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

# @param searchstring
# @param replacestring
# @param line
replaceStringInString()
{
    local searchstring=$1
    local replacestring=$2
    local line=$3
    local replaced=${line//$searchstring/$replacestring}
    echo $replaced
}

# Check that we have id in string
hasIdString()
{
    case $1 in
    *id*) return 0 ;;
    *) return 1 ;;
    esac
}

getId()
{
    local line=$1
    local start=${line##*id=}
    #echo start $start
    local id=${start%%;*}
    echo $id
    #echo $1 | sed -e 's/id=\(.*\);/\1/'
}
# localhost/emoncms/input/post.json?node=1&json={t1:23.4,h1:60}&apikey=ae9c5d7b8939562f74523b04a3958035
loopOnLineSeparators()
{
    local url="/emoncms/input/post.json?"
    local apikey="apikey=ae9c5d7b8939562f74523b04a3958035&"
    local query="$url$apikey"
    local var=$1
    local json="json={"
    while [ "$var" ] ;do
        iter=${var%%;*}
        if hasIdString $iter; then
            local node=$(replaceStringInString 'id=rucheBas' 'node=1' $iter)
            query="$query&$node&$json"
            #echo $query
        else
            local jsonValue=$(replaceStringInString "=" ":" $iter)
            query="$query$jsonValue"
            query="$query,"

        fi


        [ "$var" = "$iter" ] && \
            var='' || \
            var="${var#*;}"
    done

    query=${query%*,}
    #query="$query}"
    local finAccolade="}"
    echo "$query$finAccolade" | tr -d '\r'
}

# line="id=rucheBas;V=153;t1=22.00;h=41.00"
# query=$(loopOnLineSeparators $line)


while read line; do
    if [ ! -z "$line" ]; then # we do not want empty line
        if hasIdString $line ; then
            query=""
#             query=$(replaceSemicolumn $line)
            query=$(loopOnLineSeparators $line)
            printf "$query\n"

            printf "GET $query HTTP/1.0\r\n\r\n"  |nc  192.168.0.11 80

        fi
    fi
done

# cf: https://forum.openwrt.org/viewtopic.php?pid=223959#p223959
# command to query a GET HTTP request with OpenWRT
# printf "GET /a.php?id=nano&t=12&h=22&l=123 HTTP/1.0\r\n\r\n" |nc  jodaille.org 80
