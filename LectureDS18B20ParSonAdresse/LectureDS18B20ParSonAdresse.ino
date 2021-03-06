/**
 * Lecture de température avec un seul
 * capteur DS18B20 (le premier)
 */

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

void setup(void)
{
  // Initialisation du port série
  Serial.begin(9600);
  Serial.println("Demo de la librairie Dallas Temperature");

  // Initialisation de notre librairie
  sensors.begin();  
}


void loop(void)
{
  /**
   * Nous interrogeons notre capteur
   */
  sensors.requestTemperatures(); // envoi de la demande


  Serial.print("La temperature du premier capteur est : ");
  /**
   * temperature1 est un tableau contenant l'adresse
   * de notre capteur.
   * cf: https://git.io/vwM2x pour scanner les adresses
   */
  Serial.println(sensors.getTempC(temperature1));


}
