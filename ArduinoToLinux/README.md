# Connect Arduino to the Web with OpenWRT

## Requirements

- [x] Arduino Uno, Nano ...
- [x] OpenWRT Router [TL-WR703N Mod 64Mb](http://www.ebay.com/itm/181078954797)
- [x] a Webserver or a service

## Arduino talks to the Web

Arduino is linked to router by USB cable.

Read datas are written to serial (USB),
we use a [script](https://github.com/Jodaille/LyceeDesAndaines/blob/master/ArduinoToLinux/ReadArduinoSerial.sh) to read serial
port, do some string management, then play an HTTP GET query to a simple PHP script page on a remote server.

The script is launched on the router in a screen (GNU/Screen). So that process should not die when we kill current terminal session.

## Arduino test sketch

[LogTestToWeb.ino](https://github.com/Jodaille/LyceeDesAndaines/blob/master/ArduinoToLinux/LogTestToWeb/LogTestToWeb.ino) is a simple sketch that read then print some values every "interval" seconds (no delay) on the serial.

Each values are semicolumn separated.

## SHell script to read serial and push to the Web

[ReadArduinoSerial.sh](https://github.com/Jodaille/LyceeDesAndaines/blob/master/ArduinoToLinux/ReadArduinoSerial.sh) is launched on device /dev/ttyUSB0 (Nano)

## OpenWRT

Kernel modules to install, example:

    opkg update
    opkg install kmod-usb-serial-ch341

Modules for Arduino Nano with CH341 USB serial chip:

    kmod-usb-serial
    kmod-usb-serial-ch341
    kmod-usb-serial-cp210x

Tool to install:

    opkg install screen


[readUSB.sh](https://github.com/Jodaille/LyceeDesAndaines/blob/master/ArduinoToLinux/readUSB.sh) is a simple script that send output of serial port (/dev/ttyUSB0) to [ReadArduinoSerial.sh](https://github.com/Jodaille/LyceeDesAndaines/blob/master/ArduinoToLinux/ReadArduinoSerial.sh) for processing.


With screen you can have this process forever, that means it will not ended after your logout from terminal.

    screen -Sdm monitImport -s  /readUSB.sh


Next step will be to be able to start after the boot (crontab ?) and maybe to check that process exist unless relaunch it.

It could be nice if readUSB.sh could be detect the Arduino port.
