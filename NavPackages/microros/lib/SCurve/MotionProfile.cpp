#include "MotionProfile.h"
#include <algorithm>
#include <cmath>
#include <Arduino.h>

MotionProfile::MotionProfile(float current, float target, float time) {
    total_time = time;
    target_point = target;
    current_point = 0.0;
    // steps = static_cast<int>(total_time/dt_);

    // Compute max velocity and time phases
    max_vel = (target_point / total_time);
    max_vel = std::min(max_vel, max_acc * (total_time));

    t_acc = max_vel / max_acc;
    t_consv = total_time - 2 * t_acc;

    if (t_consv < 0) {
        t_acc = std::sqrt(target_point / max_acc);
        t_consv = 0.0f;
        max_vel = max_acc * t_acc;
        total_time = 2 * t_acc;
    }

    prev_time = 0;
    elapsed_time = 0;
    curr_vel = 0;
    
}

float MotionProfile::get_setpoint(unsigned long current_time) {
    

    // Serial.print("SETPOINT ");
    // Serial.println(current_point);

    float dt = static_cast<float>(current_time - prev_time)/ 1000.f; // convert it into dt in seconds
    prev_time = current_time;

    // float time_step = total_time / steps;
    elapsed_time += dt; // elapsed time in seconds

    if (elapsed_time < t_acc) {
        // PHASE 1
        curr_vel += max_acc * dt;
        current_point += curr_vel * dt;   

    } 
    else if (elapsed_time < (t_acc + t_consv)) {
        // PHASE 2
        Serial.print("PHASE 2  ");
        Serial.println(current_point);

        curr_vel = max_vel;
        current_point += max_vel * dt;
        
    } 
    else if (elapsed_time < (2 * t_acc + t_consv)) {
        // PHASE 3
        Serial.print("PHASE 3 ");
        Serial.println(current_point);

        curr_vel = curr_vel - max_acc * dt;
        current_point += curr_vel * dt;

    } 
    else {
        curr_vel = 0; // hold final value
        current_point = target_point;

    }

    return current_point;
}
