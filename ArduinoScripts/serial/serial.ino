#include <Arduino.h>

void setup() {
  Serial.begin(9600);  // Match the baud rate with the Raspberry Pi
  while (!Serial) ;    // Wait for the serial connection to initialize
}

void loop() {
  if (Serial.available() > 0) {
    // Read the incoming data
    String received = Serial.readStringUntil('\n');  // Read until newline
    Serial.print("Received: ");
    Serial.println(received);

    // Split the data using comma as a delimiter
    int data[3];  // Example for 3 data points
    int index = 0;

    char *token = strtok(received.c_str(), ",");
    while (token != NULL && index < 3) {
      data[index++] = atoi(token);  // Convert to integer
      token = strtok(NULL, ",");
    }

    // Process data (example: print to Serial Monitor)
    Serial.print("Parsed Data: ");
    for (int i = 0; i < index; i++) {
      Serial.print(data[i]);
      Serial.print(" ");
    }
    Serial.println();
  }
}
