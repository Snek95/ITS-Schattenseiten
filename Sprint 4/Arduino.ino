unsigned long count; //Zähler 

void setup() {
  Serial.begin(115200); //Serielle Ausgabe starten
}
void loop() {
  Serial.print(count++); //Zähler wird +1 addiert und geprintet
  delay(50);
}