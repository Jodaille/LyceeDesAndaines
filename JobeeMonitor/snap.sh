#!/bin/bash

IPCAMERA="192.168.1.13"
ARDUINOLOG="/home/jobee/screen.log"
IMAGEPATH="/home/jobee/snapshots"
TMPIMAGENAME="/snap.jpg"
TMPIMGPATH="$IMAGEPATH$TMPIMAGENAME"
IMAGENAME=`date +%Y-%m-%d-%H%M%S`
CROPIMGPATH="$IMAGEPATH/$IMAGENAME.jpg"
THRESHOLD=50

#echo $IMAGENAME

LUX=`tail -1 $ARDUINOLOG | awk -F ";" '{print $2}' | awk -F "=" '{print $2}'`

#echo Lux: $LUX THRESHOLD: $THRESHOLD


if (( $(echo "$THRESHOLD $LUX" | awk '{print ($1 < $2)}') )); 
then
	#echo "Lux: $LUX <  $THRESHOLD"
	#echo TMPIMGPATH $TMPIMGPATH
	wget -q http://admin:admin@$IPCAMERA/tmpfs/snap.jpg -O $TMPIMGPATH
	convert $TMPIMGPATH -crop 800x700+310+280 "$CROPIMGPATH"
	scp "$CROPIMGPATH" "jodaille.org:/var/www/wiki/snapshots/$IMAGENAME.jpg"
fi

