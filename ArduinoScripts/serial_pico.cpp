#include <Arduino.h>
#include <serial_parser.hpp>
#include <include_all.cpp>

void setup() {
    Serial.begin(115200);
    while (!Serial); // wait for serial monitor to open
}

void loop() {
    // Send Data
    float data[] = {1.0, 2.83, 4.56};
    uint8_t = 2;
    std::vector<uint8_t> buffer = pack_data(data, 3, id);
    send_data(buffer);

    delay(1000);

    // Receive Data
    uint8_t* payload = receive_data();

    // parse the id
    int8_t id = (int8_t)payload[0];
    if (id == 1){
        //id 1 will correspond to an array of 3 integers
        std::vector<int8_t> values = parse_int(payload, sizeof(payload));
    }
}


