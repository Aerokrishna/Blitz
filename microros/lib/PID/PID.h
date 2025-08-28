class PID {
  public:
    PID(float KP, float KI, float KD, float IMAX);
    float get_pid(float error, float scalar, unsigned long current_millis);
    void reset_I();
    void update_gains(float newKp, float newKi, float newKd);

  private:
    float kp{0.0f}, ki{0.0f}, kd{0.0f}, imax{0.0f};
    float last_t{0.0f};
    float last_error{0.0f};
    float integrator{0.0f};
};

#define ABS(x) ((x) < 0 ? -(x) : (x))
