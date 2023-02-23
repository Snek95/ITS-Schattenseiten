// Load Wi-Fi library
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

// Replace with your network credentials
const char* ssid     = "FRITZ!Box 7412";  //Wlan-name
const char* password = "58184142373790392728";  //Wlan Passwort
const char ip1[] = "192.168.178.20";   //Ip, an die gesendet wird
const char ip2[] = "10.163.100.8";    // Optional
#define UDP_PORT 4122                   //UDP Port über den gesendet wird (=UDPin-Port im TD File)
String Data = "Hi 1234, 4321 Byye ";
int n = 0;

WiFiUDP UDP;
char packet[255];
int sensorwert = 0;     //Erstellen des Integers "Sensorwert"

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(ssid, password);           //WLAN Verbinden
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  
  UDP.begin(UDP_PORT);                         //UDP Starten

  // Try to initialize!
  if (!mpu.begin()) {                       //Beschleunigungssensor "MPU6050" an den Start bringen
    Serial.println("Failed to find MPU6050 chip");      //Nachricht, falls verbindung zum MPU nicht funktioniert
    while (1) {
      delay(0);
    }
  }
    Serial.println("MPU6050 Found!");          //MPU gefunden

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.println("");
  delay(100);

}


void loop(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);      //auslesen der aktuellen Beschleunigungs,- Rotations- und Temperaturwerte am Anfang der Schleife
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);     //Printen des aktuellen x-achsen-Beschleunigungswertes 
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);     //Printen des aktuellen y-achsen-Beschleunigungswertes
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);     //Printen des aktuellen z-achsen-Beschleunigungswertes
  Serial.println(" m/s^2");
  sensorwert= abs(a.acceleration.x)+abs(a.acceleration.y)+abs(a.acceleration.z); // Addieren der einzelnen Achsenwerte und "Sensorwert" nennen
  Serial.println(sensorwert);         //Printen des Sensorwertes
  Serial.println("");

  
  Data.toCharArray( packet , Data.length() );       // Sendung per UDP
  UDP.beginPacket(ip1, UDP_PORT);
  UDP.print(uint8_t(sensorwert));                   // Sensorwert senden
  UDP.endPacket();
  
}
