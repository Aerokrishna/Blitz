import serial
import time
import struct

# Adjust the port to match yours (e.g., COM3 on Windows, /dev/ttyACM0 or /dev/ttyUSB0 on Linux)
PORT = '/dev/ttyACM0'
BAUD = 115200

try:
    ser = serial.Serial(PORT, BAUD, timeout=1)
    time.sleep(1)  # Wait for Pico to reset
    print("Connected to Pico")

    my_data1 = [1, 4.0, 6.0, 9.9]
    my_data2 = [2, 1.0, 4.0, 5.9, 3.45, 2.11]

    count = 0
    while True:
        count+=1

        if count % 2 == 0:
            my_data2[1] += 0.1
            my_data2[2] += 0.1
            my_data2[3] += 0.1
            my_data2[4] += 0.1
            my_data2[5] += 0.1

            data2 = struct.pack('<BBfffff', 0xAA, my_data2[0], my_data2[1], my_data2[2], my_data2[3], my_data2[4], my_data2[5])

            ser.write(data2)
        
        else:
            my_data1[1] += 0.1
            my_data1[2] += 0.1
            my_data1[3] += 0.1
            data1 = struct.pack('<BBfff', 0xAA, my_data1[0], my_data1[1], my_data1[2], my_data1[3])

            ser.write(data1)
        # ser.write(data2)

        time.sleep(0.1)

except serial.SerialException as e:
    print(f"Error: {e}")

finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
        print("Serial connection closed.")

