/**
 * credits:
 * https://skyduino.wordpress.com/2011/12/29/tutoriel-arduino-et-emetteurrecepteur-433mhz-virtualwire/
 */
/**
* DHT22 T°c & Humidity sensor
*/
#include <dht.h>
dht DHT;
#define DHT22_PIN 5


struct data_t {int temperature; int humidity;}; // user defined data structure
data_t data; // define a variable with that structure

#include <VirtualWire.h> // inclusion de la librairie VirtualWire

const char *msg = "Arduino"; // Tableau qui contient notre message

// emission sur pinout 12 (DATA)

const int led_pin = 13;

void setup() // Fonction setup()
{
    Serial.begin(115200); // Initialisation du port série pour avoir un retour sur le serial monitor
    Serial.println("Emission VirtualWire"); // Petit message de bienvenue

    vw_setup(2000);     // initialisation de la librairie VirtualWire à 2000 bauds (note: je n'utilise pas la broche PTT)
}

void loop() // Fonction loop()
{

    int chk = DHT.read22(DHT22_PIN);
    if( chk == DHTLIB_OK )
    {
        float temperature = DHT.temperature;
        float humidity = DHT.humidity;

        data.temperature = temperature*100;
        data.humidity= humidity*100;

        Serial.print("TX ... "); // On signale le début de l'envoi
        digitalWrite(led_pin, HIGH);
        vw_send((uint8_t *)&data, sizeof(data));// On envoie le message
        vw_wait_tx(); // On attend la fin de l'envoi
        Serial.println("Done !"); // On signal la fin de l'envoi
        digitalWrite(led_pin, LOW);

        Serial.print(" temperature: ");
        Serial.print(temperature);Serial.print(" / ");Serial.print(data.temperature);
        Serial.print(" humidity: ");
        Serial.print(humidity);Serial.println("");
        delay(2000); // Et on attend 1s pour pas flooder
    }

}
