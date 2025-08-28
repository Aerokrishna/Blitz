#include "Odom.h"

Odom::Odom(float L, float B, float meter_per_tick) {
    l = L;
    b = B;
    m_per_tick = meter_per_tick;
}
// pass encx and ency as the number of ticks, current yaw of the robot, and current time in milliseconds
std::array<float, 6> Odom::get_odom(float encx, float ency, float current_yaw, unsigned long current_millis) {
    unsigned long t_now = current_millis;
    unsigned long dt = t_now - prev_t;

    float avg_theta = (current_yaw + prev_yaw) / 2.0;
    float dtheta = prev_yaw - current_yaw;

    // calculate distance moved in x and y directions with respect to robot frame
    float dx = m_per_tick * encx + (l * sin(dtheta));
    float dy = m_per_tick * ency - (b * sin(dtheta));

    // transform dx and dy to global frame
    float gloabl_dx = (dx * cos(avg_theta)) - (dy * sin(avg_theta));
    float global_dy = (dx * sin(avg_theta)) + (dy * cos(avg_theta));

	// update x y theta vx vy vtheta
    odometry[0] += gloabl_dx;  // x
    odometry[1] += gloabl_dx;  // y
    odometry[2] = current_yaw; // yaw
    odometry[3] = (gloabl_dx / (dt * 0.001f)); // vx
    odometry[4] = (global_dy / (dt * 0.001f)); // vy
    odometry[5] = (dtheta / (dt * 0.001f)); // vtheta

    prev_yaw = current_yaw;
    prev_t = t_now;
	encx = 0;
    ency = 0;

    return odometry;
}