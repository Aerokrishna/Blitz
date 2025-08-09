// lib/PacketUtils/include/packet_utils.hpp

#pragma once
#include <stdint.h>
#include <vector>
#include <Arduino.h>
#include <iostream>
#include <cstring>  // for std::memcpy
#include <stdexcept>

std::vector<uint8_t> pack_data(int16_t* data, size_t count, uint8_t id);
void send_data(const std::vector<uint8_t>& buffer);
std::vector<uint8_t> receive_data();

// PARSE OR PACK DATA PACKETS

// Pass the payload(byte packet), received through serial, parse it in the form of template interface
template<typename T_parse>
T_parse parse_struct(const std::vector<uint8_t>& payload) {
    T_parse result;
    uint8_t length = payload.size();
    if (length < sizeof(T_parse)) {
        Serial.println("BAD DATA");
    }
    std::memcpy(&result, payload, sizeof(T_parse));
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

// Called continuously to receive data and form the payload
std::vector<uint8_t> receive_data(){
    if (Serial){
        if (Serial.available()) {  
            int byte_size = Serial.available();
            uint8_t* payload = new uint8_t[byte_size]; // dynamically allocate buffer

            for (int i = 0; i < byte_size; i++){
                payload[i] = Serial.read();
            }

            // Convert to vector
            std::vector<uint8_t> buffer(payload, payload + byte_size);

            delete[] payload;  // Free the heap memory
            return buffer;
        }
        return {};
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
