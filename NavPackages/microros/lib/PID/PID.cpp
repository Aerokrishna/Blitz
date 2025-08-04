#include "PID.h"
#include "Arduino.h"
// constructor to initialize pid gians
PID::PID(float KP, float KI, float KD, float IMAX) {
    kp = KP;
    ki = KI;
    kd = KD;
    imax = IMAX; // to prevent integral windup
}
// pass the error, a scalar value to scale the output, and the current time in milliseconds
float PID::get_pid(float error, float scalar, unsigned long current_millis) {
    unsigned long t_now = current_millis;
    unsigned long dt = t_now - last_t;
    float output = 0.0f;

    // if it is the start of the loop or if the dt is too large, reset the integrator
    if ((last_t == 0) || (dt > 1000)) {
        dt = 0.0f;
        integrator = 0.0f;
    }
    last_t = t_now; 

    // cast it as float for calculations
    float delta_time = static_cast<float>(dt) * 0.001f;

    // P component
    output += (error * kp);

    // D component
    if ((ABS(kd) > 0) && (dt > 0)) {
        float derivative = (error - last_error / delta_time);

        last_error = error;
        output += (kd * derivative);
    }

    // output += scalar;

    // I component
    if ((ABS(ki) > 0) && (dt > 0)) {
        if (ABS(output) < imax) {
            // integrator += (error * ki) * scalar * delta_time;
            integrator += (error * ki) * delta_time;
            
            // if integrator exceeds imax, limit it to imax
            if (ABS(integrator) > imax && integrator != 0.0f) {
                integrator = (integrator / ABS(integrator) * imax); // preserve sign
            }
            output += integrator;
        }
    }
    // Serial.println(integrator);
    return output;
}
void PID :: update_gains(float newKp, float newKi, float newKd) {
    kp = newKp;
    ki = newKi;
    kd = newKd;
    imax = 0.0f;
}

void PID::reset_I() { integrator = 0.0f; }
