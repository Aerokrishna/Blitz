from robot_interfaces.msg import CmdVel
from robot_interfaces.msg import Odom
from robot_interfaces.msg import PWM
from robot_interfaces.msg import Counter
from std_msgs.msg import Int16, Float32
from blitz import Blitz
blitz_interfaces = {str : Blitz}

blitz_interfaces = {
    # "cmd_vel": Blitz(
    #     topic="/cmdvel",
    #     msg_id=1,
    #     struct_fmt="fff",
    #     fields=["vx", "vy", "vyaw"],
    #     ros_msg=CmdVel,
    #     from_mcu=False
    # ),
    # "odometry": Blitz(
    #     topic="/odom",
    #     msg_id=2,
    #     struct_fmt="fff",
    #     fields=["x", "y", "yaw"],
    #     ros_msg=Odom,
    #     from_mcu=True
    # ),
    # "motor_pwm": Blitz(
    #     topic="/motor_pwm",
    #     msg_id=3,
    #     struct_fmt="h",
    #     fields=["pwm"],
    #     ros_msg=PWM,
    #     from_mcu=False
    # ),

    "counter": Blitz(
        topic="/counter",
        msg_id=3,
        struct_fmt="hhff",
        fields=["a","b","c","d"],
        ros_msg=Counter,
        from_mcu=False
    ),

    "counter_response": Blitz(
        topic="/counter_response",
        msg_id=3,
        struct_fmt="hhff",
        fields=["a","b","c","d"],
        ros_msg=Counter,
        from_mcu=True
    ),

    "counter_neg": Blitz(
        topic="/counter",
        msg_id=4,
        struct_fmt="hhff",
        fields=["a","b","c","d"],
        ros_msg=Counter,
        from_mcu=False
    ),

    "counter_neg_response": Blitz(
        topic="/counter_response_neg",
        msg_id=4,
        struct_fmt="hhff",
        fields=["a","b","c","d"],
        ros_msg=Counter,
        from_mcu=True
    ),

}
