#include <iostream>
#include <vector>
#include <cstring>  // for std::memcpy
#include <stdexcept>
#include "include_all.cpp"
#include "serial_parser.hpp"


void setup(){
    Serial.begin(115200);
    while (!Serial); // wait for serial monitor to open
}
void loop() {
    // static InterfaceID interface = -1;
    // Example: serialize struct
    std::vector<uint8_t> payload = receive_data();
    uint8_t id = payload[0];

    if (!payload.empty()){
        Serial.println(id);

        if (id == CMD_VEL){
            CmdVel cmdvel = parse_struct<CmdVel>(payload);
            
            Serial.print(" vx : ");
            Serial.print(cmdvel.vx);
            Serial.print(" vy : ");
            Serial.print(cmdvel.vy);
            Serial.print(" vyaw : ");
            Serial.println(cmdvel.vyaw);
        }

        // if (id == ODOM){
        //     Odometry odom = parse_struct<Odometry>(payload);
            
        //     Serial.print(" x : ");
        //     Serial.print(odom.x);
        //     Serial.print(" y : ");
        //     Serial.print(odom.y);
        //     Serial.print(" theta : ");
        //     Serial.println(odom.yaw);
        // }
    }
    // delay(1);
}
