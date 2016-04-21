#!/bin/sh

searchstring=";"
replacestring="#"
while read -r line; do
    echo ${line//$searchstring/$replacestring}
    #${var//;/\&}
done < /dev/stdin

query="/a.php?$var"
# http://stackoverflow.com/questions/918886/how-do-i-split-a-string-on-a-delimiter-in-bash#answer-15988793


#echo "You entered '$query'"
printf "GET $query HTTP/1.0\r\n\r\n"



# https://forum.openwrt.org/viewtopic.php?pid=223959#p223959
# printf "GET /a.php?id=nano&t=12&h=22&l=123 HTTP/1.0\r\n\r\n" |nc  jodaille.org 80
