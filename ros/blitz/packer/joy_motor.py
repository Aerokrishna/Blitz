#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from std_msgs.msg import Int16
from sensor_msgs.msg import Joy
# from robot_interfaces.msg import PWM

class JoyControl(Node):
    def __init__(self):
        super().__init__("manual_mode")

        # SUBSCRIBERS
        self.joy_sub = self.create_subscription(Joy ,"/joy", self.joy_callback, 10)
        self.pwm_pub = self.create_publisher(Int16, "/motor_pwm", 10)

        self.create_timer(0.001, self.timer_cb)
    # joystick subscriber callback
    def joy_callback (self, joy_msg: Joy):
        msg = Int16()
        msg.data = int(200*joy_msg.axes[1])
        # self.pwm_pub.publish(msg)
        # self.get_logger().info("pubbing pwm")
    
    def timer_cb(self):
        msg = Int16()
        msg.data = 100
        self.pwm_pub.publish(msg)
        self.get_logger().info("pubbing pwm")

def main(args=None):
    rclpy.init(args=args)
    node = JoyControl()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()