#include <iostream>
#include <vector>
#include <cstring>  // for std::memcpy
#include <stdexcept>
#include "include_all.cpp"
#include "serial_parser.hpp"
CmdVel cmdvel;
Odometry odom;

void setup(){
    Serial.begin(115200);
    // while (!Serial); // wait for serial monitor to open
}
void loop() {
    std::vector<uint8_t> payload = receive_data();

    if (!payload.empty()) {
        uint8_t id = payload[0];

        if (id == CMD_VEL) {
            cmdvel = parse_struct<CmdVel>(payload);
            std::vector<uint8_t> buffer = pack_data<CmdVel>(cmdvel);
            send_data(buffer);
        }
    }
    odom.id = 2;
    odom.x = 4.12;
    std::vector<uint8_t> buffer_o = pack_data<Odometry>(odom);
    send_data(buffer_o);
}

