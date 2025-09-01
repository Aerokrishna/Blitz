from robot_interfaces.msg import CmdVel
from robot_interfaces.msg import Odom
from robot_interfaces.msg import PWM
from std_msgs.msg import Int16, Float32
from schema import Schema
schemas = {str : Schema}

schemas = {
    # "cmd_vel": Schema(
    #     topic="/cmdvel",
    #     msg_id=1,
    #     struct_fmt="fff",
    #     fields=["vx", "vy", "vyaw"],
    #     ros_msg=CmdVel,
    #     parse=False
    # ),
    "odometry": Schema(
        topic="/odom",
        msg_id=2,
        struct_fmt="fff",
        fields=["x", "y", "yaw"],
        ros_msg=Odom,
        parse=True
    ),
    "motor_pwm": Schema(
        topic="/motor_pwm",
        msg_id=3,
        struct_fmt="h",
        fields=["pwm"],
        ros_msg=PWM,
        parse=False
    ),
    "time_diff": Schema(
        topic="/time_diff",
        msg_id=3,
        struct_fmt="h",
        fields=["pwm"],
        ros_msg=PWM,
        parse=True
    ),
}
