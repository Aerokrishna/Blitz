#include <Servo.h>
#include "PinMap.h"

Servo myservo;

void setup() {
  myservo.attach(S6);
  pinMode(MD5_DIR,OUTPUT);
  pinMode(MD5_PWM,OUTPUT);
  // put your setup code here, to run once:
}

void loop() {
  
  // myservo.write(0);
  // delay(5000);
  myservo.write(134);
  delay(1000);
  myservo.write(170);
  delay(1000);
  // analogWrite(MD5_PWM,40);
  // digitalWrite(MD5_DIR,HIGH);
  // delay(6000);
  

}

// Servo 0 40 close 95 open
// servo 1 30 close 85 open
// servo 2 37 close 95 open  L?

// Servo 5 120 close 70 open
// servo 4 112 close 70 open
// servo 3 124 close 85 open
  
