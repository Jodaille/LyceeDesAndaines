/**
 * This example demonstrates how to read analog signals
 * It assumes there are potentiometers connected
 * to the 16 channels of the 74HC4067 mux/demux
 *
 * For more about the interface of the library go to
 * https://github.com/pAIgn10/MUX74HC4067
 */

#include "MUX74HC4067.h"

// Creates a MUX74HC4067 instance
// 1st argument is the Arduino PIN to which the EN pin connects
// 2nd-5th arguments are the Arduino PINs to which the S0-S3 pins connect
MUX74HC4067 mux(7, 8, 9, 10, 11);

/**
 * nombre de portes (couloirs)
 */
#define nbPortes 8

/**
 * structure Porte
 * permet d'organiser les infos par porte
 */
struct Porte {
    int  pinsortie;
    int  pinentree;
    long tempsLecture;
} portes[nbPortes];

/**
 * initialisation tableau portes
 * une porte est composée de deux capteurs
 */
void initPortes(int nombreDePortes)
{
  int pinentree = 0; // entree debut à 0
  int pinsortie = 1; // sortie à 1
  for(int n=0; n<nombreDePortes; n++)
  {
    portes[n].pinentree    = pinentree;
    portes[n].pinsortie    = pinsortie;
    portes[n].tempsLecture = 0;
    /**
     *  nous ajoutons 2 pour faire suivre
     *  les pins entree sortie de chaque
     *  porte
     */
    pinentree = pinentree+2;
    pinsortie = pinsortie+2;
  }
}



void setup()
{
  initPortes(nbPortes);


  Serial.begin(115200);  // Initializes serial port

  /**
   * verification de la structure
   */
  for(int n=0; n<nbPortes; n++)
  {
    affichePortes(portes, n);
  }
    delay(1000);


  // Configures how the SIG pin will be interfaced
  // e.g. The SIG pin connects to PIN A0 on the Arduino,
  //      and PIN A0 is a analog input
  mux.signalPin(A0, INPUT, ANALOG);
}


void loop()
{
  int valeurEntree;
  int valeurSortie;
  /**
   * lecture des valeurs des portes
   */
  for (int i = 0; i < nbPortes; ++i)
  {
    // Nous conservons le temps de la precedente lecture
    long lecturePrecedente = portes[i].tempsLecture;

    // Nous conservons les millis de la lecture
    long temps = millis();
    portes[i].tempsLecture = temps;

    /**
     * nous recuperons les pins correpondants
     * aux capteurs d'entree et de sortie
     */
    int pinEntree = portes[i].pinentree;
    int pinSortie = portes[i].pinsortie;

    /**
     * Nous lisons la valeur du capteur
     */
    valeurEntree   = mux.read(pinEntree);
    valeurSortie   = mux.read(pinSortie);
    long lectureMs = portes[i].tempsLecture;

    // Nous affichons le resultat
    Serial.print("Porte:");
    Serial.print(i);
    Serial.print(" entree:");
    Serial.print(valeurEntree);
    Serial.print(" sortie:");
    Serial.print(valeurSortie);
    Serial.print(" ms:");
    Serial.print(lectureMs);
    Serial.println();
  }
  Serial.println();

  delay(500);
}

/**
 * affiche les pins des capteurs d'entree et sortie
 * en fonction du numero de porte (commence à 0)
 */
void affichePortes(struct Porte *lesPortes, int numPorte)
{
  int pinEntree = lesPortes[numPorte].pinentree;
  int pinSortie = lesPortes[numPorte].pinsortie;
  Serial.print("porte: ");
  Serial.print(numPorte);
  Serial.print(" entree: ");
  Serial.print(pinEntree);
  Serial.print(" sortie: ");
  Serial.println(pinSortie);

}
