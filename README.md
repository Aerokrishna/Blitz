# Blitz ⚡
Blitz is a lightweight library for **data transmission between a Python script (or ROS) and a microcontroller**.  
It lets you easily send structured packets to your MCU and receive data back — whether you want to **visualize it in Python/RViz** or **control hardware like motors** via ROS topics or just a Python script.

Blitz offers a simple configuration and API, serving as a clean alternative to heavier communication stacks.

---
## 🎬 Demonstration

This section describes a brief demonstration of **Blitz** using the demo scripts provided in this repository.  
In this demo, the **computer sends a number every second** to the microcontroller (MCU), and the MCU multiplies it by two and sends it back through serial. We will walk you through how to do this using **ROS** and **Python**.

> ⚠️ **Note:** If you do not have ROS installed, skip the ROS section and proceed directly to the Python demonstration.

---

### 🛠️ Setup Workspace

First, create a workspace and set up the source directory:

```bash
# Make workspace
mkdir -p ~/blitz_ws/src
cd ~/blitz_ws/src

# Clone the repository
git clone https://github.com/yourusername/blitz-serial.git
```
### Setup Microcontroller
You can use any microcontroller. This library has been tested with Raspberrypi pico, Arduino, Teensy and ESP32. 
In `blitz_mcu/src` you will find the library, Blitz. Make sure you have it in your microcontroller's workspace and is accessible by your IDE.
**Flash the microcontroller using the template script available in `blitz_mcu/templates/blitz_demo.cpp`**
Make sure your board is connected and selected in your IDE / platform.

### ROS <-> Microcontrollers
Build and source your ROS2 workspace.

```bash
cd ~/blitz_ws
colcon build
source install/setup.bash
```

Launch Blitz, this establishes the bridge between the microcontroller and the computer.
```bash
ros2 launch blitz blitz.launch.py
```

```bash
expected output
```

In another terminal launch the demo script. This script publishes to the topic `/counter`. 

```bash
ros2 rub blitz blitz_demo
```

Now we have successfully established communication between the two devices. You can verify this by echoing the topic `/counter` and `/counter_response`. `/counter` topic contains the numbers published by the demo node. `/counter_response` contains the response from the microcontroller, which is the double of the number sent by the computer.


### Python <-> Microcontroller
Install the python package. 

```bash
cd ~/blitz_ws/src/Blitz/blitz_python/
pip install e .
```

Run the python script

```bash
python demo/blitz_demo.py
```

```bash
expected output
```
From the print statements on the terminal, we can verify the data being sent to the microcontroller and back.

---

## 📖 Usage

This section explains how to configure Blitz for your **custom data transmission** and establish communication with your microcontroller.  
Depending on your setup, you may be using Blitz with **ROS**, plain **Python**, or directly on the **MCU**.

### 🤖 ROS2
In the `blitz_ros/` directory provided in this repo you will find two packages, `blitz` and `robot_interfaces`. 

-  **robot_interfaces** → In the `robot_interfaces` package you define your custom interface. Inside the `msg` directory, create your .msg file with the desired interface. Make sure to build and source the repo.
> ⚠️ Note: Blitz supports ROS interfaces with **single fields** (e.g. `Float32`, `Int32`) but not concatenated ones like `odom.pose.pose.x`. If you need such data, create a **custom ROS interface** with the fields you need.

-  **blitz** → The `blitz` package acts like a bridge between the microcontroller and ROS2 network. Navigate to the file `blitz/blitz/interfaces.py`. These are the following steps for the configuration.

```python

# add an interface as an object in the dictionary
blitz_interfaces = {

    # name : Blitz object (name does not hold any significance)
    "counter": Blitz(

        # the topic whose data you want to send to the mcu
        # this is the topic you will PUBLISH to
        topic="/counter",

        # msg id should match the id in the microcontroller
        msg_id=3,

        # format of the msg packet 
        # uint8 : B, int8 b, uint16 H, int16 h, uint32 I, int32 i, float32 f
        struct_fmt="hhff",

        # fields just as in the ros interface 
        fields=["a","b","c","d"],

        # ros interface name, import it first
        ros_msg=Counter,

        # false id you are writing to the microcontroller
        from_mcu=False
    ),

    "counter_response": Blitz(

        # to receive from the microcontroller

        # the data coming from the mcu will be published in this topic
        topic="/counter_response",

        # msg_id should match the id in the microcontroller
        msg_id=3,

        # same as above
        struct_fmt="hhff",
        fields=["a","b","c","d"],
        ros_msg=Counter,

        # set this as true to indicate that this data is coming from the microcontroller
        from_mcu=True
    ),
}
```

Once this is done, you can build your package and it will be setup to send and receive data to and from the microcontroller. Publish to the topics that have been configured to send the message to the micrcontroller, and subscribe from the topics configured to receive messages from the microcontroller.
Test your configuration by launching blitz after building and sourcing your workpace.

