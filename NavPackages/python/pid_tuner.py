import serial
import struct
import time

PORT = '/dev/ttyACM0'
BAUD = 115200

try:
    ser = serial.Serial(PORT, BAUD, timeout=1)
    time.sleep(2)
    print("Connected to Pico")

    while True:
        if ser.in_waiting>=7:  # 1 byte ID + 2*2 bytes for int16_t
            raw = ser.read(7)
            setpoint, current, time_ = struct.unpack('<hhh', raw[1:7])  # little endian, 2 signed short (int16)
            print(f"ID : {raw[0]} SETPOINT : {setpoint/100}, CURRENT : {current/100}, TIME : {time_/100}")

except serial.SerialException as e:
    print(f"Error: {e}")

finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
        print("Serial connection closed.")
