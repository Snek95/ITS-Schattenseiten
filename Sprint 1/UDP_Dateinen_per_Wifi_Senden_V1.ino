// Load Wi-Fi library
#include <WiFi.h>
#include <WiFiUdp.h>

// Replace with your network credentials
const char* ssid     = "DigiNet";
const char* password = "DigiNetE63";
const char ip1[] = "10.163.100.9";
const char ip2[] = "10.163.100.8";
#define UDP_PORT 4210
String Data = "Hi 1234, 4321 Byye ";
int n = 0;

WiFiUDP UDP;
char packet[255];
int sensorwert = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  
  UDP.begin(UDP_PORT);
}

void loop(){
  sensorwert = analogRead(33);
  Data.toCharArray( packet , Data.length() );
  //Serial.println(map(sensorwert, 0, 4096, 0, 255));
  //Serial.println(n);
  //Serial.println(packet);
  
  UDP.beginPacket(ip1, UDP_PORT);
  UDP.print(uint8_t(map(sensorwert, 0, 4096, 0, 255)));
  //UDP.print(n%15);
  UDP.endPacket();
  
  UDP.beginPacket(ip2, UDP_PORT);        
  UDP.print(uint8_t(map(sensorwert, 0, 4096, 0, 255)));
  //UDP.print(n);
  UDP.endPacket();

  // If packet received...
  int packetSize = UDP.parsePacket();
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
  
  delay(500);
}
