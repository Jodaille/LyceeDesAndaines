//  timer
unsigned long previousMillis = 0;
const long interval = 1000;


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial.print(__FILE__ " " __DATE__ " " __TIME__);
Serial.println("  IDE "); Serial.println(ARDUINO);

}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
 if(currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;
    Serial.print("id=");
    Serial.print("ruche1");
    Serial.print(";");

    Serial.print("pression=");
    Serial.print(pression());
    Serial.print(";");

    Serial.print("A1=");
    Serial.print(analogRead(A1));
    Serial.print(";");

    Serial.print("A2=");
    Serial.print(analogRead(A2));
    Serial.print(";");

    Serial.print("milli=");
    Serial.print(millis());
    Serial.println();
 }

}

float pression()
{
  int val = analogRead(A0);
  float tension = ((val-10)*0.0049);
  // Calcul de la pression reelle
  int pression_val = (100.1-(((tension/5)-0.04) / 0.009))*10;
  return pression_val; 
}
