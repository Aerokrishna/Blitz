import serial
from .interfaces import blitz_interfaces
import struct 

class Blitz:

    def __init__(self, port = "/dev/ttyACM0", baud = 115200):
        self.ser = None
        try:
            self.ser = serial.Serial(port, baud, timeout=1)
            print(f"Opened serial port {port}")
        except serial.SerialException as e:
            print(f"Serial error: {e}")
            self.ser = None

        self.schema = {s.id: s for s in blitz_interfaces.values()}

    def blitz_read(self):
        if not self.ser:
            return

        if self.ser.in_waiting >= 1:  
            header = self.ser.read(1)
            if header[0] != 0xAA:
                return  # resync

            id_byte = self.ser.read(1)[0]  # get the id
            payload_data = self.ser.read(struct.calcsize("="+self.schema[id_byte].struct))

            if payload_data is not None:
                self.schema[id_byte].data = self.schema[id_byte].unpack(payload_data)
    
    def blitz_write(self, id):
        if not self.ser:
                return
        packet = self.schema[id].pack(self.schema[id].data)
        self.ser.write(packet)
        print(f"Writing to Serial")

