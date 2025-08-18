// lib/PacketUtils/include/packet_utils.hpp
#pragma once
#include <stdint.h>
#include <vector>
#include <Arduino.h>
#include <iostream>
#include <cstring>  // for std::memcpy
#include <stdexcept>
#include "serial_interfaces.hpp"
// PARSE OR PACK DATA PACKETS

// Pass the payload(byte packet), received through serial, parse it in the form of template interface
template<typename T_parse>
T_parse parse_struct(const std::vector<uint8_t>& payload) {
    T_parse result;
    uint8_t length = payload.size();
    if (length < sizeof(T_parse)) {
        Serial.println("BAD DATA");
    }
    std::memcpy(&result, payload.data(), sizeof(T_parse));
    return result;
}

// Pass the template interface, it will pack the data in the form of the ppayload(byte packet)
template<typename T_pack>
std::vector<uint8_t> pack_data(T_pack data) {
    size_t total_bytes = sizeof(T_pack);
    std::vector<uint8_t> buffer(total_bytes);
    std::memcpy(&buffer, data, total_bytes);
    return buffer;
}

// SEND OR RECEIVE DATA PACKETS 

// Pass the payload, it will write the data to the serial port
void send_data(const std::vector<uint8_t>& buffer){
    if (Serial){
    Serial.write(buffer.data(), buffer.size());
    }
}

std::vector<uint8_t> receive_data() {
    static std::vector<uint8_t> buffer;

    while (Serial.available()) {
        buffer.push_back(Serial.read());

        // If we have at least 1 byte, we can check the ID
        if (buffer.size() == 1) {
            uint8_t id = buffer[0];
            size_t expected_size = get_packet_size(id);
            if (expected_size == 0) {
                buffer.clear(); // unknown ID
            }
        }

        // If we have full packet, return it
        if (!buffer.empty()) {

            uint8_t id = buffer[0];
            size_t expected_size = get_packet_size(id);
            if (expected_size > 0 && buffer.size() >= expected_size) {
                // Serial.println("GOOD DATA ");

                std::vector<uint8_t> packet(buffer.begin(), buffer.begin() + expected_size);
                buffer.erase(buffer.begin(), buffer.begin() + expected_size);
                return packet;
            }
        }
    }
    return {};
}


// std::vector<int16_t> parse_int(uint8_t* payload, unsigned int length) {
//     if (length % sizeof(int8_t) != 0) {
//             return {};
//         }

//         int total_elements = length / sizeof(int16_t);
//         std::vector<int16_t> parsed_data;
        
//         int16_t* values = (int16_t*)payload;  // reinterpret cast
        
//         for (int i = 0; i < total_elements; i++){
//             parsed_data.push_back(values[i]);
//         }
              
//     return parsed_data;
// }
