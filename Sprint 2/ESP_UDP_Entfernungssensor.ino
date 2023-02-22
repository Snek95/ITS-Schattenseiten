// Load Wi-Fi library
#include <WiFi.h>
#include <WiFiUdp.h>

// Replace with your network credentials
const char* ssid     = "DigiNet";                       //WIFI name
const char* password = "DigiNetE63";                    //WIFI passwort
const char ip[] = "192.168.178.165";                      //IP des 1. Laptops
#define UDP_PORT 4210                                   //UDP Port wahlen 

WiFiUDP UDP;
char packet[255];
int entfernLED = 27;
int trigger = 25;
int echo = 32;
long entfernung = 0;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);                           //WIFI verbinden
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  
  UDP.begin(UDP_PORT);                                  //UDP Port für Dateiempfängen einsetzen                       
  pinMode(entfernLED, OUTPUT);                          //Output für LED einstellen
  pinMode(trigger, OUTPUT);                             //Trigger-Lautsprecher
  pinMode(echo, INPUT);                                 //Aufnahme-Lautsprecher
}

void loop(){
  entfernung = get_distance();

  Serial.print("Entfernung : ");
  Serial.print(entfernung);
  Serial.print("cm ");
  
  UDP.beginPacket(ip, UDP_PORT);                       //Dateien schicken zum TD in Laptop 1
  UDP.print(uint8_t(entfernung));                    //Sensorwert in UDP reinschreiben
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

  if ((entfernung <= 20)&&(entfernung >= 0)) {          //LED anschalten wenn etwas in der Nähe von 20cm ist
    digitalWrite(entfernLED, HIGH);
  }
  else {                                                //LED ausschalten wenn nicht
    digitalWrite(entfernLED, LOW);
  }
  delay(500);                                           //0.5s Pause inzwischen
}

long get_distance() {
  long duration=0;
  long distance=0;

  digitalWrite(trigger, LOW);       //Den Trigger-Lautsprecher aus
  delayMicroseconds(5);
  //noInterrupts();
  digitalWrite(trigger, HIGH);      //Den Trigger-Lautsprecher an
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);       //Den Trigger-Lautsprecher wieder aus
  duration = pulseIn(echo, HIGH);   //Erfassung - Dauer in Mikrosekunden
  //interrupts();

  distance = (duration/2) * 0.03432; // Distanz in CM
  return(distance);
}
