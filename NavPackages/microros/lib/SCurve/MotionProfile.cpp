#include "MotionProfile.h"
#include <algorithm>
#include <cmath>
#include <Arduino.h>

MotionProfile::MotionProfile(float current, float target, float time) {
    total_time = time;
    target_point = target;
    current_point = current;
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

    // float time_step = total_time / steps;
    prev_time = current_time;
    
    elapsed_time = elapsed_time + dt;

    if (elapsed_time < t_acc) {
        // PHASE 1
        curr_vel += max_acc * dt;
        current_point += curr_vel * dt;   

    } 
    else if (elapsed_time < (t_acc + t_consv)) {
        // PHASE 2
        // Serial.print("PHASE 2  ");
        // Serial.println(current_point);

        curr_vel = max_vel;
        current_point += max_vel * dt;
        
    } 

    else if (elapsed_time < (2 * t_acc + t_consv)) {
        // PHASE 3
        // Serial.print("PHASE 3 ");
        // Serial.println(current_point);

        curr_vel = curr_vel - max_acc * dt;
        current_point += curr_vel * dt;

    } 

    else {
        curr_vel = 0; // hold final value
        // current_point = target_point;
    }

    Serial.print(elapsed_time);
    Serial.print(" prevtime  ");
    Serial.print(prev_time);
    Serial.print(" setpoint  ");
    Serial.print(current_point);
    Serial.print("target ");
    Serial.print(target_point);
    Serial.print("curr vel ");
    Serial.println(curr_vel);
    Serial.print("total time ");
    Serial.println(total_time);
    
    return current_point;
}

void MotionProfile :: reset_profile(float current, float target, float time){
    // resets all the variables for the motion profile
    // prev_time = 0.0f;
    elapsed_time = 0.0f;
    curr_vel = 0.0f;
    current_point = current; // needs to be replaced with correct current_point with an argument
    target_point = target;
    total_time = time;

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
}