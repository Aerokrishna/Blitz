int relay_pin = 3; //relay 1

void setup() {
  pinMode(relay_pin,OUTPUT);

  // put your setup code here, to run once:
}

void loop() {
  digitalWrite(relay_pin, HIGH);
  delay(1000);
  digitalWrite(relay_pin, LOW);
  delay(1000);

  // put your main code here, to run repeatedly:
 
\
}
