//int pinLed = 13; //led de arduino
int entrees =0; //pour mettre les compteurs à 0 au démarrage
int sorties =0;

//variable pour compter les passages vrai devant A près de la ruche - B près de la sortie
int passageA =0;
int passageB =0;

//variable pour compter les erreurs abeilles qui stationnent ou qui se suivent
int erreur =0;

//variable pour compter  1par boucle quand A/B est niveau bas
int countA = 0;
int countB = 0;

//chrono A et chronoB sont 2 variables longues pour stocker les temps de passage devant les détecteurs A et B
unsigned long chronoA=0;
unsigned long chronoB=0;

/**
 * Tiny RTC module
 * module horloge
 *
 * DS1307 pour bus I2C
 * avec batterie au lithium CR1225
 *
 * Le port I2C de l'Arduino est situé
 * sur les pin A4 et A5
 *
 * Analog pin A5 <-> SCL
 * Analog pin A4 <-> SDA
 */

#include <Wire.h>
/**
 * Nous utilisons la librairie RTClib
 * cf: https://github.com/adafruit/RTClib/archive/master.zip
 * à installer dans le répertoire libraries
 * du répertoire de sketchs
 */
#include "RTClib.h"
RTC_DS1307 RTC;
// pour ajuster l'heure changer false en : true
bool definirHeure = true;

//branchement des sorties du multiplexeur qui permettent de récupérer les données des 16 entrées (4X4)
int s0 = 6;
int s1 = 7;
int s2 = 8;
int s3 = 9;

int SIG_pin = 10; //connexion du multiplexeur à l'arduino

#define NBPORTES 3  // à adapter au nombre de portes connectées, jusqu'à 8
#define NBCAPTEURS 2 // 2 capteurs par porte

#define nbPortes 8
/**
 * structure Porte
 * permet d'organiser les infos par porte
 */
struct Porte {
    int  pinsortie;
    int  pinentree;
    long tempsLectureEntree;
    long tempsLectureSortie;
    int  lectureEntree;
    int  lectureSortie;
    int  compteurEntree;
    int  compteurSortie;
} portes[nbPortes];




void setup() {
// initialisation porte 0
portes[0].pinentree = 0;
portes[0].pinsortie = 1;
portes[0].tempsLectureEntree = 0;
portes[0].tempsLectureSortie = 0;
portes[0].lectureEntree = 1; // 1 quand rien ne coupe le faisceau
portes[0].lectureSortie = 1;
portes[0].compteurEntree = 0;
portes[0].compteurSortie = 0;

// initialisation porte 1
portes[1].pinentree = 2;
portes[1].pinsortie = 3;
portes[1].tempsLectureEntree = 0;
portes[1].tempsLectureSortie = 0;
portes[1].lectureEntree = 1;
portes[1].lectureSortie = 1;
portes[1].compteurEntree = 0;
portes[1].compteurSortie = 0;

// initialisation porte 2
portes[2].pinentree = 4;
portes[2].pinsortie = 5;
portes[2].tempsLectureEntree = 0;
portes[2].tempsLectureSortie = 0;
portes[2].lectureEntree = 1;
portes[2].lectureSortie = 1;
portes[2].compteurEntree = 0;
portes[2].compteurSortie = 0;

  // put your setup code here, to run once:
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);

  Serial.begin(9600);
  Serial.println("Start");

   //zone horloge
    Wire.begin();
    RTC.begin();
  if (! RTC.isrunning())
  {
    Serial.println("RTC ne fonctionne pas!");
  }

  if(definirHeure)
  {
      // la ligne suivante permet d'ajuster la date & l'heure
      // à la date de compilation du sketc
      RTC.adjust(DateTime(__DATE__, __TIME__));
  }

}

void loop() {
  // programme qui tourne en boucle

    //pinMode(pinLed, OUTPUT); //on éteint la led de arduino au démarrage
    unsigned long temps =millis();
    for (int numPorte=0; numPorte<NBPORTES; numPorte++)
    {
        int channelEntree = portes[numPorte].pinentree;
        portes[numPorte].lectureEntree = readMux(channelEntree);

        int channelSortie = portes[numPorte].pinsortie;
        portes[numPorte].lectureSortie = readMux(channelSortie);

        //on compte le nombre de passage devant le capteur, passé à 0
        if(portes[numPorte].lectureEntree == 0)
        {
            portes[numPorte].compteurEntree++;
        }

        if(portes[numPorte].lectureSortie == 0)
        {
            portes[numPorte].compteurSortie++;
        }

        // si nous n'avons plus de passage (capteur à 1)
        if(portes[numPorte].lectureEntree == 1)
        {
            // et que nousa avons eu plusieurs lecture positive
            if(portes[numPorte].compteurEntree > 1)
            {
                // alors nous incrémentons le passage
                passageA++;
                // et remettons notre compteur à 0
                portes[numPorte].compteurEntree = 0;
                // nous pouvons stocker le temps au passage:
                portes[numPorte].tempsLectureEntree = temps;
            }
        }
        // idem pour la porte pres de la sortie
        // si nous n'avons plus de passage (capteur à 1)
        if(portes[numPorte].lectureSortie == 1)
        {
            // et que nousa avons eu plusieurs lecture positive
            if(portes[numPorte].compteurSortie > 1)
            {
                // alors nous incrémentons le passage
                passageB++;
                // et remettons notre compteur à 0
                portes[numPorte].compteurSortie = 0;
                // nous pouvons stocker le temps au passage:
                portes[numPorte].tempsLectureSortie = temps;
            }
        }

        // verification du temps de passage :
        if(portes[numPorte].tempsLectureEntree > portes[numPorte].tempsLectureSortie)
        {
            // passée devant capteur entree avant celui de sortie: elle est sortie
            sorties++;

        }
        else
        {
            entrees++;
        }
    }
}

//readMux est la fonction qui lit chaque capteur appelé par readMux

int readMux(int channel){
  int controlPin[] = {s0, s1, s2, s3};

  int muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}, //channel 15
  };

   //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }

  //read the value at the SIG pin
  int val = digitalRead(SIG_pin);

  //return the value
  return val;

}
