#include <Arduino.h>
#include <serial_parser.hpp>
#include <include_all.cpp>

void setup() {
    Serial.begin(115200);
    while (!Serial); // wait for serial monitor to open
    Serial.println("Starting Motion Profile Test...");

}

void loop() {
    if (Serial.available()) {  
        int byte_size = Serial.available();
        uint8_t* payload = new uint8_t[byte_size]; // to avoid creating variable length array

        for (int i = 0; i<byte_size; i++){
            payload[i] = Serial.read();
        }

        std::vector<int8_t> parsed_data = parse_int(payload, byte_size);
        
        Serial.print("RECEIVED: ");
        Serial.println(parsed_data[0]);
}
}
