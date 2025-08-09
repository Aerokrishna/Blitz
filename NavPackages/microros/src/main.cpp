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
    // Example: serialize struct
    YourMom original{1.2f, 239};
    uint8_t buffer[sizeof(YourMom)];
    std::memcpy(buffer, &original, sizeof(YourMom));

    // Parse it back
    YourMom parsed = parse_struct<YourMom>(buffer, sizeof(buffer));

    // receive data
    std::vector<uint8_t>  payload = receive_data();
    
    Serial.print("A :");
    Serial.print(parsed.a);
    Serial.print(" B ");
    Serial.println(parsed.b);
    // Serial.print(" C ");
    // Serial.println(parsed.c);
}
