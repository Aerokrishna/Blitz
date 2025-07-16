
#include "PinMap.h"
#define BUTTON_PIN Lim1

void button_callback(){
  Serial.println("Button pressed");
}

void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), button_callback, RISING); 

}

void loop()
{
  Serial.println("Button not pressed");
}
