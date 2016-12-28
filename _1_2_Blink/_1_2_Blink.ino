void setup() {
  // Set the led pin to output mode
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
  // Wait half a second
  delay(500);
  // Turn pin on
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);
  // Wait half a second
  delay(500);
}