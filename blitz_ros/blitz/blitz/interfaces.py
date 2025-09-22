from robot_interfaces.msg import Counter
from std_msgs.msg import Int16
from blitz import Blitz

blitz_interfaces = {str : Blitz}

blitz_interfaces = {

    "counter": Blitz(
        topic="/counter",
        msg_id=1,
        struct_fmt="hhff",
        fields=["a","b","c","d"],
        ros_msg=Counter,
        from_mcu=False
    ),

    "counter_response": Blitz(
        topic="/counter_response",
        msg_id=1,
        struct_fmt="hhff",
        fields=["a","b","c","d"],
        ros_msg=Counter,
        from_mcu=True
    ),
}
