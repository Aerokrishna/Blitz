#include <Arduino.h>
#include <serial_parser.hpp>
#include <PID.h>
#include <MotionProfile.h>
#include <include_all.cpp>

#define KP 6.0  // radians
#define KI 0.0      // milliseconds
#define KD 0.0           // milliseconds (loop update rate)
#define IMAX 100.0
#define LOOP_DT 20.0

#define TARGET_ANGLE 0  // radians
#define TOTAL_TIME 5      // milliseconds

MotionProfile profile(0.0f, TARGET_ANGLE, TOTAL_TIME); // Time in seconds
PID motor_pid(KP, KI, KD, IMAX); // Time in seconds

float current_point = 0.0;
unsigned long last_update_time = 0;
float elapsed_time = 0.0;

void setup() {
    Serial.begin(115200);
    // while (!Serial); // wait for serial monitor to open
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

    // delay(100);

    // Receive Data
    std::vector<uint8_t> payload = receive_data();
    if (!payload.empty()){
        std::vector<int16_t> values = parse_int(payload.data(), payload.size());

        float newkp = values[0]/100.f;
        float newki = values[1]/100.f;
        float newkd = values[2]/100.f;
        float newtarget = values[3];
        float newtime = values[4]/100.f;

        profile.reset_profile(current_angle, newtarget, newtime);
        motor_pid.update_gains(newkp, newki, newkd);

        elapsed_time = 0.0;
    }
    
    unsigned long current_time = millis();

    if (current_time - last_update_time >= LOOP_DT) {
        last_update_time = current_time;
        float setpoint = profile.get_setpoint(current_time);
        float error = setpoint - current_angle;
        float control = motor_pid.get_pid(error, 0.0, current_time);
        float dir = 1;
        if (control < 0){
            dir = 0;

        }
        elapsed_time += LOOP_DT;
        
        digitalWrite(MD1_DIR, dir);
        analogWrite(MD1_PWM, abs(control));
        
        // Send Data
        int16_t data[] = {(int16_t)(setpoint*100), (int16_t)(current_angle*100), (int16_t)(elapsed_time/10)};
        uint8_t id = 2;
        std::vector<uint8_t> buffer = pack_data(data, 3, id);
        send_data(buffer);
    }

}



