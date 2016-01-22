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

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Initialisation de la carte SD ...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Carte Sd inaccesible ou absente");
    // don't do anything more:
    return;
  }
  Serial.println("Carte OK");
}

void loop() {
  // nous créons une chaîne de caractères pour
  // concaténer les données à écrire :
  String dataString = "";

  // nous convertissons la valeur de millis()
  // avec l'objet String() afin de pouvoir 
  // l'écrire sur la carte
  dataString += String(millis());
  dataString += ";";

  // lecture de trois capteurs reliés de A0 à A2:
  for (int analogPin = 0; analogPin < 3; analogPin++) {

    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      // Nous ajoutons un caractère de séparation
      dataString += ";";
    }
  }

  /**
   * nous ouvrons le fichier
   * Nb: un seul fichier peut être ouvert à la fois
   * le fichier se nomme : journal.csv
   */
  File dataFile = SD.open("journal.csv", FILE_WRITE);

  // si le fichier est disponnible, nous écrivons dedans :
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();

    // nous affichons aussi notre chaîne sur le port série :
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  // Si le fichier n'est pas ouvert nous affichons
  // un message d'erreur
  else {
    Serial.println("nous ne pouvons ouvrir journal.csv");

    delay(5000);

  }
}
