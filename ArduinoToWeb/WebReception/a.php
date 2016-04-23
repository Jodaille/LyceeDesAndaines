<?php
/**
* Sample of script to receive datas from OpenWRT router
*
*/
$arduino_id = null;
$g = $_GET;
if(isset($g['id']))
{
    $arduino_id = $g['id'];
}
else
{
    header('HTTP/1.0 400 Bad Request', true, 400);
    die();
}
/**
Arduino sketch send a string with this parameters:
Serial.print("id=");
Serial.print("pression=");
Serial.print("A1=");
Serial.print("A2=");
Serial.print("milli=");
cf: LogTestToWeb.ino
*/

$pression = $g['pression'];
$analog1  = $g['A1'];
$analog2  = $g['A2'];
$milli    = $g['milli'];
$time     = date('Y-m-d H:i:s');

$log = "$time $arduino_id $pression $analog1 $analog1 $milli\n";
file_put_contents('/tmp/arduino.log', $log, FILE_APPEND);

echo "$log";die();
