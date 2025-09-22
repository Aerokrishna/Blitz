from .blitz_helper import BlitzInterfaces
blitz_interfaces = {str : BlitzInterfaces}

blitz_interfaces = {
    "counter": BlitzInterfaces(
        msg_id=2,
        struct_fmt="hhff",
        from_mcu=False
    ),

    "counter_response": BlitzInterfaces(
        msg_id=1,
        struct_fmt="hhff",
        from_mcu=True
    ),
}
