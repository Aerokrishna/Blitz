#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Joy
from robot_interfaces.msg import PWM

class JoyControl(Node):
    def __init__(self):
        super().__init__("manual_mode")

        # SUBSCRIBERS
        self.joy_sub = self.create_subscription(Joy ,"/joy", self.joy_callback, 10)
        self.pwm_pub = self.create_publisher(PWM, "/motor_pwm", 10)
        self.count = 0

    # joystick subscriber callback
    def joy_callback (self, joy_msg: Joy):
        msg = PWM()
        # dc motor
        # msg.pwm = int(240*joy_msg.axes[1])

        #bldc motor
        msg.pwm = int(400*joy_msg.axes[1]) + 1500

        self.pwm_pub.publish(msg)
        self.get_logger().info(f"pubbing pwm {msg.pwm}")
    
def main(args=None):
    rclpy.init(args=args)
    node = JoyControl()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()