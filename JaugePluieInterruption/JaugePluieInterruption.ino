/*
 * Compteur hauteur d'eau tombée
 * Source du script
  http://cactus.io/projects/weather/arduino-weather-station-rain-sensor

  le câblage consiste à relier un fil sur la masse (GND) l'autre
  sur une pin ayant une fonction d'interruption (d2 ou d3) ici RG11_Pin
  cf: https://www.arduino.cc/en/Reference/AttachInterrupt
*/

/*
 volume du réservoir
 d'après http://atelier-meteo-arduino-lycee-vincendo.blogspot.fr/p/le-pluviometre.html
 il est de 0.2794mm
 bucket size to trigger tip (count was 0.01)
*/
#define Bucket_Size 0.2794
#define RG11_Pin 3 // digital pin RG11 connected to

/*
 bucket tip counter used in interrupt routine
 comteur de bascule du reservoir
*/
volatile unsigned long tipCount;

/*
 minuteur de gestion du rebond de l'interrupteur
 Timer to manage any contact bounce in interrupt routine
*/
volatile unsigned long contactTime;

/*
 hauteur totale détectée en mm/m2
 total amount of rainfall detected
*/
volatile float totalRainfall;


void setup() {
    Serial.begin(9600);
    // initialisation des variables
    tipCount = 0;      // nombre de basule(s)
    totalRainfall = 0; // hauteur d'eau totale mm/m2

    // utilisation de la resistance du pin RG11_Pin
    pinMode(RG11_Pin, INPUT_PULLUP); 

    // attache sur la perte du signal du bouton
    attachInterrupt(digitalPinToInterrupt(RG11_Pin), isr_rg, FALLING);
    sei();// Enable Interrupts / activation de l'interruption
}

void loop() {

    Serial.print(totalRainfall); Serial.println(" mm");

    // Add a 2 second delay / delis de 2s
    delay(2000); //just here to slow down the output. / ralentir l'affichage

}

// Interrupt handler routine that is triggered when the rg-11 detects rain
// prise en charge de l'interruption déclanchée par la bacule
// la boucle est interrompue par la bacule
void isr_rg() {
    if((millis() - contactTime) > 15 ) { // debounce of sensor signal / anti-rebond
        tipCount++;
        totalRainfall = tipCount * Bucket_Size;
        contactTime = millis();
    }
}
// end of rg-11 rain detection interrupt handler
