// Load Wi-Fi library
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(25, 26);                //25 connected to Rx, 26 connected to Tx in SD reader
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

// Replace with your network credentials
const char* ssid     = "TP-LINK_D586";                       //WIFI name
const char* password = "14242830";                    //WIFI passwort
const char ip[] = "10.10.10.100";                    //IP des Laptops
#define UDP_PORT 4100                                   //UDP Port wahlen

WiFiUDP UDP;
char packet[255];
int sensorwert = 0;
int sensorwert255 = 0;
int foton = 33;                                         //Eingang 33 für Fotonsensor
bool isPlaying = false;
int taster = 27;                                        //Eingang 27 für Resettaster  
int tasterstatus = 0;

void setup() {
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  WiFi.begin(ssid, password);                           //WIFI verbinden
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
      
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(10);                                //Set volume value. From 0 to 30
  myDFPlayer.play(1);                                   //Das erste Audiodatei spielen (die Nummer zeigt die Ordnung der Datein in der SD-Karte. Man soll die Datein als Nummer benennen (von 1 bis ...)
  myDFPlayer.pause();                                   //Das Audiodatei pausieren, damit wir danach nur weiterspielen und pausieren müssen
  
  UDP.begin(UDP_PORT);                                  //Output für LED einstellen
  pinMode(foton, INPUT);                                //Input für Fotonsensor
  pinMode(taster, INPUT);                               //Input für Resettaster
}

void loop(){
  tasterstatus=digitalRead(taster);                     //Taster lesen mit Boolean-Wert
  sensorwert = analogRead(foton);                       //Sensor wird mit dem Eingang 33 vom ESP verbunden
  sensorwert255 = map(sensorwert, 0, 4096, 0, 255);     //Sensorwert runterskalieren (Lichtsensor)

  Serial.print("; Helligkeit : ");
  Serial.println(sensorwert255);
  
  UDP.beginPacket(ip, UDP_PORT);                        //Dateien schicken zum TD in Laptop 1
  UDP.print(uint8_t(sensorwert255));                    //Sensorwert in UDP reinschreiben
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

  if (sensorwert255 > 100) {                             //Audio weiterspielen wenn es hell ist
    if (!isPlaying) {
      myDFPlayer.start();
      isPlaying = true;
    }
  }
  if (sensorwert255 < 100) {                            //Audio pausieren wenn es dunkel ist
    if (isPlaying) {
      myDFPlayer.pause();
      isPlaying = false;
    }
  }

  if (tasterstatus == HIGH) {                           //Datei wiederspielen und gerade pausieren wenn der Taster gedrückt wird
    myDFPlayer.play(1);
    myDFPlayer.pause();
    isPlaying = false;
  }
  
  delay(500);                                           //0.5s Pause inzwischen
}
