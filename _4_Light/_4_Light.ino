void setup() {  
  pinMode(A0, INPUT);
  Serial.begin(9600);  
}

int value;

void loop() {
  // Get value, 0-1023
  value = analogRead(A0);
  Serial.println(value);
  delay(100);
}
