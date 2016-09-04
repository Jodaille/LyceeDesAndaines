/**
 * Lecture de température avec trois
 * capteurs DS18B20 par leur adresse
 */

#include <JeeLib.h>
#include <avr/sleep.h>

volatile bool adcDone;
ISR(WDT_vect) { Sleepy::watchdogEvent(); }
ISR(ADC_vect) { adcDone = true; }

#define SLEEP_DURATION 1000 * 10

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
 adresse :	0x28, 0x42, 0x6B, 0x1C, 0x07, 0x00, 0x00, 0x34
adresse :	0x28, 0xED, 0x0B, 0x2A, 0x07, 0x00, 0x00, 0x1C
adresse :	0x28, 0xF7, 0x9D, 0x29, 0x07, 0x00, 0x00, 0x2B
 */
DeviceAddress temperature1 = { 0x28, 0x42, 0x6B, 0x1C, 0x07, 0x00, 0x00, 0x34 };
DeviceAddress temperature2 = { 0x28, 0xED, 0x0B, 0x2A, 0x07, 0x00, 0x00, 0x1C };
DeviceAddress temperature3 = { 0x28, 0xF7, 0x9D, 0x29, 0x07, 0x00, 0x00, 0x2B };

void setup(void)
{
  // Initialisation du port série
  Serial.begin(9600);

  // Initialisation de notre librairie
  sensors.begin();
}


void loop(void)
{

    String tolog = builString();
    Serial.println(tolog);
    Serial.flush();delay(5);
    Sleepy::loseSomeTime(SLEEP_DURATION);

}

String builString()
{
  String dataString = "id=Danvou;";


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

  dataString += "t3=";
  dataString += String(sensors.getTempC(temperature3));
  dataString += ";";

  dataString += "milli=";
  dataString += String(millis());
  dataString += ";";
  return dataString;
}
