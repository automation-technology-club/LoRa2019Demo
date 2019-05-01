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
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_D1_SSD1306.h>


#define SS      D4 
#define RST     22 
#define DI0     D8 
#define OLED_RESET 0  // GPIO0
Adafruit_D1_SSD1306 display(OLED_RESET);
#define APIN D3 //button A on OLED
#define RELAYNODE 1

//float t, h; //tempature and humidity
float t,h;
int n;
byte relayState = LOW;

void setup() {
  pinMode(APIN, INPUT_PULLUP); //A
  attachInterrupt(digitalPinToInterrupt(APIN), displayA, HIGH);
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");
 LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();
}

void loop() {
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
    //Serial.println(buffer);
n = buffer.toInt();
buffer.remove(0,2);
t = buffer.toFloat();
buffer.remove(0,6);
h = buffer.toFloat();
updateOLED();
    Serial.print("N: "); Serial.print(n); Serial.print(" T: ");Serial.print(t); Serial.print(" H: ");Serial.println(h);
    yield();
  }
  yield();
}

void updateOLED() {

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.print("Node: "); display.println(n);
  display.setTextSize(2);
  display.println(t);
  display.setTextSize(1);
  display.println("H: ");
  display.setTextSize(2);
  display.println(h);
  display.display();
  
}

void displayA() {
  relayState = !relayState;
  char hex[4];
  sprintf(hex,"%d,%d", RELAYNODE, relayState);
  // send packet
  LoRa.beginPacket();
  LoRa.print((String)hex);
  LoRa.endPacket();
  delay(150);
  yield();
}
