#include "MotionProfile.h"
#include <algorithm>
#include <cmath>
#include <Arduino.h>

// Constructor
MotionProfile::MotionProfile(float current, float target, float time) {
    reset_profile(current, target, time); // call shared logic
    prev_time = 0;
}

// Public method to get the next position setpoint
float MotionProfile::get_setpoint(unsigned long current_time) {
    float dt = 0;
    if (prev_time != 0) {
        dt = static_cast<float>(current_time - prev_time) / 1000.f; // convert ms to seconds
    }

    prev_time = current_time;
    elapsed_time += dt;

    if (elapsed_time < t_acc) {
        curr_vel += max_acc * dt;
        current_point += curr_vel * dt;
    } 
    else if (elapsed_time < (t_acc + t_consv)) {
        curr_vel = max_vel;
        current_point += curr_vel * dt;
    } 
    else if (elapsed_time < (2 * t_acc + t_consv)) {
        curr_vel -= max_acc * dt;
        current_point += curr_vel * dt;
    } 
    else {
        curr_vel = 0;
        current_point = target_point;
    }

    return current_point;
}

// Public method to reset the motion profile
void MotionProfile::reset_profile(float current, float target, float time) {
    current_point = current;
    target_point = target;
    total_time = time;
    elapsed_time = 0.0f;
    curr_vel = 0.0f;
    compute_profile(current, target, time); // shared logic
}

// Private helper method to compute motion profile parameters
void MotionProfile::compute_profile(float current, float target, float time) {
    float displacement = target - current;
    float direction = (displacement >= 0) ? 1.0f : -1.0f;
    float s = fabs(displacement);
    float a = max_acc; // always positive
    float T = time;

    float discriminant = T*T - (4 * s / a);
    if (discriminant < 0) {
        t_acc = sqrt(s / a);
        max_vel = a * t_acc;
        t_consv = 0;
        total_time = 2 * t_acc;
    } else {
        t_acc = (T - sqrt(discriminant)) / 2;
        max_vel = a * t_acc;
        t_consv = T - 2 * t_acc;
    }

    max_vel *= direction;
    max_acc = a * direction;
}
