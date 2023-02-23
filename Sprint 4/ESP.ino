#define RXp2 16 //Empfänger Pin
#define TXp2 17 //Sender Pin
void setup() {

  Serial.begin(115200); //Serielle Schnittstelle zum printen
  Serial2.begin(115200, SERIAL_8N1, RXp2, TXp2); //Serielle Schnittstelle zum auslesen des Empfänger Pins
}
void loop() {

    Serial.println(Serial2.readStringUntil('\n')); //Serielle Kommunikation bis Ende der Zeile auslesen und printen
}