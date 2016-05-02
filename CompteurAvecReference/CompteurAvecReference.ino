int entrees = 0;

const int sensorA = 2;
int countA = 0;
int stateA = 1;
const int sensorB = 3;
int countB = 0;
int stateB = 1;

void setup() {
  Serial.begin(9600);
  Serial.println("Start");
}

void loop() {
  int vA = digitalRead(sensorA);
  int vB = digitalRead(sensorB);
  if(vA == 0)
  {
    stateA = 0;
    if(stateA == 0)
    {
      Serial.print(countA);Serial.println("++");
      countA++;
    }

  }
  else
  {
    if(countA > 1)
    {
      countA = 0;
      stateA = 0;
      entrees++;
      Serial.print("entrees: ");Serial.print(entrees);
      Serial.println(" reset compteur");
      
    }
  }

}
