#include <Arduino.h>
#include <MotionProfile.h>
#include <include_all.cpp>

#define TARGET_ANGLE 3.14  // radians
#define TOTAL_TIME 3      // milliseconds
#define LOOP_DT 20           // milliseconds (loop update rate)

MotionProfile profile(0.0f, TARGET_ANGLE, TOTAL_TIME); // Time in seconds

unsigned long last_update_time = 0;

void setup() {
    Serial.begin(115200);
    while (!Serial); // wait for serial monitor to open
    Serial.println("Starting Motion Profile Test...");
}

void loop() {
    unsigned long now = millis();

    if (now - last_update_time >= LOOP_DT) {
        last_update_time = now;

        float setpoint = profile.get_setpoint(now);
        Serial.print("Time: ");
        Serial.print(static_cast<float>(now)/1000.f);
        Serial.print(" s\tSetpoint: ");
        Serial.println(setpoint, 6);  // print with 6 decimal places
    }
}

