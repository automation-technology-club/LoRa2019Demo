/* LeRoy Miller (C) 2018, see license.h for license information */

/*
 *  LoRa32u4II LoRa Connected to:
  (7) <----> DIO0
  (8) <----> SEL (Chip Select (SS))
  (4) <----> RST
  *
  */
  
#include <SPI.h>
#include <LoRa.h> //https://github.com/sandeepmistry/arduino-LoRa
#include <Wire.h>
 
#define NODE 2 //Node Number of Sensor
#define ONE_WIRE_BUS 2
#define SS      8 
#define RST     4 
#define DI0     7 


float t, h; //tempature and humidity
int counter;

void setup() {
  Serial.begin(9600);
  //while (!Serial);
LoRa.setPins(SS,RST,DI0);
  Serial.println("LoRa Sender");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
//  sensors.begin();
  }

void loop() {
  getTemp();
  sendPacket();
  
}

void getTemp() {

//sensors.requestTemperatures();
  t = random(50, 85); //faking a temperature why not
  h = random(0.0,99.0); //humidity not supported by sensor so fake it
}

void sendPacket() {
  Serial.print("Sending packet: ");
  char hex[20];
  sprintf(hex, "%d %d.%02d %d.%02d",NODE,(int)t,(int)(t*100)%100,(int)h,(int)(h*100)%100);
  //sprintf(hex, "%d.%02d %d.%02d",(int)t,(int)(t*100)%100,(int)h,(int)(h*100)%100);
  // send packet
  LoRa.beginPacket();
  LoRa.print((String)hex);
  LoRa.endPacket();
  delay(2000);
}
