#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"
#define mqtt_broker "broker.hivemq.com"
#define DHTTYPE DHT11 

WiFiClient esp;
PubSubClient client(esp);

const char* ssid="aidid";
const char* password="aididalam";
const char* dataReceiveTopic= "rma/TechPathshala/Receive";
const char* dataSendTopic= "rma/TechPathshala/Send";

String data="";
int red=D5;
int green=D7;
int DHTPin = D6;
DHT dht(DHTPin, DHTTYPE);                
String Temperature;
String Humidity;
unsigned long Time=millis();
unsigned long timeDelay=2000;
int pos=1;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(green,OUTPUT);
  pinMode(red,OUTPUT);
  pinMode(DHTPin, INPUT);
  dht.begin();

  WiFi.begin(ssid,password);

  while(WiFi.status()!=WL_CONNECTED){
    delay(1000);
    Serial.println("Connecting");
  }

  Serial.println("");
  Serial.println("Connected");
  Serial.println("Ip Address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_broker,1883);
  while(!client.connected()){
    if(client.connect("RMATUTORIAL")){
      Serial.print("Broker Connected");
    }else{
      Serial.println("Broker not connected");
      Serial.println("Retring in 2s");
      delay(2000);
    }
  }

  client.subscribe(dataReceiveTopic);
  client.setCallback(callback);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(data.startsWith("ledgreen")){
     digitalWrite(green,data.substring(8, data.length()).toInt());
   }
  if(data.startsWith("ledred")){
     analogWrite(red,data.substring(6, data.length()).toInt());
  }
  Temperature = (String)dht.readTemperature();
  Humidity = (String)dht.readHumidity();
  
  if(millis()-Time>timeDelay && pos==1){
    String str="temperature"+Temperature;
    client.publish(dataSendTopic, str.c_str());
    Time=millis();
    pos=2;
  }

  if(millis()-Time>timeDelay && pos==2){
    String str="humidity"+Humidity;
    client.publish(dataSendTopic, str.c_str());
    Time=millis();
    pos=1;
  }

  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length){
  String dt="";
  for(int i=0; i< length; i++){
    dt+= (char) payload[i];
  }

  data=dt;
}
