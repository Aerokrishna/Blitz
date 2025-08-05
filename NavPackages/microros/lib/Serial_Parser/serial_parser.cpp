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

// std::vector<float> parse_long_int() {
    
// }

// std::vector<float> parse_float() {
    
// }

// std::vector<float> parse_long_float() {
    
// }
