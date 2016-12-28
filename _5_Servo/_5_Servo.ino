#include <Servo.h>

Servo myServo;

int pos;

void setup() {
  myServo.attach(11);
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) {     
    myServo.write(pos);
    delay(15);
  }
  for (pos = 180; pos >= 0; pos -= 1) {
    myServo.write(pos);  
    delay(15); 
  }

}

