  #include <Wire.h>
  #include <Adafruit_INA219.h>
  
  Adafruit_INA219 ina219;

  float voltage_V = 0,shuntVoltage_mV,busVoltage_V;
  float newvolt = 0;
  
  int sensorPin = A3;
  int inputPin = A0;
  int lamp = A1;  
  int pirState = LOW;
  int val = 0;       
  
  int lightCal;
  int lightVal;
  
void setup() {
  
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  
  pinMode(lamp, OUTPUT); 
  pinMode(inputPin, INPUT);
  
  lightCal = analogRead(sensorPin);
  
  Serial.begin(9600);
  ina219.begin();
  
}

void loop() {
  
  StartPro();
  LedLamp();
  delay(300);
  getData();
  Printer();
  delay(300);
  TwoLed();
  delay(300);

}

//---------------------------------------------------------------------------------------------------------

void LedLamp(){

    lightVal = analogRead(sensorPin);

    if (lightVal < lightCal - 50) //Нощ
    {
      val = digitalRead(inputPin);
        if (val == HIGH) 
        {          
          digitalWrite(lamp, HIGH);
          if (pirState == LOW) 
          {
            pirState = HIGH;
          }
        } 
        else 
        {
          digitalWrite(lamp, LOW); 
          if (pirState == HIGH)
          {
            pirState = LOW;
          }
        }
    }
    else // Ден
    {
        digitalWrite(lamp, LOW);
    }  
   
}
//---------------------------------------------------------------------------------------------------------

void StartPro() {
  
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
    
  }
  
//---------------------------------------------------------------------------------------------------------

void TwoLed(){

  if(newvolt > 0.75) 
  {
     digitalWrite(2, HIGH);
     delay(600);
  }
  if(newvolt > 1) 
  {
    digitalWrite(3, HIGH);
    delay(600);
  }
  if(newvolt > 2) 
  {
    digitalWrite(4, HIGH);
    delay(600);
  }
  if(newvolt > 3) 
  {
    digitalWrite(5, HIGH);
    delay(600);
  }
  if(newvolt > 4) 
  {
    digitalWrite(6, HIGH);
    delay(600);
  }
  if(newvolt > 5) 
  {
    digitalWrite(7, HIGH);
    delay(600);
  }
  if(newvolt > 6) 
  {
    digitalWrite(9, HIGH);
    delay(600);
  }
  if(newvolt > 7) 
  {
    digitalWrite(8, HIGH);
    delay(600);
  }
} 

//------------------------------------------------------------------------------------------------------

void getData(){
  
 busVoltage_V = (ina219.getBusVoltage_V()- 1.18) * 16;
 newvolt = busVoltage_V;
 
}

//----------------------------------------------------------------------------------------------------

void Printer(){
  
  Serial.print("Voltage on Path:   "); Serial.print(busVoltage_V); Serial.println(" V");
  Serial.println("----------------------------------------------------------------------------");
  
}




   
