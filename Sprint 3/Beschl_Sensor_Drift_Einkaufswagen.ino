// Load Wi-Fi library
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

// Replace with your network credentials
const char* ssid     = "FRITZ!Box 7412";      //Wlan-name
const char* password = "58184142373790392728"; //Wlan Passwort
const char ip1[] = "192.168.178.20";   //Ip, an die gesendet wird
const char ip2[] = "10.163.100.8";    // Optional für weiteren PC
#define UDP_PORT 4120                   //UDP Port über den gesendet wird (=UDPin-Port im TD File)
String Data = "Hi 1234, 4321 Byye ";
int n = 0;

WiFiUDP UDP;
char packet[255];
int sensorwert = 0;       //Erstellen des Integers "Sensorwert"

float xmem=0; //Erstellen des Integers "xmem"
float ymem=0; //Erstellen des Integers "ymem"
float zmem=0; //Erstellen des Integers "zmem"
int motion=0;  //Erstellen des Integers "motion"

void setup() {
  Serial.begin(115200);
  delay(0);

  WiFi.begin(ssid, password);               //WLAN Verbinden
  while (WiFi.status() != WL_CONNECTED) {
    delay(0);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  
  UDP.begin(UDP_PORT);                      //UDP Starten

  // Try to initialize!
  if (!mpu.begin()) {                       //Beschleunigungssensor "MPU6050" an den Start bringen
    Serial.println("Failed to find MPU6050 chip");    //Nachricht, falls verbindung zum MPU nicht funktioniert
    while (1) {
      delay(0);
    }
  }
    Serial.println("MPU6050 Found!");             //MPU gefunden

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);   //Beschleunigungswerten eine Range bis 2G geben
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);        // Gyrowerte (Rotation) Werte bis 500Grad auslesen
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);      // Bandbreite setzen
  Serial.println("");
  delay(0);

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  xmem=a.acceleration.x;        // Beschleunigungswert, der im Sensor auf der x-Achse detektiert wird, wird xmem genannt
  ymem=a.acceleration.y;        // Beschleunigungswert, der im Sensor auf der y-Achse detektiert wird, wird ymem genannt
  zmem=a.acceleration.z;        // Beschleunigungswert, der im Sensor auf der z-Achse detektiert wird, wird zmem genannt
}


void loop(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);        //auslesen der aktuellen Beschleunigungs,- Rotations- und Temperaturwerte am Anfang der Schleife
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);     //Printen des aktuellen x-achsen-Beschleunigungswertes 
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);     //Printen des aktuellen y-achsen-Beschleunigungswertes
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);     //Printen des aktuellen z-achsen-Beschleunigungswertes
  Serial.println(" m/s^2");
  
  Serial.println(sensorwert);         //Printen des Sensorwertes
  Serial.println("");

  if((abs(a.acceleration.x/xmem)-1)>0.05){   // Wenn Erhöhung des aktuellen X-Wertes (a.acceleration.x) im Vergleich zum vorherigen Wert (xmem) um 5% Größer ist, dann wird integer "motion" um 1 Wert höher gesetzt
    motion+=1;
    Serial.println(xmem);
  }
  if((abs(a.acceleration.y/ymem)-1)>0.05){   // Wenn Erhöhung des aktuellen Y-Wertes (a.acceleration.y)  im Vergleich zum vorherigen Wert (ymem) um 5% Größer ist, dann wird integer "motion" um 1 Wert höher gesetzt
    motion+=1;
    Serial.println(ymem);
  }
  if((abs(a.acceleration.z/zmem)-1)>0.05){   // Wenn Erhöhung des aktuellen Z-Wertes (a.acceleration.z)  im Vergleich zum vorherigen Wert (zmem) um 5% Größer ist, dann wird integer "motion" um 1 Wert höher gesetzt
    motion+=1;
    Serial.println(zmem);
  }
  if(motion>1){       // Wenn "motion" einen Wert größer als 1 bekommt, dann findet eine Beschleunigung statt und das Programm sendet mit dem "Sensorwert" eine 1 per UDP an Touchdesigner
    sensorwert=1;     // 0 oder 1 wird in "Sensorwert" gespeichert und geschickt
  }
  
  Data.toCharArray( packet , Data.length() );     // Sendung per UDP
  UDP.beginPacket(ip1, UDP_PORT);
  UDP.print(uint8_t(sensorwert));
  UDP.endPacket();
  motion=0;                                       // "motion" nach jedem Durchlauf wieder auf Null setzen, Drift ausgleichen
  sensorwert=0;                                   // Sensorwert nach jedem Durchlauf wieder auf Null setzen
  xmem=a.acceleration.x;                          // Aktueller X-Achsenwert am Ende der Schleife als xmem abgespeichert
  ymem=a.acceleration.y;                          // Aktueller Y-Achsenwert am Ende der Schleife als ymem abgespeichert
  zmem=a.acceleration.z;                          // Aktueller Z-Achsenwert am Ende der Schleife als zmem abgespeichert
  delay(10);
}
