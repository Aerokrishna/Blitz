import struct

class Schema:
    def __init__(self, topic, msg_id, struct_fmt, fields, ros_msg, parse):
        self.topic = topic
        self.id = msg_id
        self.struct = struct_fmt
        self.fields = fields
        self.ros_msg = ros_msg
        self.parse = parse
        self.payload_data = None
        self.pub = None

    def pack(self, msg):
        values = []
        for field in self.fields:
            # obj = msg
            # for part in field.split("."):
            obj = getattr(msg, field)
            values.append(obj)
        fmt = "<BB" + self.struct
        return struct.pack(fmt, 0xAA, self.id, *values)

    def unpack(self, data):
        fmt = self.struct
        # print(data)
        unpacked = struct.unpack(fmt, data)
        # packet_id, values = unpacked[0], unpacked[1:]
        # if packet_id != self.id:
        #     raise ValueError("ID mismatch")
        msg = self.ros_msg()
        for field, value in zip(self.fields, unpacked):
            setattr(msg, field, value)
        return msg
