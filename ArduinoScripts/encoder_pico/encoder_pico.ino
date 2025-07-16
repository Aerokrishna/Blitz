//MOTOR D1
// ------------------------------Include Files------------------------------ 

// Standard Libraries
#include <Arduino.h>

int encoderValue = 0;
// int encoderValue2 = 0;


void updateEncoder_(uint gpio, uint32_t events){


  if (digitalRead(10)==1){
    encoderValue++;

  }
  else {
  encoderValue--;}

  // if (gpio == encoderPin2){ 
  //   if (digitalRead(enc_right_B2)==1){
  //   encoderValue2++;

  // }
  // else {
  // encoderValue2--;}
  // }
  // Serial.println("Hi");
}


void setup() {
  Serial.begin (9600);

  pinMode(8, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);

  // digitalWrite(encoderPin, HIGH); 

  // pinMode(encoderPinencoderPin, INPUT_PULLUP);
         
  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin encoderPin), or interrupt 1 (pin 3) 
  // attachInterrupt(digitalPinToInterrupt(encoderPin), updateEncoder, RISING); 
  gpio_set_irq_enabled_with_callback(8, GPIO_IRQ_EDGE_RISE, true, updateEncoder);


  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
}

void loop(){ 
  //Do stuff here

  Serial.print("encoderValue1  ");
  Serial.println(encoderValue);



  // Serial.println(digitalRead(encoderPin));
  // delay(1000); //just here to slow down the output, and show it will work  even during a delay
}
