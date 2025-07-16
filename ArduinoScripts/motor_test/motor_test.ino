
// fowd left (1-fwd)
// fowd right (0-fwd)
// back left change wire pwm - 21 dir - 20 1-fwd
// back right change wire pwm - 19 dir - 18 0-fwd
// platform  down - 1

#define PWM_ARC 2
#define ARC_DIR_REVERSE 3
#define ARC_DIR_FORWARD 6

// int pwm_pin2 = 3;
// int dir2 = 2;
// int dir1 = 23;

// int led_pin = 25;

void setup() {
  // for (int i=0;i<4;i++){
  //   pinMode(pwm[i],OUTPUT);
  //   pinMode(dir[i],OUTPUT);
  // }
  pinMode(PWM_ARC,OUTPUT);
  pinMode(ARC_DIR_REVERSE,OUTPUT);
  pinMode(ARC_DIR_FORWARD,OUTPUT);
  // pinMode(dirB,OUTPUT);
  Serial.begin(9600);

  // put your setup code here, to run once:
  // analogWrite(0, 500);
}

void loop() {

// analogWrite(pwm_pin,120); // pin analog val
// digitalWrite(dir, 0);

analogWrite(PWM_ARC,150); // pin analog val
digitalWrite(ARC_DIR_REVERSE, 0);
digitalWrite(ARC_DIR_FORWARD, 1);

  // put your main code here, to run repeatedly:

}
