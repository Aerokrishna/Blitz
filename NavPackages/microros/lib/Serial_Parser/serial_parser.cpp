#include "serial_parser.hpp"

std::vector<int8_t> parse_int(uint8_t* payload, unsigned int length) {
    if (length % sizeof(int8_t) != 0) {
            return {};
        }

        int total_elements = length / sizeof(int8_t);
        std::vector<int8_t> parsed_data;
        
        int8_t* values = (int8_t*)payload;  // reinterpret cast
        
        for (int i = 0; i < total_elements; i++){
            parsed_data.push_back(values[i]);
        }
              
    return parsed_data;
}

// pack_data : requires data tyoe of the array, number of elements, id
template<typename T>
std::vector<uint8_t> pack_data(const T* data, size_t count, uint8_t id) {

    size_t total_bytes = sizeof(T) * count;
    std::vector<uint8_t> buffer(total_bytes + 1);
    buffer[0] = id;

    std::memcpy(&buffer[1], data, total_bytes);
    return buffer;
}

void send_data(const std::vector<uint8_t>& buffer){
    if (Serial){
    Serial.write(buffer.data(), buffer.size());
    }
}

std::vector<uint8_t> receive_data(){
    if (Serial){
    if (Serial.available()) {  
        int byte_size = Serial.available();
        uint8_t* payload = new uint8_t[byte_size]; // to avoid creating variable length array

        for (int i = 0; i<byte_size; i++){
            payload[i] = Serial.read();
        }
        return payload;
        }
    return {};
    }
}

// std::vector<float> parse_long_int() {
    
// }

// std::vector<float> parse_float() {
    
// }

// std::vector<float> parse_long_float() {
    
// }