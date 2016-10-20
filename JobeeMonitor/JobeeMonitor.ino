/**
 * JobeeMonitor
 * Envoi de valeurs de capteurs via le port série
 * OpenWRT en lecture et envoi sur le web
 *
 */

#define DEBUG   1 // set to 1 to trace activity via serial console

/**
    Wire is need for I2C
    TSL2561
    BMP280
*/
#include <Wire.h>

/* SparkFun TSL2561 library
Product page: https://www.sparkfun.com/products/11824
Hook-up guide: https://learn.sparkfun.com/tutorials/getting-started-with-the-tsl2561-luminosity-sensor

https://github.com/sparkfun/SparkFun_TSL2561_Arduino_Library
*/
#include <SparkFunTSL2561.h>

/*
BMP280
*/
#include "BMP280.h"

// Create an SFE_TSL2561 object, here called "light":
SFE_TSL2561 light;

// BMP280 bmp;
BMP280 bmp;

// Global variables:

boolean TSLgain;     // Gain setting, 0 = X1, 1 = X16;
unsigned int TSLms;  // Integration ("shutter") time in milliseconds


struct data_t {
    double lux;
    double bmpP;
    double bmpT;
    int toptemperature;
    int entrytemperature;
    int humidity;
    byte voltage;
    byte id;
    }; // user defined data structure
data_t data; // define a variable with that structure

void setup()
{
  // Initialize the Serial port:
  Serial.begin(9600);


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
}

void loop()
{
    delay(TSLms);
    // There are two light sensors on the device, one for visible light
    // and one for infrared. Both sensors are needed for lux calculations.
    unsigned int data0, data1;
    double bmpT,bmpP;

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
}
