#include <Arduino.h>
#include <include_all.cpp>

// Global PID parameters
float Kp = 0.0, Ki = 0.0, Kd = 0.0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait until Serial is ready
  }
  Serial.println("Pico Ready.");
}

void loop() {
  if (Serial.available() > 0) {
    String received = Serial.readStringUntil('\n');
    received.trim();  // Remove any leading/trailing whitespace

    // Split the string using space as delimiter
    int firstSpace = received.indexOf(' ');
    int secondSpace = received.indexOf(' ', firstSpace + 1);

    if (firstSpace != -1 && secondSpace != -1) {
      String kp_str = received.substring(0, firstSpace);
      String ki_str = received.substring(firstSpace + 1, secondSpace);
      String kd_str = received.substring(secondSpace + 1);

      Kp = kp_str.toFloat();
      Ki = ki_str.toFloat();
      Kd = kd_str.toFloat();

      // Print the values to confirm
      Serial.print("Received PID -> Kp: ");
      Serial.print(Kp);
      Serial.print(", Ki: ");
      Serial.print(Ki);
      Serial.print(", Kd: ");
      Serial.println(Kd);
    } else {
      Serial.println("Invalid input format. Expected: <Kp> <Ki> <Kd>");
    }
  }
}

