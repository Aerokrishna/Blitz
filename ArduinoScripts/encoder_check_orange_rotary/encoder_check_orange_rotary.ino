#include "PinMap.h"

//these pins can not be changed 2/3 are special pins
int encoderPin1 = 3; //white
int encoderPin2 = 4; //green
volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastencoderValue = 0;

void updateEncoder(){
  if (digitalRead(encoderPin2)==1){
    encoderValue++;
  }
  else {
  encoderValue--;}

}
void setup() {
  Serial.begin (9600);

  pinMode(2, INPUT_PULLUP); 
  pinMode(encoderPin2, INPUT_PULLUP);

  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
  attachInterrupt(digitalPinToInterrupt(2), updateEncoder, RISING); 


}

void loop(){ 
  //Do stuff here
  Serial.println(encoderValue);
  // delay(100); //just here to slow down the output, and show it will work  even during a delay
}




// //MOTOR DRIVER TESTER CODE -- OF COURSE ITS ME. WHO ELSE DO YOU THINK IS THAT GOOD BRO ?
// #define DIR 0
// #define PWM 1


// void setup()
// {
//   pinMode(DIR,OUTPUT);
//   pinMode(PWM,OUTPUT);
//   analogWriteResolution(8);
// }

// void loop()
// {
//   digitalWrite(DIR,1);
//   analogWrite(PWM,125);
//   delay(2500);
//   digitalWrite(DIR,0);
//   analogWrite(PWM,125);
//   delay(2500);
// }