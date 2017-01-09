/**
 * Exemple basic de réception avec transmetteurs
 * de type RFM69
 * 
 * source:
 * Simple RFM12B wireless demo - Receiver - no ack
 * Glyn Hudson openenergymonitor.org GNU GPL V3 12/4/12
 * Credit to JCW from Jeelabs.org for RFM12 
 * 
 * voir aussi: 
 * https://www.mysensors.org/build/connect_radio#wiring-the-rfm69-radio
 */

#define RF69_COMPAT 1  // 1 pour RFM69CW ou 0 pour RFM12B
#include <JeeLib.h>

#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define I2C_ADDR    0x27  // Defini adrese I2C pour PCF8574T (ecran)
#define BACKLIGHT_PIN  3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7

#define  LED_OFF  1
#define  LED_ON  0

/*-----( Declare objects )-----*/  
LiquidCrystal_I2C  lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

#define myNodeID 30          // node ID du Rx (range 0-30) 
#define network     210      // groupe du reseau (peut être entre 1-250), le même que l'emeteur
/**
 * Frequences du RF12B/RFM69 peut être: 
 * RF12_433MHZ, RF12_868MHZ ou RF12_915MHZ.  
 * fonction de la frequence du module
*/
#define RF_freq RF12_868MHZ

// Structure contenant les données, doivent être identiques entre emetteur et recepteur
typedef struct { int power1, power2, power3, battery; } PayloadTX;
PayloadTX emontx;  

const int emonTx_NodeID=10; // Nodeid / identifiant de l'emetteur

void setup() {
  
  rf12_initialize(myNodeID,RF_freq,network);   //Initialise RFM69 avec parametres precedents  
  Serial.begin(57600); 
  Serial.println("RF12B demo Receiver - Simple demo"); 

  lcd.begin (16,2); // pour LCD de 16 caracteres x 2 lignes
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);

  lcd.home (); // place le curseur en 0,0 (premier caractere, premiere ligne)
  lcd.setCursor (3,0);
  lcd.print(myNodeID); // affiche le node id (ici: 30)

 Serial.print("Node: "); 
 Serial.print(myNodeID); 
 Serial.print(" Freq: "); 
 if (RF_freq == RF12_433MHZ) Serial.print("433Mhz");
 if (RF_freq == RF12_868MHZ) Serial.print("868Mhz");
 if (RF_freq == RF12_915MHZ) Serial.print("915Mhz");  
 Serial.print(" Network: "); 
 Serial.println(network);
}

void loop() {
  
 if (rf12_recvDone()){ // si reception
  if (rf12_crc == 0 && (rf12_hdr & RF12_HDR_CTL) == 0) {

    lcd.home (); // retour à 0 du LCD

    int node_id = (rf12_hdr & 0x1F); //extraction du nodeID depuis payload

    lcd.print("node:");
    lcd.print(node_id);// Affichage du nodeid de l'emetteur sur le LCD

    if (node_id == emonTx_NodeID)  {    //est-ce bien l'id de notre emetteur?
        emontx=*(PayloadTX*) rf12_data; // Extraction des données reçues de payload 
       Serial.print("power1: "); Serial.println(emontx.power1); 
       Serial.print("power2: "); Serial.println(emontx.power2); 
       Serial.print("power3: "); Serial.println(emontx.power3); 
       Serial.print("battery: "); Serial.println(emontx.battery);

       lcd.setCursor (0,1);       // on passe à la deuxieme ligne du LCD
       lcd.print(emontx.battery); // on affiche la valeur battery du struct
       Serial.println("  "); 
  }
 }
}
}
