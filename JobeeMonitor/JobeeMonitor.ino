/**
 * JobeeMonitor
 * Envoi de valeurs de capteurs via le port série
 * OpenWRT en lecture et envoi sur le web
 *
 */

#define DEBUG   0 // set to 1 to trace activity via serial console


#include <JeeLib.h>
#include <avr/sleep.h>

volatile bool adcDone;
ISR(WDT_vect) { Sleepy::watchdogEvent(); }
ISR(ADC_vect) { adcDone = true; }

#define SLEEP_DURATION 1000 * 10


#define RainSensor A7

/*
    Raingauge

    http://atelier-meteo-arduino-lycee-vincendo.blogspot.fr/p/le-pluviometre.html
*/
#define  Bucket_Size 0.2794 // bucket size to trigger tip (count was 0.01)
const    byte interruptRainGauge = 2;
volatile unsigned long tipCount; // bucket tip counter used in interrupt routine
volatile unsigned long contactTime; // Timer to manage any contact bounce in interrupt routine
volatile float totalRainfall; // total amount of rainfall detected

/**
    Wire is need for I2C
    TSL2561
    BMP280
*/
#include <Wire.h>

/*
DS3231 RTC https://github.com/adafruit/RTClib
*/
#include "RTClib.h"
RTC_DS3231 rtc;

/*
    SparkFun TSL2561 library
    Product page: https://www.sparkfun.com/products/11824
    Hook-up guide: https://learn.sparkfun.com/tutorials/getting-started-with-the-tsl2561-luminosity-sensor

    https://github.com/sparkfun/SparkFun_TSL2561_Arduino_Library
*/
#include <SparkFunTSL2561.h>

/*
    BMP280
*/
#include "BMP280.h"

/*
    Si7021 temperature and humidity sensor
    https://github.com/sparkfun/Si7021_Breakout/tree/master/Libraries
*/
#include "SparkFun_Si7021_Breakout_Library.h"

// Create an SFE_TSL2561 object, here called "light":
SFE_TSL2561 light;

// BMP280 bmp;
BMP280 bmp;

//Create Instance of HTU21D or SI7021 temp and humidity sensor and MPL3115A2 barrometric sensor
Weather SI7021;

// Global variables:

boolean TSLgain;     // Gain setting, 0 = X1, 1 = X16;
unsigned int TSLms;  // Integration ("shutter") time in milliseconds


float SI7021Humidity = 0;
float SI7021Temp     = 0;

struct data_t {
    double lux;
    double bmpP;
    double bmpT;
    int toptemperature;
    int entrytemperature;
    float humidity;
    float SI7021Temp;
    unsigned int RainFall;
    float totalRainfall;
    byte voltage;
    unsigned int time;
    byte id;
    }; // user defined data structure

data_t data; // define a variable with that structure

void setup()
{
  // Initialize the Serial port:
  Serial.begin(9600);

  // Rain gauge initialisation
  tipCount = 0;
  totalRainfall = 0;
  //Pullup reistance like push button to detect raingauge tip
  pinMode(interruptRainGauge, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptRainGauge),rainIncrement,FALLING);
  sei();// Enable Interrupts

  #if DEBUG
    Serial.println("Initialize TSL2561");Serial.flush();
  #endif
  light.begin();
  // If TSLgain = false (0), device is set to low gain (1X)
  // If TSLgain = high (1), device is set to high gain (16X)
  TSLgain = 0;
  // If time = 2, integration will be 402ms
  unsigned char TSLtime = 2;

  light.setTiming(TSLgain,TSLtime,TSLms);

  #if DEBUG
      Serial.println("Set timing...");
  #endif

  light.setTiming(TSLgain,TSLtime,TSLms);

  #if DEBUG
    Serial.println("Power up TSL2561");Serial.flush();
  #endif

  light.setPowerUp();

  if(!bmp.begin()){
    Serial.println("BMP init failed!");
  }
  bmp.setOversampling(4);

  //Initialize SI7021 sensor
  SI7021.begin();

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
  }
  else
  {
      if (rtc.lostPower()) {
          Serial.println("RTC lost power, lets set the time!");
          // following line sets the RTC to the date & time this sketch was compiled
          rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
          // This line sets the RTC with an explicit date & time, for example to set
          // January 21, 2014 at 3am you would call:
          // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
      }
  }
}

