#include <array>

class Odom {
  public:
    Odom(float L, float B, float meter_per_tick); 
    // L perpendicular distance of x wheel encoder from the center
    // B perpendicular distance of y wheel encoder from the center
    std::array<float, 6> get_odom(float encx, float ency, float current_yaw, unsigned long current_millis);

  private:
    float l{0.0f}, b{0.0f}, m_per_tick{0.0f};
    float prev_t{0.0f};
    float prev_yaw{0.0f};
    std::array<float, 6> odometry = {0.0f, 0.0f, 0.0f, 
                                    0.0f, 0.0f, 0.0f}; // x, y, theta, vx. vy. vtheta

};