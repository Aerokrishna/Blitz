#!/usr/bin/env python3
# subscribes from ros2 topics and packs data to the serial port
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Imu
import struct
import serial
from interfaces import schemas
from schema import Schema
from rclpy.executors import SingleThreadedExecutor

class SerialSender(Node):
    def __init__(self, port="/dev/ttyACM0", baud=115200):
        super().__init__("serial_sender")
        self.schema = schemas 
        
        try:
            self.ser = serial.Serial(port, baud, timeout=1)
            self.get_logger().info(f"Opened serial port {port}")
        except serial.SerialException as e:
            self.get_logger().error(f"Serial error: {e}")
            self.ser = None

        # Create subscriptions
        for name, schema in schemas.items():
            if not schema.parse:
                self.create_subscription(
                    schema.ros_msg,
                    schema.topic,
                    self.sub_callback(schema),
                    10
                )
                self.get_logger().info(f"Subscribed to {schema.topic}")

    def sub_callback(self, schema : Schema):
        def callback(msg):
            if not self.ser:
                return
            packet = schema.pack(msg)
            self.ser.write(packet)
            # self.get_logger().debug(f"Sent {schema.topic}: {packet.hex()}")
        return callback

    # def timer_callback(self, schema : Schema):

class SerialReceiver(Node):
    def __init__(self, port="/dev/ttyACM0", baud=115200):
        super().__init__("serial_receiver")
        self.schema = schemas 
        
        try:
            self.ser = serial.Serial(port, baud, timeout=1)
            self.get_logger().info(f"Opened serial port {port}")
        except serial.SerialException as e:
            self.get_logger().error(f"Serial error: {e}")
            self.ser = None

        self.create_timer(0.001, self.serial_read)
        # Create Publisher and Timer
        for name, schema in schemas.items():
            if schema.parse:
                schema.pub = self.create_publisher(schema.ros_msg, schema.topic, 10)
                self.create_timer(0.1, self.timer_callback(schema))
                self.get_logger().info(f"Publishing to {schema.topic}")

        self.schema = {s.id: s for s in schemas.values()}

    def timer_callback(self, schema : Schema):
        def callback():
            if not self.ser:
                return
            data = self.schema[schema.id].payload_data
            if data is not None:
                msg = schema.unpack(data)
                schema.pub.publish(msg)
            # self.get_logger().debug(f"Publishing to {schema.topic}")
        return callback

    def serial_read(self):
        if not self.ser:
            return
        
        if self.ser.in_waiting >= 1:  
            header = self.ser.read(1)
            if header[0] != 0xAA:
                return  # resync

            id_byte = self.ser.read(1)[0]  # already got ID
            self.schema[id_byte].payload_data = self.ser.read(struct.calcsize((self.schema[id_byte].struct)))
            
def main():
    rclpy.init()

    sender = SerialSender()
    receiver = SerialReceiver()

    executor = SingleThreadedExecutor()
    executor.add_node(sender)
    executor.add_node(receiver)
    
    try:
        executor.spin()  # either spin in multithreaded executor, or run separately
    except KeyboardInterrupt or serial.SerialException as e:
        sender.get_logger().error(f"Serial error: {e}")
        pass
    finally:
        sender.destroy_node()
        rclpy.shutdown()

if __name__ == "__main__":
    main()