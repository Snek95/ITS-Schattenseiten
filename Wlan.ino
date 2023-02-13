// Load Wi-Fi library
#include <WiFi.h>
#include <WiFiUdp.h>
#define RXp2 16
#define TXp2 17

// Replace with your network credentials
const char* ssid     = "TP-LINK_D586";                       //WIFI name
const char* password = "14242830";                    //WIFI passwort
const char ip[] = "10.10.10.100";                    //IP des Laptops
#define UDP_PORT 4201                                   //UDP Port wahlen

WiFiUDP UDP;
char packet[255];      

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2);
  WiFi.begin(ssid, password);                           //WIFI verbinden
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  UDP.begin(UDP_PORT);                                  
}

void loop(){
  int Sensor;
  String data = Serial2.readStringUntil('\n');
  Serial.println(data);
  Sensor = data.toInt();
  
  UDP.beginPacket(ip, UDP_PORT);                        //Dateien schicken zum TD in Laptop 1
  UDP.print(uint8_t(Sensor));                    //Sensorwert in UDP reinschreiben
  UDP.endPacket();                                      //Paket schicken

  int packetSize = UDP.parsePacket();                   //Dateien empfangen vom TD
  if (packetSize) {
    //Serial.print("Received packet! Size: ");
    //Serial.println(packetSize); 
    int len = UDP.read(packet, 255);
    if (len > 0)
    {
      packet[len] = '\0';
    }
    //Serial.print("Packet received: ");
    Serial.println(packet);
  }             
  delay(50);                                           //0.5s Pause inzwischen
}
