/*
http://cactus.io/projects/weather/arduino-weather-station-rain-sensor
*/

// http://atelier-meteo-arduino-lycee-vincendo.blogspot.fr/p/le-pluviometre.html
#define Bucket_Size 0.2794 // bucket size to trigger tip (count was 0.01)   
#define RG11_Pin 3 // digital pin RG11 connected to 


volatile unsigned long tipCount; // bucket tip counter used in interrupt routine 
volatile unsigned long contactTime; // Timer to manage any contact bounce in interrupt routine 
volatile float totalRainfall; // total amount of rainfall detected 


void setup() { 
Serial.begin(9600);
tipCount = 0; 
totalRainfall = 0; 

pinMode(RG11_Pin, INPUT_PULLUP); 
attachInterrupt(digitalPinToInterrupt(RG11_Pin), isr_rg, FALLING); 
sei();// Enable Interrupts 
} 

void loop() { 

Serial.print(totalRainfall); Serial.println(" mm"); 

// Add a 2 second delay. 
delay(2000); //just here to slow down the output. 

} 

// Interrupt handler routine that is triggered when the rg-11 detects rain 
void isr_rg() { 

if((millis() - contactTime) > 15 ) { // debounce of sensor signal 
tipCount++; 
totalRainfall = tipCount * Bucket_Size; 
contactTime = millis(); 
} 
} 
// end of rg-11 rain detection interrupt handler
