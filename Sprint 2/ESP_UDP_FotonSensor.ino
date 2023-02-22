// Load Wi-Fi library
#include <WiFi.h>
#include <WiFiUdp.h>

// Replace with your network credentials
const char* ssid     = "reddevils";                       //WIFI name
const char* password = "Hoilamgi_41";                    //WIFI passwort
const char ip[] = "192.168.178.165";                      //IP des Laptops
#define UDP_PORT 4210                                   //UDP Port wahlen

WiFiUDP UDP;
//char packet[255];
//int fotonLED = 26;
int sensorwert = 0;
int sensorwert255 = 0;
int foton = 33;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);                           //WIFI verbinden
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  
  UDP.begin(UDP_PORT);                                  //UDP Port für Dateiempfängen einsetzen
  //pinMode(fotonLED, OUTPUT);                                  //Output für LED einstellen
  pinMode(foton, INPUT);
}

void loop(){
  sensorwert = analogRead(foton);                       //Sensor wird mit dem Eingang 33 vom ESP verbunden
  sensorwert255 = map(sensorwert, 0, 4096, 0, 255);     //Sensorwert runterskalieren (Lichtsensor)

  Serial.print("; Helligkeit : ");
  Serial.println(sensorwert255);
  
  UDP.beginPacket(ip, UDP_PORT);                       //Dateien schicken zum TD in Laptop 1
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

  /*if (sensorwert255 > 100) {                            //LED anschalten wenn es zu hell ist
    digitalWrite(fotonLED, LOW);
  }
  else {                                                //LED ausschalten wenn es zu dunkel ist
    digitalWrite(fotonLED, HIGH);
  }
*/
  delay(500);                                           //0.5s Pause inzwischen
}
