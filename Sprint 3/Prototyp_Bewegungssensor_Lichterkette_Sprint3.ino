#include <FastLED.h> //LED Bibliothek zum Steuern der Lichterkette
#define NUM_LEDS 38 //Anzahl der zu steuernden LEDs
#define DATA_PIN 10 //Steckplatz der Lichterkette
#define UPDATES_PER_SECOND 100 //
int counter;
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
CRGB leds[NUM_LEDS]; //Initialisierung LEDs

#define echoPin1 2 //PIN zuweisung am Arduino
#define trigPin1 3
#define echoPin2 4
#define trigPin2 5
#define echoPin3 7
#define trigPin3 8
 
long duration, distance, RightSensor,FrontSensor,LeftSensor; 


float puffer1; //Benötigter Zwischenspeicher zur Berechnung vom Mittelwert
float puffer2;
float puffer3;
float puffer4;
float puffer5;

void setup() { 
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); //Lichterkette mit Anzahl LED und Art  
  currentPalette = RainbowColors_p; //LEDs sollen in allen Farben leuchten
  currentBlending = LINEARBLEND; //Lineare Interpolation zwischen Farb- und Helligkeitswerten
  Serial.begin (115200); //Serielle kommunikation starten, damit man sich später die Werte am serial monitor ansehen kann.
  pinMode(trigPin1, OUTPUT); //In- und Outputs 
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
}

void loop() {
  
  SonarSensor(trigPin1, echoPin1);  //Hier werden die Sensoren nacheinander aktiviert und die Entfernungen gemessen
  RightSensor = distance;  
  RightSensor = average(RightSensor, puffer2, 5); //Auch hier wird ein Mittelwert über die letzten 5 Werte gebildet

  SonarSensor(trigPin2, echoPin2);
  LeftSensor = distance;
  LeftSensor = average(LeftSensor, puffer3, 5);

  SonarSensor(trigPin3, echoPin3);  
  FrontSensor = distance;
  FrontSensor = average(FrontSensor, puffer4, 5);

  Serial.print(LeftSensor); //Hiermit kann ich mir alle drei gemessenen Entfernungen ausgeben und die Funktionalität überprüfen
  Serial.print(" - ");
  Serial.print(FrontSensor);
  Serial.print(" - ");
  Serial.println(RightSensor);

  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; //Index wird benutzt für Farbänderung
  FillLEDsFromPaletteColors(startIndex, LeftSensor, FrontSensor, RightSensor); //Aufruf der Funktion zur Bestimmung der Farben der einzelnen LEDs. Entfernung ist eigentlich Helligkeit, je nach Entfernung sollen LEDs unterschiedlich hell leuchten
  FastLED.show(); //Ohne diesen befehl leuchtet die Kette nicht
  FastLED.delay(1000 / UPDATES_PER_SECOND); //Kleines Delay
  delay(50); 
}  
    


void FillLEDsFromPaletteColors( uint8_t colorIndex, long entfernung1, long entfernung2, long entfernung3) //In dieser Funktion wird zuerst geguckt welche der 3 übergebenen Entfernungen die Kürzeste ist. Anschließend wird der kürzeste Wert als Helligkeit (0-255) ausgegeben
{   
    int closest;
   
    if (entfernung1 <= entfernung2 && entfernung1 <= entfernung3) {
      closest = entfernung1;
    }
    else if (entfernung2 <= entfernung1 && entfernung2 <= entfernung3) {
      closest = entfernung2;
    }
    else if (entfernung3 <= entfernung2 && entfernung3 <= entfernung1) {
      closest = entfernung3;
    }
    
    int brightness; 
      if (closest >= 255){ //Entfernungen über 2.55m werden als 2.55m behandelt. 255cm entsprechen einer Helligkeit von 0.
        closest = 255;
      }
      Serial.println(closest);
      delay(50);
    
    brightness = 255 - closest; //255 ist die maximale Helligkeit, die kürzeste gemessene Entfernung wird an dieser Stelle subtrahiert. Sollte jemand so genau vor dem Einkaufswagen stehen, leuchtet die Lichterkette in ihrer maximalen Helligkeit
    if (brightness <= 0) {
      brightness = 0; //Es darf keine Werte kleiner gleich null geben
    }
    
    float mwert = average(brightness, puffer1, 10); //Mittelwert über die letzten 10 Helligkeiten
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending); //LEDs werden durchiteriert wobei Regenbogefarben verwendet werden sollen
        colorIndex += 3; //ColorIndex erhöht Farbabstand, sorgt für veränderung der Farben
    }
    
}   

float average(float Rohwert, float &buff, float faktor) { //Diese Funktion bildet einen Mittelwert

  float avg ;
  buff = buff - buff / faktor;
  buff = buff + Rohwert;
  avg = buff / faktor;

  return avg;

}

void SonarSensor(int trigPin,int echoPin) //Funktion zu Berechnung der Entfernung mittels Ultraschall.
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH); //Schallwelle wird losgeschickt
delayMicroseconds(10); //Für eine Dauer von 10 Mikrosekunden
digitalWrite(trigPin, LOW); 
duration = pulseIn(echoPin, HIGH); //Zeit für die Rückkehr der Schallwelle wird gemessen
distance = (duration/2) * 0.03432; //Zeit geteilt durch zwei, mal Faktor für Schallgeschwindigkeit in Luft, ergeben Entfernung in cm

}