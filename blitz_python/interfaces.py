from robot_interfaces.msg import Counter
from blitz import Blitz

blitz_interfaces = {str : Blitz}

blitz_interfaces = {

    "counter": Blitz(
        msg_id=2,
        struct_fmt="hhff",
        from_mcu=False
    ),

    "counter_response": Blitz(
        msg_id=1,
        struct_fmt="hhff",
        from_mcu=True
    ),

}
