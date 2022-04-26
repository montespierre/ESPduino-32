// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(2, OUTPUT);
}


void loop() {
  digitalWrite(2, HIGH);   
  delay(1000);                       
  digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       
}
