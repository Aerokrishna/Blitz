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

    # Send data to Pico
    # ser.write(b"Hello Pico!\n")

    count = 0
    while True:
        if ser.in_waiting >= 1:  # full packet
            count+=1
            header = ser.read(1)
            if header[0] != 0xAA:
                continue  # resync

            id_byte = ser.read(1)[0]  # already got ID
            if id_byte == 1:  # CMD_VEL
                data_bytes = ser.read(12)  # 3 floats
                vx, vy, vyaw = struct.unpack('fff', data_bytes)
                print(f"{count} ID: {id_byte}, vx: {round(vx, 3)}, vy: {round(vy,3)}, vyaw: {round(vyaw,3)}")

            elif id_byte == 2:  # ODOM
                data_bytes = ser.read(20)  # 4 floats
                # print(len(data_bytes))
                x, y, yaw, pitch, roll = struct.unpack('fffff', data_bytes)
                print(f"{count} ID: {id_byte}, x: {round(x, 3)}, y: {round(y,3)}, yaw: {round(yaw,3)}, pitch: {round(pitch,3)}, roll: {round(roll,3)}")

except serial.SerialException as e:
    print(f"Error: {e}")

finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
        print("Serial connection closed.")

