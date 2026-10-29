# Blitz Architecture ⚡

How Blitz moves data between a computer (Python or ROS 2) and a microcontroller: the wire format, how each side packs, parses and stores data, and how the ROS bridge is built.

---

## 🧩 Components

| Component | Location | Role |
|---|---|---|
| MCU library | `blitz_mcu/src/Blitz/` | Header-only C++ (`blitz.hpp` + `blitz_interfaces.hpp`) |
| Python library | `blitz_python/blitz/` | Standalone serial client |
| ROS 2 bridge | `blitz_ros/blitz/` | Two nodes (`packer`, `parser`) that map topics ↔ serial frames |

```
  ROS 2 / Python                       Serial (USB)                   Microcontroller
 ───────────────                      ────────────                   ─────────────────
 /counter ──▶ packer ── pack() ─────▶ [AA|id|payload] ─────▶ receive_data() → store_data()
                                                                        │
 /counter_response ◀── parser ◀── unpack() ◀── [AA|id|payload] ◀── send_data(pack_data<T>())
```

All three speak the same wire format, so one firmware works with both plain Python and ROS.

---

## 📦 Wire Format

```
+------+------+---------------------------+
| 0xAA |  id  |  payload (struct bytes)   |
|  1B  |  1B  |  sizeof(struct) bytes     |
+------+------+---------------------------+
```

- **`0xAA`** — start-of-frame header, used to find frame boundaries.
- **`id`** — which interface this is. Both sides derive the payload length from the id, so **length is never sent**.
- **payload** — the raw bytes of a packed struct: little-endian, no padding.
- **Debug frame** (MCU → computer only): `| 0xAA | 99 | len | utf-8 text |`. id `99` is reserved.

Framing overhead is 2 bytes. A `Counter` (`hhff`) is 12 bytes of payload, so 14 bytes on the wire.

### The schema contract

Each interface is declared once per side, and the declarations must agree:

| | MCU | Python | ROS |
|---|---|---|---|
| Layout | `#pragma pack(1)` struct | `struct_fmt="hhff"` | `struct_fmt` + `fields` + `ros_msg` |
| ID | `PacketID` enum | `msg_id` | `msg_id` |
| Length | `get_packet_size(id)` | `struct.calcsize(fmt)` | `struct.calcsize(fmt)` |

Why the bytes line up: `#pragma pack(1)` removes struct padding, Python's `<` and `=` format prefixes use standard sizes with no alignment, and all the supported MCUs (AVR, ARM Cortex-M, RP2040, ESP32) are little-endian. A `memcpy` of the struct on the MCU therefore produces exactly the same bytes as `struct.pack` on the computer.

**Rules:** IDs must be unique across *all* interfaces in both directions, because the computer keys a single dictionary by id. Field order must match on every side. `99` is reserved for debug.

---

## 🔌 MCU Side (`blitz.hpp`)

**Packing:** `pack_data<T>(data, id)` copies the struct into a byte vector with `memcpy` and puts the `id` in front. `send_data(buffer)` writes `0xAA` and then the buffer to `Serial`.

**Receiving:** `receive_data()` is **non-blocking** and keeps its own static buffer:
1. Move all available `Serial` bytes into the buffer.
2. Discard bytes until `buffer[0] == 0xAA`.
3. Read the `id` and look up `get_packet_size(id)`. For an unknown id (size 0), drop the header and search again.
4. If the full payload has arrived, return `[id | payload]` and erase those bytes. Otherwise return `{}` and finish the frame on a later call.

Each call returns at most one packet, so call it on every `loop()`.

**Parsing:** `parse_struct<T>(payload)` removes the id byte and copies the bytes straight into `T` with `memcpy`. There is no field-by-field decoding.

**Storing:** `store_data(payload)` is a dispatcher you write yourself. It switches on `payload[0]` and parses the payload into the matching **global struct**. Each global holds the latest value for its interface. The dispatcher is also the natural place to set flags or react to a message; the demo echoes the doubled values back from here.

**Debugging:** `debug_state("text")` sends a debug frame, which appears on the `/debug` topic in ROS.

---

## 🐍 Computer Side: Python (`blitz_python`)

