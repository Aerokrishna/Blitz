# Blitz ⚡
Blitz is a lightweight library for **data transmission between a Python script (or ROS) and a microcontroller**.  
It lets you easily send structured packets to your MCU and receive data back — whether you want to **visualize it in Python/RViz** or **control hardware like motors** via ROS topics.  

Blitz offers a simple configuration and API, serving as a clean alternative to heavier communication stacks.
 
---

## 📖 Usage

This section explains how to configure Blitz for your **custom data transmission** and establish communication with your microcontroller.  
Depending on your setup, you may be using Blitz with **ROS**, plain **Python**, or directly on the **MCU**.

### 🤖 ROS2
In the `ros/` directory provided in this repo you will find two packages, `blitz` and `robot_interfaces`. 

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

Once this is done, you can build your package and it will be setup to send and receive data to and from the microcontroller. 

### 🐍 Python
coming soon

### 🔌 MCU
This section is relevant whether or not you are using a pio package. Here we will be addressing the files with respect to the current repo structure. Navigate to `mcu_pio/lib/Blitz/serial_interfaces.hpp`. This is the configuration file for the microcontroller.

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

An example of the `cpp` code and the interfaces configuration can be found in the
`templates` directory of the repository. This directory contains an example script `mcu_.pp`to receive data, and send data. The `store_data` function in this script takes the payload or byte array returned by the `receive_data` function and stores the data in the correct variables defined earlier by matching the id of the data received. 

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

---
## 🎬 Demonstration
This section will describe the brief demonstration of Blitz provided in this repository. 
The dependencies to follow along the  are listed as follows :
Platform IO must be installed
ROS must be installed, if not skip to the python config tutorial.

### ROS <-> Microcontroller
make ws, make src, clone the repo inside src
colcon build outside src
connect your microcontroller, upload the code through pio uploader, pio package is mcu_pio
launch the file ros2 launch blitz blitz.launch.py
echo the topic in another terminal, you will see data coming
now run another node in some terminal, you will see the data in the topic getting multiplied by 2. 

### Python <-> Microcontroller
something similar

---

## PIO Package
you can use timer callbacks, include folder structure etc.
