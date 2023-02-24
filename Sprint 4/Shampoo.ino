#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(25, 26);                //25 connected to Rx, 26 connected to Tx in SD reader
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

int sensorwert = 0;
int sensorwert255 = 0;
int photoD = 33;                                         //Pin der Photodiode
bool isPlaying = false;

void setup() {
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3 module
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(10);                                
                                  
  pinMode(photoD, INPUT);                                
}

void loop(){
  sensorwert = analogRead(photoD);                       
  sensorwert255 = map(sensorwert, 0, 4096, 0, 255);     //Sensorwert nach 8 Bit mappen
                                                                                                                                                                                                                                                                             

  if (sensorwert255 > 100) {                             //Audio spielt wenn es hell ist
    if (!isPlaying) {
      myDFPlayer.play(1);
      isPlaying = true;
    }
  }
  if (sensorwert255 < 100) {                            //Audio stoppt wenn es dunkel ist
    if (isPlaying) {
      myDFPlayer.stop();
      isPlaying = false;
    }
  }
  
  delay(500);                                           
}