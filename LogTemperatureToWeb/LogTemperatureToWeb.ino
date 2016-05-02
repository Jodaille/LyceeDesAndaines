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
DeviceAddress temperature1 = { 0x28, 0xEF, 0x40, 0x1E, 0x07, 0x00, 0x00, 0xAD };

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print(__FILE__ " " __DATE__ " " __TIME__);
  Serial.println("  IDE "); Serial.println(ARDUINO);

  // Initialisation de notre librairie
  sensors.begin();  

}

void loop() {

  // boucle principale

  // nous conservons la valeur du nombre de ms
  unsigned long currentMillis = millis();

  // nous comparons avec la lecture précédente
 if(currentMillis - previousMillis >= interval) {

    // save the last time you blinked the LED 
    previousMillis = currentMillis;

    Serial.print("id=");
    Serial.print("ruche1");
    Serial.print(";");

    /**
    * Nous interrogeons notre capteur
    */
    sensors.requestTemperatures(); // envoi de la demande

   /**
   * temperature1 est un tableau contenant l'adresse
   * de notre capteur.
   * cf: https://git.io/vwM2x pour scanner les adresses
   */
    Serial.print("t1=");
    Serial.print(sensors.getTempC(temperature1));
    Serial.print(";");

    Serial.print("pression=");
    Serial.print(pression());
    Serial.print(";");

    Serial.print("A1=");
    Serial.print(analogRead(A1));
    Serial.print(";");

    Serial.print("A2=");
    Serial.print(analogRead(A2));
    Serial.print(";");

    Serial.print("milli=");
    Serial.print(millis());
    Serial.println();
 }

}

float pression()
{
  int val = analogRead(A0);
  float tension = ((val-10)*0.0049);
  // Calcul de la pression reelle
  int pression_val = (100.1-(((tension/5)-0.04) / 0.009))*10;
  return pression_val; 
}
