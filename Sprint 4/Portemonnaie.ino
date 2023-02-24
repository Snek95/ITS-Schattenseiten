/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-rfid-nfc
 */
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Arduino.h"

//WLAN Zugangsdaten
const char* ssid = "TP-LINK_D586";
const char* password = "14242830";
const char ip[] = "10.10.10.100";  //IP des Laptops
#define UDP_PORT 4211              //UDP Port für Touchdesigner

int sensorwert255 = 0;  
int sensorwertraw = 0;

WiFiUDP UDP;
char packet[255];

void setup() {
  pinMode(35, INPUT);
  Serial.begin(9600);
  WiFi.begin(ssid, password);  //WIFI Verbindung
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
}

void loop() {
  sensorwertraw = analogRead(35);
  if (sensorwertraw < 2000) {         //Mapping in binärwert
    sensorwert255 = 1;
  } else {
    sensorwert255 = 0;
  }
  UDP.beginPacket(ip, UDP_PORT);      //Dateien schicken zum TD in Laptop 1
  UDP.print(uint8_t(sensorwert255));  //Sensorwert in UDP Paket schreiben
  UDP.endPacket();                    //Paket schicken
  delay(50);
}
