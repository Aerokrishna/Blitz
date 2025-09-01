enum PacketID : uint8_t {
    CMD_VEL = 1,
    ODOM = 2,
    PWM_ = 3
};

#pragma pack(push, 1)
struct Odometry{
    uint8_t id;
    float x;
    float y;
    float yaw;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct CmdVel {
    uint8_t id;
    float vx;
    float vy;
    float vyaw;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct PWM {
    uint8_t id;
    int16_t pwm;

};  
#pragma pack(pop)

size_t get_packet_size(uint8_t id) {
    switch (id) {
        case CMD_VEL: return sizeof(CmdVel);
        case ODOM:    return sizeof(Odometry);
        case PWM_:    return sizeof(PWM);

        default:      return 0; // unknown
    }
}
