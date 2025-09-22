import struct

class BlitzInterfaces:
    def __init__(self, msg_id, struct_fmt, from_mcu):
        self.id = msg_id
        self.struct = struct_fmt
        self.from_mcu = from_mcu
        self.payload_data = None
        self.data = None

    def pack(self, data):
        fmt = "<BB" + self.struct
        return struct.pack(fmt, 0xAA, self.id, *data)

    def unpack(self, payload_data):
        fmt = "="+self.struct
        unpacked = struct.unpack(fmt, payload_data)
        
        return unpacked
