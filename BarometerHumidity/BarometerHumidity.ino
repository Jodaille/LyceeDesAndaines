#include "BMP280.h"
#include "SparkFun_Si7021_Breakout_Library.h"
#include <Wire.h>

#define P0 1025.25

float humidity = 0;
float tempf = 0;

BMP280 bmp;

Weather sensor;

// Loop timer
unsigned long previousMillis = 0;
const long interval = 100;

// Define the number of samples to keep track of.  The higher the number,
// the more the readings will be smoothed, but the slower the output will
// respond to the input.  Using a constant rather than a normal variable lets
// use this value to determine the size of the readings array.
const int numReadings = 4;

int readings;      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int inputPin = A7;

void setup()
{
  Serial.begin(9600);
  if(!bmp.begin()){
    Serial.println("BMP init failed!");
    while(1);
  }
  else Serial.println("BMP init success!");

  bmp.setOversampling(4);

  sensor.begin();

  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings = 0;
  }

}
void loop()
{
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
    // save the last time 
    previousMillis = currentMillis;

    // subtract the last reading:
    total = total - readings;
    // read from the sensor:
    readings = analogRead(inputPin);
    // add the reading to the total:
    total = total + readings;
    // advance to the next position in the array:
    readIndex = readIndex + 1;

    // if we're at the end of the array...
    if (readIndex >= numReadings) {
      // ...wrap around to the beginning:
      readIndex = 0;
    }
  
    // calculate the average:
    average = total / numReadings;

    double T,P;
    char result = bmp.startMeasurment();

    getWeather();
    printInfo();
  
    if(result!=0){
  
      result = bmp.getTemperatureAndPressure(T,P);

      if(result!=0)
      {
        double A = bmp.altitude(P,P0);

        Serial.print(" T=");Serial.print(T,2); Serial.print("°C ");
        Serial.print("P=");Serial.print(P,2); Serial.print("mBar ");
        Serial.print("A=");Serial.print(A,2); Serial.print("m ");
        Serial.print("Rain=");Serial.print(average); Serial.println("");
        

      }
      else {
        Serial.println("Error.");
      }
    }
    else {
      Serial.println("Error.");
    }

  }
}

void getWeather()
{
  // Measure Relative Humidity from the HTU21D or Si7021
  humidity = sensor.getRH();

  // Measure Temperature from the HTU21D or Si7021
  tempf = sensor.getTemp();
  // Temperature is measured every time RH is requested.
  // It is faster, therefore, to read it from previous RH
  // measurement with getTemp() instead with readTemp()
}
//---------------------------------------------------------------
void printInfo()
{
//This function prints the weather data out to the default Serial Port

  Serial.print("Temp:");
  Serial.print(tempf);
  Serial.print("C, ");

  Serial.print("Humidity:");
  Serial.print(humidity);
  Serial.print("%");
}
