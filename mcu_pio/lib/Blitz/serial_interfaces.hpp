enum PacketID : uint8_t {
    COUNTER = 1,
};

#pragma pack(push, 1)
struct Counter {
    uint8_t id;
    int16_t a;
    int16_t b;
    float c;
    float d;

};  
#pragma pack(pop)

size_t get_packet_size(uint8_t id) {
    switch (id) {
        case COUNTER:    return sizeof(Counter);

        default:      return 0; // unknown
    }
}
