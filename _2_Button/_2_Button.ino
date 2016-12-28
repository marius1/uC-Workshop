int value = 0;

void setup() {
  // Set the LED pin to output mode
  pinMode(13, OUTPUT);
  // Set the button pin to input mode
  pinMode(8, INPUT);
  // Set interal pull up
  digitalWrite(8, HIGH);
}

void loop() {
  // Read the state of the button
  value = digitalRead(8);
  // Set the LED to the button's value 
  digitalWrite(13, value);
}
