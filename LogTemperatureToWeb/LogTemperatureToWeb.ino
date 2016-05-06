/**
 * Envoi d'une température via le port série
 * OpenWRT en lecture et envoi sur le web
 * 
 */

/**
 * Définition du timer
 * permet d'éviter l'emploi de delay()
 * qui provoque souvent un blocage
 * du Arduino au bout d'un certain laps de temps
 */
unsigned long previousMillis = 0;
const long interval = 1000;

/** 
 *  Nous utilisons la librairie OneWire
 *  Elle doit être présente dans le répertoire libraries
 *  situé dans le répertoire des croquis/sketchs
 *  voir dans le menu Préférences
 *  cf: https://github.com/PaulStoffregen/OneWire/archive/master.zip
 */
#include <OneWire.h>

/**
 * Nous utilisons aussi la librairie DallasTemperature
 * cf: https://github.com/milesburton/Arduino-Temperature-Control-Library
 */
#include <DallasTemperature.h>
 
/* 
 *  Le fil des données (jaune?) est relié au pin 2 (D2) du Arduino
 *  une resistance de 4,7kOhm est placée en 5V et D2
 */

#define ONE_WIRE_BUS 2

/** 
 * Création d'une instance oneWire afin de pouvoir 
 * communiquer avec des périphériques OneWire
 * Nb: pas seulement des capteurs Maxim/Dallas
 */ 
OneWire oneWire(ONE_WIRE_BUS);
 
/**
 * Nous passons notre instance oneWire à notre 
 * objet sensors utilisant DallasTemperature
 */
DallasTemperature sensors(&oneWire);

/**
 * Adresses du capteur, cf: https://git.io/vwM2x
 * pour scanner l'adresse de votre capteur
 */
//DeviceAddress temperature1 = { 0x28, 0xEF, 0x40, 0x1E, 0x07, 0x00, 0x00, 0xAD };
DeviceAddress temperature1 = { 0x28, 0x80, 0xA0, 0x2A, 0x07, 0x00, 0x00, 0xD6 };
DeviceAddress temperature2 = { 0x28, 0x62, 0xB9, 0x29, 0x07, 0x00, 0x00, 0x96 };


/**
 * Écriture sur une carte SD
 * 
 * SD card reliée au bus SPI :
 * MOSI       - pin 11 
 * MISO       - pin 12
 * CLK ou SCK - pin 13
 * CS         - pin 4
 *
 * SPI pour Serial Peripheral Interface
 * 
 * created  24 Nov 2010
 * modified 9 Apr 2012
 * by Tom Igoe
 * cf: https://www.arduino.cc/en/Tutorial/Datalogger
 */
#include <SPI.h>
#include <SD.h>

// Arduino Uno pin 4
// cf: https://www.arduino.cc/en/Reference/SPI
const int chipSelect = 4;

bool sDisReady = false;

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
/**
 * Nous utilisons la librairie RTClib
 * cf: https://github.com/adafruit/RTClib/archive/master.zip
 * à installer dans le répertoire libraries
 * du répertoire de sketchs
 */
#include "RTClib.h"
RTC_DS1307 RTC;

// pour ajuster l'heure changer en : true
bool definirHeure = false;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print(__FILE__ " " __DATE__ " " __TIME__);
  Serial.println("  IDE "); Serial.println(ARDUINO);

  if(SD.begin(chipSelect))
  {
    sDisReady = true;
  }

  // Initialisation de notre librairie
  sensors.begin();  

  Wire.begin();
  RTC.begin();

  if(definirHeure)
  {
      // la ligne suivante permet d'ajuster la date & l'heure
      // à la date de compilation du sketc
      RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}

void loop() {

  // boucle principale

  // nous conservons la valeur du nombre de ms
  unsigned long currentMillis = millis();

  // nous comparons avec la lecture précédente
 if(currentMillis - previousMillis >= interval) {

    // save the last time you blinked the LED 
    previousMillis = currentMillis;
    
    String tolog = builString();
    Serial.println(tolog);
    if (sDisReady) {
      /**
       * nous ouvrons le fichier
       * Nb: un seul fichier peut être ouvert à la fois
       * le fichier se nomme : journal.csv
       */
      File dataFile = SD.open("journal.csv", FILE_WRITE);
      // si le fichier est disponnible, nous écrivons dedans :
      if (dataFile) {
        dataFile.println(tolog);
        dataFile.close();
      }
    }
 }

}

String builString()
{
  String dataString = "id=rucheA;";

  dataString += "time=";
  dataString += buildTime();
  dataString += ";";

  sensors.requestTemperatures(); // envoi de la demande
   /**
   * temperature1 est un tableau contenant l'adresse
   * de notre capteur.
   * cf: https://git.io/vwM2x pour scanner les adresses
   */
  dataString += "t1=";
  dataString += String(sensors.getTempC(temperature1));
  dataString += ";";

  dataString += "t2=";
  dataString += String(sensors.getTempC(temperature2));
  dataString += ";";

  dataString += "pression=";
  dataString += String(pression());
  dataString += ";";

  dataString += "A1=";
  dataString += analogRead(A1);
  dataString += ";";

  dataString += "A2=";
  dataString += analogRead(A2);
  dataString += ";";

  dataString += "milli=";
  dataString += String(millis());
  dataString += ";";
  return dataString;
}

/**
 * build a string with time
 */
String buildTime()
{
  DateTime now = RTC.now();
  String dateString = String(now.year());

  dateString += "-";
  int month = now.month();
  if(month<10)
  {
    dateString += "0";
  }
  dateString += month;

  dateString += "-";
  int day = now.day();
  if(day<10)
  {
    dateString += "0";
  }
  dateString += day;

  dateString += " ";
  int hour = now.hour();
  if(hour<10)
  {
    dateString += "0";
  }
  dateString += hour;

  dateString += ":";
  int minute = now.minute();
  if(minute<10)
  {
    dateString += "0";
  }
  dateString += minute;

  dateString += ":";
  int second = now.second();
  if(second<10)
  {
    dateString += "0";
  }
  dateString += second;
 
  return dateString;
}

float pression()
{
  int val = analogRead(A0);
  float tension = ((val-10)*0.0049);
  // Calcul de la pression reelle
  int pression_val = (100.1-(((tension/5)-0.04) / 0.009))*10;
  return pression_val; 
}
