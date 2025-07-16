int button_pin = 17;


//function called everytime button is pressed
void button_callback(){
  Serial.println("Button pressed");
  
}

void setup() {
 
  pinMode(button_pin, INPUT_PULLDOWN);
  //attachInterrupt(digitalPinToInterrupt(button_pin), button_callback, RISING); 

  //start serial monitor
  Serial.begin(9600);
  // put your setup code here, to run once:
}
void loop(){
  Serial.println(digitalRead(button_pin));
  
}