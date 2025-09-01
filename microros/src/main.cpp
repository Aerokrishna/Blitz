#include <iostream>
#include <vector>
#include <cstring>  // for std::memcpy
#include <stdexcept>
#include "include_all.cpp"
#include "serial_parser.hpp"

PWM motor_pwm;
PWM send_pwm;
unsigned long t_prev = millis();

void setup(){
    Serial.begin(115200);
    pinMode(MD1_PWM, OUTPUT);
    pinMode(MD1_DIR, OUTPUT);

    // while (!Serial); // wait for serial monitor to open
}
void loop() {
    std::vector<uint8_t> payload = receive_data();
    unsigned long t_now = millis();
    if (!payload.empty()) {
        uint8_t id = payload[0];

        if (id == PWM_) {
            motor_pwm = parse_struct<PWM>(payload);
            
            // if (motor_pwm.pwm < 0){
            //     digitalWrite(MD1_DIR, 0);
            //     analogWrite(MD1_PWM, abs(motor_pwm.pwm));
            // }
            // else {
            //     digitalWrite(MD1_DIR, 1);
            //     analogWrite(MD1_PWM, abs(motor_pwm.pwm));
            // }
            // send_pwm.pwm = int(t_now-t_prev);
            // send_pwm.pwm = motor_pwm.pwm;
            // send_pwm.id = 3;

            send_data(pack_data<PWM>(motor_pwm));
            t_prev = t_now;
        }
    }

}
