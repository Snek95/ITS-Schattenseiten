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
//const char ip2[] = "10.163.100.8";    // Optional für weiteren PC
#define UDP_PORT 4120
String Data = "Hi 1234, 4321 Byye ";
int n = 0;

WiFiUDP UDP;
char packet[255];e
int sensorwert = 0;

float xmem=0;
float ymem=0;
float zmem=0;
float xrot=0;
float yrot=0;
float zrot=0;
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
  xmem=a.acceleration.x;        
  ymem=a.acceleration.y;
  zmem=a.acceleration.z;
  xrot=g.gyro.x;              //Rotationswerte mit einbeziehen!
  yrot=g.gyro.y;              //Rotationswert mit einbeziehen!
  zrot=g.gyro.z;              //Rotationswert mit einbeziehen!
}

void loop(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  //Serial.print("Acceleration X: ");
  //Serial.print(a.acceleration.x);
  //Serial.print(", Y: ");
  //Serial.print(a.acceleration.y);
  //Serial.print(", Z: ");
  //Serial.print(a.acceleration.z);
  //Serial.println(" m/s^2");
  //sensorwert=abs(a.acceleration.z)+abs(a.acceleration.x)+abs(a.acceleration.y);
  //Serial.println(sensorwert);
  //Serial.println("");
  //Serial.print("Rotation X: ");
  //Serial.print(g.gyro.x);
  //Serial.print(", Y: ");
  //Serial.print(g.gyro.y);
  //Serial.print(", Z: ");
  //Serial.print(g.gyro.z);
  //Serial.println(" rad/s");

  if((abs(a.acceleration.x/xmem)-1)>0.05){   //Schwellenwert 5%
    motion+=1;                                
    //Serial.println((a.acceleration.x/xmem)-1);
    //Serial.println("");
  }
  if((abs(a.acceleration.y/ymem)-1)>0.05){  //Schwellenwert 5%
    motion+=1;
    //Serial.println((a.acceleration.y/ymem)-1);
    //Serial.println("");
  }
  if((abs(a.acceleration.z/zmem)-1)>0.05){   //Schwellenwert 5%
    motion+=1;
    //Serial.println((a.acceleration.z/zmem)-1);
    //Serial.println("");
  }
  if ((abs(g.gyro.x/xrot)-1)>0.3){   //Rotation Schwellenwert (sehr wichtig, so ist audioausgabe viel stabiler da dadurch gleichmäßige Bewegung erkennt) zwischen 0,3 und 0,5 bei accelerationschwelle von 0,05
    motion+=1;
    //Serial.println((g.gyro.x/xrot)-1);
    //Serial.println("");
  }
  if ((abs(g.gyro.y/yrot)-1)>0.3){          // wenn gyro schwellenwert geringer als 0,3 ist, dann erkennt er schiefes ablegen als 1; 0,5 schon ist obere Grenze, da dann gyrowerte nicht mehr soo viel einfluss haben.
    motion+=1;
    //Serial.println((g.gyro.y/yrot)-1);
    //Serial.println("");
  }
  if ((abs(g.gyro.z/zrot)-1)>0.3){
    motion+=1;
    //Serial.println((g.gyro.z/zrot)-1);
    //Serial.println("");
  }

  if(motion>=1){      // mit größer gleich, wird auch bei berührung einer achse angeschaltet, was sehr wichtig ist! (könnte auch "größer Null" dort stehen), so kann beim schwellenwert ein höherer wert eingestellt werden!
    sensorwert=1;     // 0 oder 1 wird in Sensorwert gespeichert und geschickt
    Serial.println(motion);
  }
  else{
    Serial.println(motion);
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
  xrot=g.gyro.x;
  yrot=g.gyro.y;
  zrot=g.gyro.z;
  delay(10);                        //bringt auch stabilität ins audiosignal da Wert nicht so oft erneu verglichen wird
}
