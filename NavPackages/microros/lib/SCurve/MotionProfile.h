class MotionProfile {
    public:
        MotionProfile(float current, float target, float time);
        float get_setpoint(unsigned long current_time);

    private:
        float target_point{0.0f}, total_time{0.0f}; 
        float current_point{0.0f};
        float curr_vel{0.0f};
        float max_acc{1.54f};
        float prev_time{0.0f};
        int steps{20};
        float elapsed_time{0.0f};
        float t_acc{0.0f}; float t_consv{0.0f}; float max_vel{0.0f};

};
