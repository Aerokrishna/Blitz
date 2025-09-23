// 3 steps to add new interface
// - include packet id in the PacketID function
// - add the interface as a struct
// - include the packet id in the packet size function

enum PacketID : uint8_t {

    // each interface has an id, which is common between sbc and mcu
    // each interface or data packet is referenced in the microcontroller code using enum names
    // like COUNTER, ODOM

    COUNTER = 1,

};

// add your interface as a struct
#pragma pack(push, 1)
struct Counter {

    // order of these fields must match your ros interface
    uint8_t id;
    int16_t a;
    int16_t b;
    float c;
    float d;

};  
#pragma pack(pop)

size_t get_packet_size(uint8_t id) {
    switch (id) {

        // add this line for each new interface 
        case COUNTER:    return sizeof(Counter);

        default:      return 0; 
   }
}
