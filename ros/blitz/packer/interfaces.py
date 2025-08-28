from robot_interfaces.msg import CmdVel
from robot_interfaces.msg import Odom
from schema import Schema
schemas = {str : Schema}

schemas = {
    "cmd_vel": Schema(
        topic="/cmdvel",
        msg_id=1,
        struct_fmt="fff",
        fields=["vx", "vy", "vyaw"],
        ros_msg=CmdVel,
        parse=False
    ),
    "odometry": Schema(
        topic="/odom",
        msg_id=2,
        struct_fmt="fff",
        fields=["x", "y", "yaw"],
        ros_msg=Odom,
        parse=True
    )
}