/**
 * Send temperature humidity and voltage with 433Mhz RF
 * credits:
 * https://skyduino.wordpress.com/2011/12/29/tutoriel-arduino-et-emetteurrecepteur-433mhz-virtualwire/
 * http://jeelabs.org/
 */
#define DEBUG   0 // set to 1 to trace activity via serial console

/**
* Jeelabs stuff voltage + sleep
* @see http://jeelabs.org/2012/05/12/improved-vcc-measurement/
*/
//
#include <JeeLib.h>
#include <avr/sleep.h>

#include <VirtualWire.h> // inclusion de la librairie VirtualWire

volatile bool adcDone;
ISR(WDT_vect) { Sleepy::watchdogEvent(); }
ISR(ADC_vect) { adcDone = true; }
static byte vccRead (byte count =4) {
  set_sleep_mode(SLEEP_MODE_ADC);
  ADMUX = bit(REFS0) | 14; // use VCC and internal bandgap
  bitSet(ADCSRA, ADIE);
  while (count-- > 0) {
    adcDone = false;
    while (!adcDone)
      sleep_mode();
  }
  bitClear(ADCSRA, ADIE);
  // convert ADC readings to fit in one byte, i.e. 20 mV steps:
  //  1.0V = 0, 1.8V = 40, 3.3V = 115, 5.0V = 200, 6.0V = 250
  return (55U * 1023U) / (ADC + 1) - 50;
}

struct data_t {
    int temperature;
    int humidity;
    byte voltage;
    }; // user defined data structure
data_t data; // define a variable with that structure


/**
* DHT22 T°c & Humidity sensor
*/
#include <dht.h>
dht DHT;
#define DHT22_PIN 5

#include <dht11.h>
dht11 DHT11;
#define DHT11PIN 6

const char *msg = "Arduino"; // Tableau qui contient notre message

// emission sur pinout 12 (DATA)

const int led_pin = 13;



void setup() // Fonction setup()
{
    Serial.begin(115200); // Initialisation du port série pour avoir un retour sur le serial monitor
    Serial.println("Emission VirtualWire"); // Petit message de bienvenue
    Serial.flush();
    vw_setup(2000);     // initialisation de la librairie VirtualWire à 2000 bauds (note: je n'utilise pas la broche PTT)
}

void loop() // Fonction loop()
{

    byte x = vccRead();
    #if DEBUG
      Serial.print("V:");Serial.println(x);Serial.flush();
    #endif
    Sleepy::loseSomeTime(16);
    if(x > 115)
    {
        //int chk = DHT11.read(DHT11PIN);
        
        //int chk = DHT.read22(DHT22_PIN);
        int chk = DHT11.read(DHT11PIN);
        //Serial.print("check:");Serial.println(chk);Serial.flush();
        if( chk == 0 )
        {
            float temperature = DHT11.temperature;
            float humidity = DHT11.humidity;


            data.temperature = temperature*100;
            data.humidity= humidity*100;
            data.voltage = x;
            #if DEBUG
                Serial.println("1.0V = 0, 1.8V = 40, 3.3V = 115, 4.0V = 150 , 5.0V = 200, 6.0V = 250 ");
                Serial.flush();
                Serial.print("TX ... "); // On signale le début de l'envoi
                digitalWrite(led_pin, HIGH);
                Serial.flush();
            #endif
            vw_send((uint8_t *)&data, sizeof(data));// On envoie le message
            vw_wait_tx(); // On attend la fin de l'envoi

            #if DEBUG
                Serial.println("Done !"); // On signal la fin de l'envoi
                digitalWrite(led_pin, LOW);
                Serial.print("Voltage: ");
                Serial.print(x);
                Serial.print(" temperature: ");
                Serial.print(temperature);//Serial.print(" / ");Serial.print(data.temperature);
                Serial.print(" humidity: ");
                Serial.print(humidity);Serial.println("");
                Serial.flush();
            #endif
            // Et on attend 1s pour pas flooder
        }
    }
    Sleepy::loseSomeTime(1024*5);

}
