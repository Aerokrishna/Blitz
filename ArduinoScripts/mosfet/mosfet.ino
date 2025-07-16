#include "PinMap.h"

int mosfet_pin = Lim4;

void setup() {
  pinMode(mosfet_pin,OUTPUT);


  // put your setup code here, to run once:
}

void loop() {
  
  digitalWrite(mosfet_pin,HIGH);
  delay(1000);
  digitalWrite(mosfet_pin,LOW);
  delay(1000);
}
  
