#include "MotionProfile.h"
#include <algorithm>
#include <cmath>
#include <Arduino.h>

MotionProfile::MotionProfile(float current, float target, float time) {
    total_time = time;
    target_point = target;
    current_point = current;
    
    float a = max_acc;
    float T = total_time;
    float s = target_point;

    float discriminant = 25 - (4 * s / a);
    if (discriminant < 0) {
        // Switch to triangular profile
        t_acc = sqrt(s / a);
        max_vel = a * t_acc;
        t_consv = 0;
        total_time = 2 * t_acc;
    } else {
        t_acc = (T - sqrt(T * T - (4 * s / a))) / 2;
        max_vel = a * t_acc;
        t_consv = T - 2 * t_acc;
    }

    prev_time = 0;
    elapsed_time = 0;
    curr_vel = 0;
    
}

float MotionProfile::get_setpoint(unsigned long current_time) {
    
    float dt = 0;
    if (prev_time != 0){
        dt = static_cast<float>(current_time - prev_time)/ 1000.f; // convert it into dt in seconds
    }

    // float time_step = total_time / steps;
    prev_time = current_time;
    elapsed_time = elapsed_time + dt;
    
    if (elapsed_time < t_acc) {
        curr_vel += max_acc * dt;
        current_point += curr_vel * dt;
        
    } 
    else if (elapsed_time < (t_acc + t_consv)) {
        // curr_vel = max_vel;
        // current_point += max_vel * dt;
        current_point += curr_vel * dt;
       
    } 

    else if (elapsed_time < (2 * t_acc + t_consv)) {
        curr_vel = curr_vel - max_acc * dt;
        current_point += curr_vel * dt;
       
    } 

    else {
        curr_vel = 0; // hold final value
        current_point = target_point;
     
    }

    return current_point;
}

void MotionProfile :: reset_profile(float current, float target, float time){
    // resets all the variables for the motion profile
    elapsed_time = 0.0f;
    curr_vel = 0.0f;
    total_time = time;
    target_point = target;
    current_point = current;
    
    float a = max_acc;
    float T = total_time;
    float s = target_point;

    float discriminant = 25 - (4 * s / a);
    if (discriminant < 0) {
        // Switch to triangular profile
        t_acc = sqrt(s / a);
        max_vel = a * t_acc;
        t_consv = 0;
        total_time = 2 * t_acc;
    } else {
        t_acc = (T - sqrt(T * T - (4 * s / a))) / 2;
        max_vel = a * t_acc;
        t_consv = T - 2 * t_acc;
    }
}