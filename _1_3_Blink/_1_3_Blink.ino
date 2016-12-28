void setup() {
  // Set the led pin to output mode
  pinMode(11, OUTPUT);
}

int i;
void loop() {

  for(i=0; i <= 255; i++) {
    analogWrite(11, i);
    delay(10);
  }

  for(i=255; i >= 0; i--) {
    analogWrite(11, i);
    delay(10);
  }
}
