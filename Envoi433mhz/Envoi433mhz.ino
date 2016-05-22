/**
 * credits:
 * https://skyduino.wordpress.com/2011/12/29/tutoriel-arduino-et-emetteurrecepteur-433mhz-virtualwire/
 */

#include <VirtualWire.h> // inclusion de la librairie VirtualWire

const char *msg = "Arduino"; // Tableau qui contient notre message

// emission sur pinout 12 (DATA)

const int led_pin = 13;

void setup() // Fonction setup()
{
    Serial.begin(115200); // Initialisation du port série pour avoir un retour sur le serial monitor
    Serial.println("Tuto VirtualWire"); // Petit message de bienvenue

    vw_setup(2000);     // initialisation de la librairie VirtualWire à 2000 bauds (note: je n'utilise pas la broche PTT)
}

void loop() // Fonction loop()
{
    Serial.print("TX ... "); // On signale le début de l'envoi
    digitalWrite(led_pin, HIGH);
    vw_send((uint8_t *)msg, strlen(msg)); // On envoie le message
    vw_wait_tx(); // On attend la fin de l'envoi
    Serial.println("Done !"); // On signal la fin de l'envoi
    digitalWrite(led_pin, LOW);
    delay(1000); // Et on attend 1s pour pas flooder
}