void loop()
{
    delay(TSLms);
    // There are two light sensors on the device, one for visible light
    // and one for infrared. Both sensors are needed for lux calculations.
    unsigned int data0, data1;
    double bmpT,bmpP;

    DateTime now = rtc.now();
    data.time = now.unixtime();

    if (light.getData(data0,data1))
    {
        #if DEBUG
            Serial.print("data0: ");
            Serial.print(data0);
            Serial.print(" data1: ");
            Serial.println(data1);
        #endif
        double TSLlux;    // Resulting lux value
        boolean TSLgood;  // True if neither sensor is saturated

        // Perform lux calculation:

        TSLgood = light.getLux(TSLgain,TSLms,data0,data1,TSLlux);
        if(TSLgood)
        {
            data.lux = TSLlux;
        }
        #if DEBUG
            Serial.print("lux: ");
            Serial.println(TSLlux);
        #endif
    }
    else
    {
        #if DEBUG
            Serial.println("no light sensor");
        #endif
    }

    char bmpResult = bmp.startMeasurment();
    if(bmpResult!=0)
    {
        bmpResult = bmp.getTemperatureAndPressure(bmpT,bmpP);

        if(bmpResult!=0)
        {
            data.bmpP = bmpP;
            data.bmpT = bmpT;
            #if DEBUG
                Serial.print("bmpT:");Serial.print(bmpT,2); Serial.print(" degC ");
                Serial.print("bmpP:");Serial.print(bmpP,2); Serial.println(" mBar");
            #endif
        }
    }
    else
    {
        #if DEBUG
            Serial.println("no BMP");
        #endif
    }

    data.humidity   = SI7021.getRH();
    data.SI7021Temp = SI7021.getTemp();
    #if DEBUG
        Serial.print("Humidity:");Serial.print(data.humidity); Serial.print("% ");
        Serial.print("SI7021Temp:");Serial.print(data.SI7021Temp); Serial.println("°C");
    #endif

    data.RainFall = analogRead(RainSensor);
    data.totalRainfall = totalRainfall;

    #if DEBUG
        Serial.print("RainFall:");Serial.print(data.RainFall);
        Serial.print(" RainLevel:");Serial.println(totalRainfall);
    #endif

    String tolog = builString();
    Serial.println(tolog);
    Serial.flush();delay(5);
    Sleepy::loseSomeTime(SLEEP_DURATION);

}

// Interrupt handler routine that is triggered when the rg-11 detects rain
void rainIncrement()
{
    if((millis() - contactTime) > 15 ) { // debounce of sensor signal
        tipCount++;
        totalRainfall = tipCount * Bucket_Size;
        contactTime = millis();
    }
}
// end of rg-11 rain detection interrupt handler

String builString()
{
    String dataString = "id=Danvou;";


    //sensors.requestTemperatures(); // envoi de la demande
    /**
    * temperature1 est un tableau contenant l'adresse
    * de notre capteur.
    * cf: https://git.io/vwM2x pour scanner les adresses
    double lux;
    double bmpP;
    double bmpT;
    int toptemperature;
    int entrytemperature;
    float humidity;
    float SI7021Temp;
    unsigned int RainFall;
    float totalRainfall;
    byte voltage;
    byte id;
    */

    dataString += "lux=";
    dataString += String(data.lux);
    dataString += ";";

    dataString += "bmpP=";
    dataString += String(data.bmpP);
    dataString += ";";

    dataString += "bmpT=";
    dataString += String(data.bmpT);
    dataString += ";";

    dataString += "topT=";
    dataString += String(data.toptemperature);
    dataString += ";";

    dataString += "entryT=";
    dataString += String(data.entrytemperature);
    dataString += ";";

    dataString += "h=";
    dataString += String(data.humidity);
    dataString += ";";

    dataString += "siT=";
    dataString += String(data.SI7021Temp);
    dataString += ";";

    dataString += "rainLevel=";
    dataString += String(data.totalRainfall);
    dataString += ";";

    dataString += "RainFall=";
    dataString += String(data.RainFall);
    dataString += ";";

    dataString += "milli=";
    dataString += String(millis());
    dataString += ";";
    return dataString;
}
