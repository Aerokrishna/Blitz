float positionVal;
float readVal;

//Line Sensor init
const byte rx = 21;                  // Defining pin 0 as Rx
const byte tx = 22;                  // Defining pin 1 as Tx
const byte analogpin = A6;            // Connect UART output enable of LSA08 to pin 2


void setup() {
  
  pinMode(junctionPulse,INPUT);   

 
  Serial.begin(9600);
  // put your setup code here, to run once:

}

void loop() {
  readVal = analogRead(analogpin);    // Read value from analog pin

  
  positionVal = ((float)readVal/921)*70;

  Serial.println(positionVal);
  delay(100);
  // put your main code here, to run repeatedly:

}
