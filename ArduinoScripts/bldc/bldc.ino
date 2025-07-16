#include <Servo.h>

Servo bldc;
void setup() {

  bldc.attach(5);
  bldc.writeMicroseconds(1000);

  pinMode(4, OUTPUT);
  digitalWrite(4, 0);
  delay(5000);
  // put your setup code here, to run once:

}

void loop() {
  bldc.writeMicroseconds(1200);
  delay(2000);
  // put your main code here, to run repeatedly:

}