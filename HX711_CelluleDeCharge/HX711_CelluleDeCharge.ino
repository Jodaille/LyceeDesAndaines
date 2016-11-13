/*
 Cellule de charge et ampli hx711
cf: https://github.com/bogde/HX711.git
*/

#include "HX711.h"

HX711 scale;

void setup() {
  Serial.begin(38400);
  Serial.println("HX711 Demo");

  Serial.println("Initialisation de la balance");
  // parametre "gain" est ommis; la valeur par defaut de la librairie est 128
  // HX711.DOUT  - pin #A1
  // HX711.PD_SCK - pin #A0
  scale.begin(A1, A0);

  Serial.println("Avant reglage de la balance:");
  Serial.print("lecture: \t\t");
  Serial.println(scale.read());     // affiche la lecture brute du convetisseur (ADC)

  Serial.print("lecture moyenne de 20 lectures: \t\t");
  Serial.println(scale.read_average(20));   // print the average of 20 readings from the ADC

  Serial.print("affiche la moyenne de 5 lectures moins le poids de tare (pas encore defini): \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("moyenne de 5 lectures moins la tare divise par le parametre SCALE (balance): \t\t");
  Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
            // by the SCALE parameter (not set yet)

  scale.set_scale(2280.f);// valeur obtenue en calibrant la balance avec dees poids connus; voir le README
  scale.tare();           // remise à 0 de la balance

  Serial.println("Apres reglage de la balance:");

  Serial.print("lecture: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("lecture moyenne: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("valeur: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("valeur avec calibration: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
            // by the SCALE parameter set with set_scale

  Serial.println("Lectures:");
}

void loop() {
  Serial.print("une lecture:\t");
  Serial.print(scale.get_units(), 1);
  Serial.print("\t| moyenne:\t");
  Serial.println(scale.get_units(10), 1);

  scale.power_down();// mise en veille du convertisseur (ADC)
  delay(5000);
  scale.power_up();
}
