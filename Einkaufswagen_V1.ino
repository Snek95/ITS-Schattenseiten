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
const char ip2[] = "10.163.100.8";    // Optional
#define UDP_PORT 4122
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
  delay(100);

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
  sensorwert= abs(a.acceleration.x)+abs(a.acceleration.y)+abs(a.acceleration.z);
  Serial.println(sensorwert);
  Serial.println("");

  
  Data.toCharArray( packet , Data.length() );
  UDP.beginPacket(ip1, UDP_PORT);
  UDP.print(uint8_t(sensorwert));
  UDP.endPacket();
  
}
