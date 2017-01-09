/**
 * Exemple basic de transimission avec transmetteurs
 * de type RFM69
 * 
 * source:
 * Simple RFM12B wireless demo - Receiver - no ack
 * Glyn Hudson openenergymonitor.org GNU GPL V3 7/7/11
 * Credit to JCW from Jeelabs.org for RFM12 
 * 
 * voir aussi: 
 * https://www.mysensors.org/build/connect_radio#wiring-the-rfm69-radio
 */

#define RF69_COMPAT 1  // 1 pour RFM69CW ou 0 pour RFM12B
#include <JeeLib.h>    //from jeelabs.org

#define myNodeID 10          //node ID du transmetteur (range 0-30) different du recepteur
#define network     210      //groupe reseau (peut être entre 1-250).le même que le recepteur
/**
 * Frequences du RF12B/RFM69 peut être: 
 * RF12_433MHZ, RF12_868MHZ ou RF12_915MHZ.  
 * fonction de la frequence du module
*/
#define RF_freq RF12_868MHZ


// Structure contenant les données, doivent être identiques entre emetteur et recepteur
typedef struct { int power1, power2, power3, battery; } PayloadTX;
PayloadTX emontx;  

const int LED = 8;                                             //emonTx V3
void setup() {
  rf12_initialize(myNodeID,RF_freq,network);   //Initialise RFM69 avec parametres precedents  
Serial.begin(57600);
Serial.println("RFM12B Transmitter - Simple demo");

 Serial.print("Node: "); 
 Serial.print(myNodeID); 
 Serial.print(" Freq: "); 
 if (RF_freq == RF12_433MHZ) Serial.print("433Mhz");
 if (RF_freq == RF12_868MHZ) Serial.print("868Mhz");
 if (RF_freq == RF12_915MHZ) Serial.print("915Mhz"); 
 Serial.print(" Network: "); 
 Serial.println(network);

pinMode(LED, OUTPUT);

}

void loop() {
  // on incremente histoire d'avoir des valeurs qui changent et verifier la transmission
  emontx.power1=emontx.power1+1;
  emontx.power2=emontx.power2+2;
  emontx.power3=emontx.power3+3;
  emontx.battery=emontx.battery+4;
    
    rf12_sendNow(0, &emontx, sizeof emontx); // on envoi la structure emontx
    rf12_sendWait(2);
    
  Serial.print("power1: "); Serial.println(emontx.power1); 
  Serial.print("power2: "); Serial.println(emontx.power2); 
  Serial.print("power3: "); Serial.println(emontx.power3); 
  Serial.print("battery: "); Serial.println(emontx.battery); 
  Serial.println("  "); 
  
  digitalWrite(LED, HIGH); delay(500); digitalWrite(LED, LOW);   
  delay(2000);
}
