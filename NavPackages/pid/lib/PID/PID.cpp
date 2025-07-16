#include "PID.h"

PID::PID(float KP, float KI, float KD, float IMAX) {
    kp = KP;
    ki = KI;
    kd = KD;
    imax = IMAX;
}

float PID::get_pid(float error, float scalar, unsigned long current_millis) {
    unsigned long t_now = current_millis;
    unsigned long dt = t_now - last_t;
    float output = 0.0f;

    if ((last_t == 0) || (dt > 1000)) {
        dt = 0.0f;
        integrator = 0.0f;
    }

    last_t = t_now; 
    float delta_time = static_cast<float>(dt) * 0.001f;
    output += (error * kp);

    if ((ABS(kd) > 0) && (dt > 0)) {
        float derivative = (error - last_error / delta_time);

        last_error = error;
        output += (kd * derivative);
    }

    output += scalar;

    if ((ABS(ki) > 0) && (dt > 0)) {
        if (ABS(output) < imax) {
            integrator += (error * ki) * scalar * delta_time;
            if (ABS(integrator) > imax && integrator != 0.0f) {
                integrator = (integrator / ABS(integrator) * imax);
            }
            output += integrator;
        }
    }

    return output;
}

void PID::reset_I() { integrator = 0.0f; }
