#include <FastLED.h> //LED Bibliothek zum Steuern der Lichterkette
#define NUM_LEDS 10 //Anzahl der zu steuernden LEDs
#define DATA_PIN 10 //Steckplatz der Lichterkette
#define UPDATES_PER_SECOND 100 //

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
CRGB leds[NUM_LEDS]; //Initialisierung LEDs

#define echoPin1 3
#define trigPin1 2


long duration, distance, Sensor;


void setup() { 
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); //Lichterkette mit Anzahl LED und Art  
  currentPalette = RainbowColors_p; //LEDs sollen in Regenbogenfarben leuchten
  currentBlending = LINEARBLEND; //Lineare Interpolation zwischen Farb- und Helligkeitswerten
  Serial.begin (115200); //Serielle kommunikation starten, damit man sich die Werte am serial monitor ansehen kann.
  pinMode(trigPin1, OUTPUT); //Sendepuls des Sensors = Output
  pinMode(echoPin1, INPUT); //Schallempfänger des Sensors = Input
  
}

void loop() {
  
  SonarSensor(trigPin1, echoPin1);  //Hier wird die Funktion zu Berechnung der Entfernung aufgerufen
  Sensor = distance;  

  if (Sensor >= 255){ //Gemessene Entfernung wird auf 255 begrenzt, da der Helligkeitsbereich der Lichterkette mit 8-Bit codiert ist
    Sensor = 255;
  }

  
  Serial.print(Sensor);
  Serial.print(" cm"); //Serielle Ausgabe der Entfernung
  
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; //Index wird benutzt für Farbänderung
  

  FillLEDsFromPaletteColors(startIndex, Sensor); //Aufruf der Funktion zur Bestimmung der Farben der einzelnen LEDs. Entfernung ist eigentlich Helligkeit, je nach Entfernung sollen LEDs unterschiedlich hell leuchten
  FastLED.show(); //Ohne diesen befehl leuctet die Kette nicht
  FastLED.delay(1000 / UPDATES_PER_SECOND); //Kleines Delay
  delay(50); 
}  
    


void FillLEDsFromPaletteColors( uint8_t colorIndex, long entfernung) //Funktion aus der FastLED Bibliothek mit der sich die Farben der einzelnen LEDs einstellen lassen
{   
    int brightness;
    brightness = BrightnessFromDistance(entfernung); //S. Zeile 58
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending); //LEDs werden durchiteriert wobei Regenbogefarben verwendet werden sollen
        colorIndex += 3; //ColorIndex erhöht Farbabstand
    }
    
}

int BrightnessFromDistance (int val){ //Diese Funktion weißt den verschiedenen  Entfernungsbereichen eine bestimmte Helligkeit zu. Nah -> hell, entfernt -> dunkel.
  
  int brightness;
  
  if (val <= 50) {
      brightness = 255;
      //Serial.println("255");
      return brightness;
    }     

    else if (val > 50 && val <= 100) {
      brightness = 140;
      //Serial.println("140");
      return brightness;
    } 

    else if (val > 100 && val <= 150) {
      brightness = 80 ;
      //Serial.println("150");
      return brightness;
    }     

    else if (val > 150 && val <= 200) {
      brightness = 20 ;
      //Serial.println("20");
      return brightness;
    }  

    else if (val > 200 && val <= 255) {
      brightness = 0;
      return brightness;
    } 

}

void SonarSensor(int trigPin, int echoPin) //Funktion zu Berechnung der Entfernung mittels Ultraschall.
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = (duration/2) * 0.03432; //Formel rechnet gemessene Zeit in Entfernung in cm um. Entfernung wird anschließend in einer globalen Variable gespeichert

}