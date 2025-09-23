#include "blitz.hpp"

// declared as a global variable 
Counter count;

// takes raw byte array, matches the interface and parses the data
void store_data(std::vector<uint8_t> payload) {

    if (!payload.empty()) {

        // find id
        uint8_t id = payload[0];
    
        // parse the struct based on the id
        if (id == COUNTER) {
            
            // function to parse the struct
            count = parse_struct<Counter>(payload); // count.a, count.b, count.c, count.d, based on your interface

            // multiply the count by two 
            count.a = count.a * 2;
            count.b = count.b * 2;
            count.c = count.c * 2;
            count.d = count.d * 2;

            // function to send data, data should be of one of the interfaces struct type
            send_data(pack_data<Counter>(count));

        }
    }
}

void setup(){

    // serial begin
    Serial.begin(115200);
}

void loop() {

    // recieves data in every loop 
    std::vector<uint8_t> payload = receive_data();

    // stores data
    store_data(payload);
    
}