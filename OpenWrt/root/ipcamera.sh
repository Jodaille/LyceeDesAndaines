#!/bin/sh
#

URL="http://192.168.0.19"

SNAP="/snapshot.cgi?loginuse=admin&loginpas=&"

CONTROLURL="/decoder_control.cgi?loginuse=admin&loginpas=&"

BASEURL="$URL$CONTROLURL"
SNAPURL="$URL$SNAP"


UP="command=0"
STOPUP="command=1"
DOWN="command=2"
STOPDOWN="command=3"

PRESET1="command=31"
PRESET2="command=32"

# Move caemra down
#wget -q  $BASEURL$DOWN > /dev/null
#sleep 1
#wget -q $BASEURL$STOPDOWN > /dev/null
#sleep 1

# Move camera to preset position 1
#wget -q $BASEURL$PRESET1 > /dev/null
#sleep 1


#echo $SNAPURL
NOW=$(/bin/date +"%Y-%m-%d-%H%M%S")

wget $SNAPURL -O /tmp/snapshot.jpg


echo date is: $NOW

scp -i /etc/dropbear/dropbear_rsa_host_key /tmp/snapshot.jpg jobee@jodaille.org:/home/jobee/snapshots/$NOW.jpg


#http://192.168.0.19/decoder_control.cgi?loginuse=admin&loginpas=&command=2
# http://192.168.0.19/decoder_control.cgi?loginuse=admin&loginpas=&command=31&onestep=0&sit=31&14724919849090.09144154703244567&_=1472491984917
