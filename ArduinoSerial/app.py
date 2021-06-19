import serial
import time
import paho.mqtt.client as mqtt

dataReceiveTopic= "rma/Techpathala/Receive"
dataSendTopic="rma/Techpathala/Send"
arduino = serial.Serial(port='COM9',  baudrate=115200, timeout=0.1)

def sendData(x):
    arduino.write(bytes(x, 'utf-8'))
    time.sleep(0.05)

def on_connect(client,userdata, flags,rc):
    print("Mqtt Connected")
    client.subscribe(dataReceiveTopic)

def on_message(client, userdata, msg):
    sendData(msg.payload.decode())

client = mqtt.Client()
client.on_connect=on_connect
client.on_message=on_message

client.connect("broker.hivemq.com" ,1883, 60)

while True:
        data=arduino.readline()
        if len(data)>0:
            client.publish(dataSendTopic,data)
            client.loop()
