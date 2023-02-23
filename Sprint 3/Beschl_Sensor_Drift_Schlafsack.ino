// Load Wi-Fi library
#include <WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;

// Replace with your network credentials
const char* ssid     = "FRITZ!Box 7412";
const char* password = "58184142373790392728";
const char ip1[] = "192.168.178.20";   //meine ip
const char ip2[] = "10.163.100.8";    // Optional für weiteren PC
#define UDP_PORT 7000
String Data = "Hi 1234, 4321 Byye ";
int n = 0;

WiFiUDP UDP;
char packet[255];
int sensorwert = 0;

float xmem=0;
float ymem=0;
float zmem=0;
int motion=0;

void setup() {
  Serial.begin(115200);
  delay(0);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(0);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  
  UDP.begin(UDP_PORT);

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(0);
    }
  }
    Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.println("");
  delay(0);

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  xmem=a.acceleration.x;        // Grundwert, der aktuell im Sensor detektiert wird
  ymem=a.acceleration.y;
  zmem=a.acceleration.z;
}


void loop(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");
  //sensorwert=abs(a.acceleration.z)+abs(a.acceleration.x)+abs(a.acceleration.y);
  Serial.println(sensorwert);
  Serial.println("");

  if((abs(a.acceleration.x/xmem)-1)>0.05){   //anpassen, bei hochheben möglicherweise größerer prozentwert (grenzwert)
    motion+=1;
  }
  if((abs(a.acceleration.y/ymem)-1)>0.05){   // Wenn Ergebnis von "aktueller Wert durch den Grundwert minus 1" einen Wert höher als den eingestellten Grenzwert hat, dann wird eine 1 generiert
    motion+=1;
  }
  if((abs(a.acceleration.z/zmem)-1)>0.05){   //jede Achse wird hier mit einbezogen
    motion+=1;
  }
  if(motion>1){
    sensorwert=1;     // 0 oder 1 wird in Sensorwert gespeichert und geschickt
  }
  
  Data.toCharArray( packet , Data.length() );
  UDP.beginPacket(ip1, UDP_PORT);
  UDP.print(uint8_t(sensorwert));
  UDP.endPacket();
  motion=0;
  sensorwert=0;
  xmem=a.acceleration.x;
  ymem=a.acceleration.y;
  zmem=a.acceleration.z;
  delay(10);
}
