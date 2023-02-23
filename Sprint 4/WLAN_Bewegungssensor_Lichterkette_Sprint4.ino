#include <FastLED.h> //LED Bibliothek zum Steuern der Lichterkette
#define NUM_LEDS 38 //Anzahl der zu steuernden LEDs
#define DATA_PIN 10 //Steckplatz der Lichterkette
#define UPDATES_PER_SECOND 100 //
int counter;
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
CRGB leds[NUM_LEDS]; //Initialisierung LEDs
#define echoPin1 2  //PIN zuweisung am Arduino
#define trigPin1 3
#define echoPin2 4
#define trigPin2 5
#define echoPin3 7
#define trigPin3 8

const uint8_t PROGMEM gamma8[256] = {                                     //Ein LUT der die 256 Helligkeitsstufen der Lichterkette an die Gamma Kurve des Auges anpasst.  
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

long duration, distance, RightSensor,FrontSensor,LeftSensor; 


float puffer1;  //Benötigter Zwischenspeicher zur Berechnung vom Mittelwert
float puffer2;
float puffer3;
float puffer4;
float puffer5;

void setup() { 
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); //Lichterkette mit Anzahl LED und Art  
  currentPalette = RainbowColors_p; //LEDs sollen in allen Farben leuchten
  currentBlending = LINEARBLEND; //Lineare Interpolation zwischen Farb- und Helligkeitswerten
  Serial.begin (115200); //Serielle kommunikation starten, damit man sich später die Werte am serial monitor ansehen kann.
  pinMode(trigPin1, OUTPUT);  //In- und Outputs 
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

  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; //Index wird benutzt für Farbänderung
  float entfernung = 0;
  FillLEDsFromPaletteColors(startIndex, LeftSensor, FrontSensor, RightSensor); //Aufruf der Funktion zur Bestimmung der Farben der einzelnen LEDs. Entfernung ist eigentlich Helligkeit, je nach Entfernung sollen LEDs unterschiedlich hell leuchten
  FastLED.show(); //Ohne diesen befehl leuctet die Kette nicht
  FastLED.delay(1000 / UPDATES_PER_SECOND); //Kleines Delay
  delay(50); //Das delay von einer Sekunde sorgt in ca. jeder neuen Sekunde für einen neuen Messwert.
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
          
    brightness = pgm_read_byte(&gamma8[(byte)mwert]); //Gemessene Entfernung wird mithilfe des LUT an die Gamma Kurve des Auges angepass, Entfernung 32cm = 32 Eintrag in der Liste = 1
    
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