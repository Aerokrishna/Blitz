import serial
import time
import tkinter as tk
from tkinter import ttk
import threading
import struct

# ============ Serial Setup ============
PORT = '/dev/ttyACM0'  # Update for your system
BAUD = 115200

try:
    ser = serial.Serial(PORT, BAUD, timeout=1)
    time.sleep(2)
    print("Connected to Pico")
except serial.SerialException as e:
    print(f"Serial connection error: {e}")
    ser = None

# ============ GUI Setup ============
root = tk.Tk()
root.title("PID Tuner")
root.geometry("400x400")  # Increased window size

# Style
style = ttk.Style()
style.configure("TLabel", font=("Helvetica", 12))
style.configure("TButton", font=("Helvetica", 12))

# Slider value labels
kp_val = tk.StringVar(value="1.00")
ki_val = tk.StringVar(value="0.10")
kd_val = tk.StringVar(value="0.05")
target_val = tk.StringVar(value="0")
time_val = tk.StringVar(value="1.00")

# Update functions
def update_kp(val): kp_val.set(f"{float(val):.2f}")
def update_ki(val): ki_val.set(f"{float(val):.2f}")
def update_kd(val): kd_val.set(f"{float(val):.2f}")
def update_target(val): target_val.set(f"{int(float(val))}")
def update_time(val): time_val.set(f"{float(val):.2f}")

# Sliders and labels
ttk.Label(root, text="Kp").grid(row=0, column=0, padx=10, pady=10, sticky="w")
kp_slider = ttk.Scale(root, from_=0.0, to=20.0, orient='horizontal', command=update_kp)
kp_slider.set(6.5)
kp_slider.grid(row=0, column=1, padx=10)
ttk.Label(root, textvariable=kp_val).grid(row=0, column=2)

ttk.Label(root, text="Ki").grid(row=1, column=0, padx=10, pady=10, sticky="w")
ki_slider = ttk.Scale(root, from_=0.0, to=1.0, orient='horizontal', command=update_ki)
ki_slider.set(0.0)
ki_slider.grid(row=1, column=1, padx=10)
ttk.Label(root, textvariable=ki_val).grid(row=1, column=2)

ttk.Label(root, text="Kd").grid(row=2, column=0, padx=10, pady=10, sticky="w")
kd_slider = ttk.Scale(root, from_=0.0, to=2.0, orient='horizontal', command=update_kd)
kd_slider.set(0.0)
kd_slider.grid(row=2, column=1, padx=10)
ttk.Label(root, textvariable=kd_val).grid(row=2, column=2)

# New sliders for target position and time
ttk.Label(root, text="Target Position (°)").grid(row=3, column=0, padx=10, pady=10, sticky="w")
target_slider = ttk.Scale(root, from_=-180, to=180, orient='horizontal', command=update_target)
target_slider.set(0)
target_slider.grid(row=3, column=1, padx=10)
ttk.Label(root, textvariable=target_val).grid(row=3, column=2)

ttk.Label(root, text="Total Time (s)").grid(row=4, column=0, padx=10, pady=10, sticky="w")
time_slider = ttk.Scale(root, from_=0, to=10, orient='horizontal', command=update_time)
time_slider.set(4.0)
time_slider.grid(row=4, column=1, padx=10)
ttk.Label(root, textvariable=time_val).grid(row=4, column=2)

# Send all values
def send_pid_values():
    if ser is None or not ser.is_open:
        print("Serial port not open.")
        return

    kp = int(kp_slider.get() * 100)
    ki = int(ki_slider.get() * 100)
    kd = int(kd_slider.get() * 100)
    target = int(target_slider.get())         # degrees (0–360)
    total_time = int(time_slider.get() * 100) # time in 0.01s

    # Pack as 5 int16_t values
    data = struct.pack('<hhhhh', kp, ki, kd, target, total_time)
    ser.write(data)
    print(f"Sent: kp={kp}, ki={ki}, kd={kd}, target={target}, time={total_time}")

ttk.Button(root, text="Send", command=send_pid_values).grid(row=5, column=0, columnspan=3, pady=20)

# ============ Background Serial Listener ============
def serial_listener():
    while ser and ser.is_open:
        try:
            if ser.in_waiting:
                msg = ser.readline().decode('utf-8').strip()
                if msg:
                    print(f"Pico says: {msg}")
        except Exception as e:
            print(f"Error in serial thread: {e}")
            break

listener_thread = threading.Thread(target=serial_listener, daemon=True)
listener_thread.start()

# ============ Run GUI ============
root.mainloop()

# Close serial on exit
if ser and ser.is_open:
    ser.close()
    print("Serial connection closed.")
