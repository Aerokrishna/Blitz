#define Encoder_output_A 3// green
#define Encoder_output_B 4 // yellow
// these two pins has the hardware interrupts as well. 
 
int enc_count = 0;
int encoder_res = 6;
float encoder_wheel_dia = 0.1; //in meters
float pi = 3.147;

void setup() {
Serial.begin(9600); // activates the serial communication
pinMode(Encoder_output_A,INPUT); // sets the Encoder_output_A pin as the input
pinMode(Encoder_output_B,INPUT); // sets the Encoder_output_B pin as the input
attachInterrupt(digitalPinToInterrupt(Encoder_output_A),DC_Motor_Encoder,RISING);
}
 
void loop() {
  float dis = pi * encoder_wheel_dia * ((enc_count)/encoder_res);
  Serial.print("Result: ");
  Serial.println(enc_count); 
  delay(1000);
}
 
void DC_Motor_Encoder(){
  int b = digitalRead(Encoder_output_B);
  if(b > 0){
    enc_count++;
  }
  else{
    enc_count--;
  }
}