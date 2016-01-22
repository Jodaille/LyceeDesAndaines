/**
 * Tiny RTC module
 * module horloge
 * 
 * DS1307 pour bus I2C
 * avec batterie au lithium CR1225
 * 
 * Le port I2C de l'Arduino est situé
 * sur les pin A4 et A5
 * 
 * Analog pin A5 <-> SCL
 * Analog pin A4 <-> SDA
 */

#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

void setup () {
    Serial.begin(9600);
    Wire.begin();
    RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC ne fonctionne pas!");
    // la ligne suivante permet d'ajuster la date & l'heure
    // à la date de compilation du sketc
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}
void loop () {
    DateTime now = RTC.now(); 
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println(); 
    delay(1000);
}
