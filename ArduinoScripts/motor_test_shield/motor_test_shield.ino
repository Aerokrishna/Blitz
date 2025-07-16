#include <AFMotor.h>

AF_DCMotor rmotor(3);
AF_DCMotor lmotor(4);

void setup() 
{
	//Set initial speed of the motor & stop
	rmotor.setSpeed(255);
	rmotor.run(RELEASE);
  lmotor.setSpeed(255);
	lmotor.run(RELEASE);
  Serial.begin(9600);
}

void loop() 
{ 

	// Turn on motor
	rmotor.run(FORWARD);
  lmotor.run(FORWARD);
  Serial.println(8);
   }