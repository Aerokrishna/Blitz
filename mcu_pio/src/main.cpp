#include <iostream>
#include <vector>
#include <cstring>  // for std::memcpy
#include <stdexcept>
#include "include_all.cpp"
#include "serial_parser.hpp"
#include <Servo.h>

Counter count;
Counter_neg send_count;

unsigned long t_prev = millis();

void setup(){
    Serial.begin(115200);
    // while (!Serial); // wait for serial monitor to open
}

void loop() {

    std::vector<uint8_t> payload = receive_data();
    unsigned long t_now = millis();
    if (!payload.empty()) {
        uint8_t id = payload[0];

        if (id == COUNTER) {
            count = parse_struct<Counter>(payload);

            std::vector<uint8_t> buffer_o = pack_data<Counter>(count);
            send_data(buffer_o);

        }

        if (id == COUNTER_NEG) {
            send_count = parse_struct<Counter_neg>(payload);

            std::vector<uint8_t> buffer_o = pack_data<Counter_neg>(send_count);
            send_data(buffer_o);

        }
    }

    // send_count.id = 3;

}
