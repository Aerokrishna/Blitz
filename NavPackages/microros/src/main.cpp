#include <Arduino.h>
#include <PID.h>
#include <MotionProfile.h>
#include <include_all.cpp>

#define KP 1.5  // radians
#define KI 0.08      // milliseconds
#define KD 0.0           // milliseconds (loop update rate)
#define IMAX 100.0
#define LOOP_DT 20.0

#define TARGET_ANGLE 90  // radians
#define TOTAL_TIME 5      // milliseconds

MotionProfile profile(0.0f, TARGET_ANGLE, TOTAL_TIME); // Time in seconds
PID motor_pid(KP, KI, KD, IMAX); // Time in seconds

float current_point = 0.0;
unsigned long last_update_time = 0;

void setup() {
    Serial.begin(115200);
    while (!Serial); // wait for serial monitor to open
    // Serial.println("Starting Motion Profile Test...");

    pinMode(MD1_DIR, OUTPUT);
    pinMode(MD1_PWM, OUTPUT);

    pinMode(enc_left_A, INPUT_PULLUP);
    pinMode(enc_left_B, INPUT_PULLUP);
    
    // gpio_set_irq_enabled_with_callback(enc_right_A, GPIO_IRQ_EDGE_RISE, true, updateEncoder);
    gpio_set_irq_enabled_with_callback(enc_left_A, GPIO_IRQ_EDGE_RISE, true, updateEncoder);
    // attachInterrupt(digitalPinToInterrupt(enc_left_A),updateEncoder,RISING);
}

void loop() {
    
    // digitalWrite(MD1_DIR, 1);
    // analogWrite(MD1_PWM, 50);
    // // // delay(100);
    // // // Serial.print(MD1_DIR);

    Serial.print(" CURRENT ANGLE ");
    Serial.println(current_angle);
    unsigned long current_time = millis();

    if (current_time - last_update_time >= LOOP_DT) {
        last_update_time = current_time;
        float setpoint = profile.get_setpoint(current_time);
        float error = setpoint - current_angle;
        float control = motor_pid.get_pid(error, 0.0, current_time);
        Serial.print(" CONTROL: ");
        Serial.print(control, 2);  // print with 6 decimal places
        Serial.print(" SETPOINT: ");
        Serial.println(setpoint, 2);  // print with 6 decimal places
        float dir = 1;
        if (control < 0){dir = 0;}
        
        digitalWrite(MD1_DIR, dir);
        analogWrite(MD1_PWM, control);

    }

    if (Serial.available() > 0) {

    String received = Serial.readStringUntil('\n');
    received.trim();  // Remove any leading/trailing whitespace

    // Split the string using space as delimiter
    int firstSpace = received.indexOf(' ');
    int secondSpace = received.indexOf(' ', firstSpace + 1);

    String kp_str = received.substring(0, firstSpace);
    String ki_str = received.substring(firstSpace + 1, secondSpace);
    String kd_str = received.substring(secondSpace + 1);

    float Kp = kp_str.toFloat();
    float Ki = ki_str.toFloat();
    float Kd = kd_str.toFloat();
    Serial.print("Received PID -> Kp: ");
    Serial.print(Kp);
    Serial.print(", Ki: ");
    Serial.print(Ki);
    Serial.print(", Kd: ");
    Serial.println(Kd);
    
    profile.reset_profile(0.0f, 5.00, 30.0);
    motor_pid.update_gains(Kp, Ki, Kd);

}
}
