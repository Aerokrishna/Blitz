#include "PinMap.h"
int IN1 = Lim1;
int IN2 = Lim2;
int IN3 = Lim3;
int IN4 = Lim4;

void setup() {
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);

  // put your setup code here, to run once:

}

void loop() {
  // digitalWrite(IN1,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN4,HIGH);
  delay(1000);
  // digitalWrite(IN3,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN4,LOW);
  delay(1000);
  // put your main code here, to run repeatedly:

}