**Config:** each interface is a `BlitzInterfaces(msg_id, struct_fmt, from_mcu)` entry in `interfaces.py`. `Blitz()` opens the port and builds an `{id: interface}` map.

**Packing:** `blitz_write(id)` runs `struct.pack("<BB" + fmt, 0xAA, id, *iface.data)` and then `ser.write`.

**Parsing:** `blitz_read()`:
1. If bytes are waiting, read 1. If it isn't `0xAA`, discard it and return; the reader resyncs over the next calls.
2. Read the `id`, look up its format, read `calcsize(fmt)` bytes, then `struct.unpack`.

**Storing:** the unpacked tuple is written to `blitz_interfaces[name].data`. This is a latest-value store that your script polls. Each call reads at most one frame, so call `blitz_read()` often.

---

## 🤖 Computer Side: ROS 2 (`blitz_ros`)

`blitz.launch.py` starts two nodes. Each opens the serial port (`/dev/ttyACM0` @ 115200) on its own; one only writes and the other only reads, so they never contend.

### Config: `blitz/interfaces.py`

```python
Blitz(topic, msg_id, struct_fmt, fields, ros_msg, from_mcu)
```

`fields` maps each position in `struct_fmt` to an attribute name on `ros_msg`. That mapping is how a ROS message turns into bytes and back, and it is why only **flat (single-level) messages** are supported. Custom messages go in `robot_interfaces/msg/`.

### `packer` (node `serial_sender`): ROS → MCU
- Subscribes to the `topic` of every `from_mcu=False` interface.
- On each message it builds `[getattr(msg, f) for f in fields]`, packs it and writes it to serial.
- It is **event-driven**: one frame per published message, sent at whatever rate you publish.

### `parser` (node `serial_receiver`): MCU → ROS
- Creates a publisher for every `from_mcu=True` interface, plus `/debug` (`std_msgs/String`).
- Runs a tight `serial_read()` loop (not `executor.spin`) with the same framing logic as Python.
- **id 99:** read `len` bytes, decode them as UTF-8 and publish on `/debug`.
- **Known id:** unpack, create a new `ros_msg()`, `setattr` each field, then publish.
- **Unknown id:** log an ID-mismatch error and drop the frame.

**Adding an interface:** add a `.msg` file in `robot_interfaces`, an entry in `interfaces.py`, and a struct + enum value + `get_packet_size` case on the MCU. There is no code generation and no agent process.

---

## 📡 Best-Effort Delivery

Blitz is fire-and-forget, in the spirit of UDP or ROS `BEST_EFFORT` QoS:

- **No ACKs, retries or sequence numbers.** A lost frame stays lost.
- **No checksum.** A corrupted payload behind a valid header and id is delivered as-is.
- **Resync by header.** Both sides discard bytes until they see `0xAA`. Because `0xAA` can also appear inside a payload, a dropped byte can briefly cause a misaligned frame; the stream usually realigns within a few frames.
- **Latest value wins.** The MCU globals and Python's `.data` are overwritten rather than queued. ROS publishers use a queue depth of 10.

This suits high-rate periodic data (velocity commands, odometry, sensor readings), where the next sample replaces a lost one. For one-shot commands that must arrive, add your own ack interface.

---

## ✅ Advantages

- **Tiny overhead:** 2 bytes of framing and a raw binary payload. No text, JSON, CDR or XRCE encoding.
- **Header-only and Arduino-core only:** runs on AVR, Pico, Teensy and ESP32 without an RTOS or a micro-ROS agent.
- **Non-blocking receive:** the MCU's `loop()` never stalls waiting on serial.
- **Zero-copy-style decoding:** a `memcpy` into a packed struct on the MCU and a single `struct.unpack` on the computer.
- **Config-driven:** a new interface means editing configuration only. No code generation and no extra build tooling.
- **Decoupled:** the MCU knows nothing about ROS. The same firmware serves plain Python and ROS.

## ⚠️ Limitations

- Only flat, fixed-size messages: no nested ROS fields, arrays or strings (apart from debug text).
- Payload length is fixed per id.
- One serial port, hard-coded to `/dev/ttyACM0` @ 115200.
- No integrity check (see *Best-Effort Delivery*).
