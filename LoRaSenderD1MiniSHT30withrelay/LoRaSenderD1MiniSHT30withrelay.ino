/* LeRoy Miller (C) 2018, see license.h for license information */

/*
 *  WeMos D1          RFM9x Module
  GPIO12 (D6) <----> MISO
  GPIO13 (D7) <----> MOSI
  GPIO14 (D5) <----> CLK
  GPIO15 (D8) <----> DIO0/D2 OR DIO1/D3 OR DIO2/D4
  GPIO02 (D4) <----> SEL (Chip Select)
  RST         <----> RST
  * Appears D1, D2, D3, A0, and D0 free
  */
  
#include <SPI.h>
#include <LoRa.h> //https://github.com/sandeepmistry/arduino-LoRa
#include <Wire.h>

#include <WEMOS_SHT3X.h> //https://github.com/wemos/WEMOS_SHT3x_Arduino_Library

#define NODE 1 //Node Number of Sensor
#define SS      D4 
#define RST     22 
#define DI0     D8 
#define RELAY   D1

SHT3X sht30(0x45);

float t, h; //tempature and humidity
int counter;

void setup() {
  Serial.begin(9600);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  while (!Serial);
LoRa.setPins(SS,RST,DI0);
  Serial.println("LoRa Sender");

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
}

void loop() {
  getTemp();
  sendPacket();
  recieveLoRa();
  yield();
  //delay(150);
  //ESP.deepSleep(15*1000000);
}

void recieveLoRa() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
 String buffer;
    // read packet
    while (LoRa.available()) {
      //Serial.print((char)LoRa.read());
      buffer = buffer+(char)LoRa.read();
    }
    Serial.println(buffer);

int c = buffer.toInt();
Serial.println(c);
buffer.remove(0,2);
Serial.println(buffer);
int g = buffer.toInt();
Serial.println(g);
   if (c == NODE) { 
    if (g == 0) {
      digitalWrite(RELAY, LOW); //relay off
         }
    else if (g == 1) {
      digitalWrite(RELAY, HIGH); //relay on    
    }
   }
  }
  
}

void getTemp() {

if (sht30.get() == 0) {
  t = sht30.fTemp;
  h = sht30.humidity;
} else {
  Serial.println("Error!");
}
}
void sendPacket() {
  Serial.println("Sending packet");
  char hex[20];
  sprintf(hex, "%d %d.%02d %d.%02d",NODE,(int)t,(int)(t*100)%100,(int)h,(int)(h*100)%100);
  //sprintf(hex, "%d.%02d %d.%02d",(int)t,(int)(t*100)%100,(int)h,(int)(h*100)%100);
  //sprintf(hex, "%02d %02d",(int)t,(int)h);
  
  // send packet
  LoRa.beginPacket();
  LoRa.print((String)hex);
  LoRa.endPacket();
  delay(2000);
}