```bash
ros2 launch blitz blitz.launch.py
```

```bash
expected output
```


### 🐍 Python
In the `blitz_python/` directory of this repository, you will find two subdirectories: **`blitz`** and **`examples`**.  
- The **`blitz`** directory contains the core implementation and imports required to use Blitz Python.  
- The **`examples`** directory provides demo scripts that showcase how to use the library.  

To install the package, navigate to:  

```bash
cd ~/blitz_ws/src/Blitz/blitz_python
pip install -e .
```

To configure your custom data packets, navigate to `~/blitz_ws/src/Blitz/blitz_python/blitz/interfaces.py`

```python
from .blitz_helper import BlitzInterfaces
blitz_interfaces = {str : BlitzInterfaces}

# blitz interfaces are written as a dictionary 
blitz_interfaces = {

    # the key represents the name of the interface and will be used to reference the interface in the script
    "counter": BlitzInterfaces(
        # msg id should match the id in the microcontroller
        msg_id=2,

        # format of the msg packet 
        # uint8 : B, int8 b, uint16 H, int16 h, uint32 I, int32 i, float32 f
        struct_fmt="hhff",

        # false if you want to send this interface to the microcontroller,
        from_mcu=False
    ),

    "counter_response": BlitzInterfaces(

        # similar configuration as above
        msg_id=1,
        struct_fmt="hhff",
        # true when you want to parse this interface from the microcontroller
        from_mcu=True
    ),
}

```

The below is the walk through of how to use it in a python script.

```python
from blitz import blitz_interfaces, Blitz
import time

# initialize
blitz = Blitz()

while True:

    # fill the data field of the interface
    blitz_interfaces["counter"].data = [2, 3, 3.4, 5.1]

    # call this function whenver you want to send it to the microcontroller
    blitz.blitz_write(id=blitz_interfaces["counter"].id)

    time.sleep(0.1)

    # reads the serial port, parses the data and updates the data for each interface
    blitz.blitz_read()

    # the parsed data can be accessed as an array of items as configured in the interfaces
    print("DATA :: ", blitz_interfaces["counter_response"].data)

```

### 🔌 MCU
This section is relevant whether or not you are using a pio package. Here we will be addressing the files with respect to the current repo structure. Navigate to `blitz_mcu/src/Blitz/blitz_interfaces.hpp`. This is the configuration file for the microcontroller.

```cpp
// 3 steps to add new interface
// - include packet id in the PacketID function
// - add the interface as a struct
// - include the packet id in the packet size function

enum PacketID : uint8_t {

    // each interface has an id, which is common between sbc and mcu
    // each interface or data packet is referenced in the microcontroller code using enum names
    // like COUNTER, ODOM

    COUNTER = 1,

};

// add your interface as a struct
#pragma pack(push, 1)
struct Counter {

    // order of these fields must match your ros interface
    uint8_t id;
    int16_t a;
    int16_t b;
    float c;
    float d;

};  
#pragma pack(pop)

size_t get_packet_size(uint8_t id) {
    switch (id) {

        // add this line for each new interface 
        case COUNTER:    return sizeof(Counter);

        default:      return 0; 
   }
}

```

Now to send and receive data in the microcontroller, the following functions are used in the microcontroller script.

**To receive and parse the data coming from the computer :**
```cpp
receive_data(); // receives the data, returns a byte array
parse_struct(const std::vector<uint8_t>& payload); // converts byte array into useful data

// use case
Counter count;
payload = receive_data();
count = parse_struct<Counter>(payload);
```

**To pack and send the data to the computer :**


```cpp
pack_data(); // takes data and converts it into a byte array
send_data(const std::vector<uint8_t>& buffer); // takes the byte array and writes it to the serial port

// use case
Counter count_response;

count_response.id = COUNTER;
count_response.a = 1;
send_data(pack_data<Counter>(count_response));

```
To receive the messages from the computer and store them in the correct interface msg_ids are matched. The mssage interfaces are referenced by enums in the script. The `store_data` function is the recommended way to convert the raw data returned by the receive data function into the right struc in this script as per the id. Store data function can be used as a callback in order to set flags as it is triggered whenever a new message is received. This function is to be called with receive data and is only required when receiving data from the computer.

```cpp
// store data is usually called immediately after receive data function
std::vector<uint8_t> payload = receive_data();
store_data(payload);

// takes raw byte array, matches the interface and parses the data
void store_data(std::vector<uint8_t> payload) {

    if (!payload.empty()) {

        // find id
        uint8_t id = payload[0];
    
        // parse the struct based on the id
        if (id == COUNTER) {
            
            // parses the data according to the right interface
            count = parse_struct<Counter>(payload);
        }
    }
}

```

An example of the `cpp` code and the interfaces configuration can be found in the
`blitz_mcu/templates` directory of the repository.  This directory contains an example script `blitz_demo.cpp`. 

---
