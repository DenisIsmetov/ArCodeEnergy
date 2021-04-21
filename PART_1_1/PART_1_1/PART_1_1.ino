  #include "DHT.h"
  #include <SPI.h>
  #include <Ethernet.h>
  #include <Wire.h>
  #include <Adafruit_INA219.h>

  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; 
  
  #define DHTPIN A0
  #define DHTTYPE DHT11
  #define fsrpin A2
  
  DHT dht(DHTPIN,DHTTYPE);

  Adafruit_INA219 ina219;

  float voltage_V = 0,shuntVoltage_mV,busVoltage_V;
  float humidityData;
  float temperatureData;
  
  int fsrreading; 
  int previous = 0;
  int stepdata = 0; 
  
  long time = 0;  
  long debounce = 40; 
  
  char server[] = "energy-path.vshumen.com"; // Моят IPv4 адрес
  IPAddress ip(37, 64, 4, 237 ); //Моето публично ip
  EthernetClient client; 

void setup() {
  
  Serial.begin(9600);
  
  dht.begin();
  ina219.begin();
  
  if (Ethernet.begin(mac) == 0) {
  Serial.println("Failed to configure Ethernet using DHCP");
  Ethernet.begin(mac, ip);
  }
  delay(800);
}

void loop() {
  getData();
  Printer();
  delay(500);
  StepSensor();
  delay(700);
  humidityData = dht.readHumidity();
  temperatureData = dht.readTemperature(); 
  LanCard();
  delay(700);
  LanCardVolt();
  delay(700);
  LanCardStep();
  delay(700);
}
//---------------------------------------------------------------------------------------------------------
  void StepSensor(){
    
    fsrreading = analogRead(fsrpin);
  
    if (fsrreading > 200 && previous < 200 && millis() - time > debounce) 
    {
      Serial.println("1");
      stepdata = 1;
    }
    else
    {
      Serial.println("0");
      stepdata = 0;
    }
    
    previous = fsrreading;
  }
//---------------------------------------------------------------------------------------------------------

 void LanCard(){
   if (client.connect(server, 80)) { // Задаване на порт 8080, 80
    Serial.println("111---------- connected ----------111");
    // Make a HTTP request:
    Serial.print("GET /dht/dht.php?humidity=");
    client.print("GET /dht/dht.php?humidity=");     
    Serial.println(humidityData);
    client.print(humidityData);
    client.print("&temperature=");
    Serial.println("&temperature=");
    client.print(temperatureData);
    Serial.println(temperatureData);
    client.print(" ");      
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: energy-path.vshumen.com"); // Моят IPv4 адрес
    client.println("Connection: close");
    client.println();
    
  } else {
    // if you didn't get a connection to the server:
    Serial.println("111---------- connection failed ----------111");
  }
 }
 //---------------------------------------------------------------------------------------------------------

 void LanCardVolt(){
   if (client.connect(server, 80)) { 
    Serial.println("222---------- connected ----------222");
    // Make a HTTP request:
    Serial.print("GET /voltage/voltage.php?voltage=");
    client.print("GET /voltage/voltage.php?voltage="); 
    Serial.println(busVoltage_V);
    client.print(busVoltage_V);
    client.print(" ");      
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: energy-path.vshumen.com"); 
    client.println("Connection: close");
    client.println();
    
  } else {
    // if you didn't get a connection to the server:
    Serial.println("222---------- connection failed ----------222");
  }
 }

 //---------------------------------------------------------------------------------------------------------

 void LanCardStep(){
   if (client.connect(server, 80)) { 
    Serial.println("333---------- connected ----------333");
    // Make a HTTP request:
    Serial.print("GET /step/step.php?step=");
    client.print("GET /step/step.php?step="); 
    Serial.println(stepdata);
    client.print(stepdata);
    client.print(" ");  
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: energy-path.vshumen.com"); 
    client.println("Connection: close");
    client.println();
    
  } else {
    Serial.println("333---------- connection failed ----------333");
  }
 }
//------------------------------------------------------------------------------------------------------
void getData(){
  
 busVoltage_V = (ina219.getBusVoltage_V() - 1.19) * 16;
 
}

//----------------------------------------------------------------------------------------------------

void Printer(){
  
  Serial.print("Voltage on Path:   "); Serial.print(busVoltage_V); Serial.println(" V");
  Serial.println("----------------------------------------------------------------------------");
  
}
 


   
