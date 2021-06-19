#include "DHT.h"
#define DHTTYPE DHT11

int red=5;
int green=7;
int DHTpin=6;

DHT dht(DHTpin,DHTTYPE);

String Temperature;
String Humidity;

unsigned long Time= millis();
unsigned long timeDelay=2000;
int pos=1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(green,OUTPUT);
  pinMode(red,OUTPUT);
  pinMode(DHTpin,INPUT);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){
    String data= Serial.readString();
    //ledgreen1
    if(data.startsWith("ledgreen")){
      digitalWrite(green,data.substring(8, data.length()).toInt());
    }
    
    if(data.startsWith("ledred")){
      analogWrite(red,data.substring(6, data.length()).toInt());
    }
  }
  
  Temperature = (String)dht.readTemperature();
  Humidity = (String)dht.readHumidity();
  //Serial.println(Temperature+" "+Humidity);
  //delay(1000);
  
  if(millis()-Time>timeDelay && pos==1){
    Serial.println("temperature"+Temperature);
    Time=millis();
    pos=2;
  }

  if(millis()-Time>timeDelay && pos==2){
      Serial.println("humidity"+Humidity);
      Time=millis();
      pos=1;
  }
}
