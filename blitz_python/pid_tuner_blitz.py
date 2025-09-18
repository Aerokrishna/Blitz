import serial
import time
import tkinter as tk
from tkinter import ttk
import threading
from interfaces import blitz_interfaces
from blitz_serial import SerialBlitz

blitz = SerialBlitz()

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
kp_slider = ttk.Scale(root, from_=0.0, to=10.0, orient='horizontal', command=update_kp)
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
    kp = kp_slider.get()
    ki = ki_slider.get()
    kd = kd_slider.get()
    target_angle = target_slider.get()         # degrees 
    total_time = time_slider.get() # time in 0.01s

    blitz_interfaces["pid_cmd"].data = [kp, ki, kd, target_angle, total_time]

    for i in range(2):
        blitz.blitz_write(id=blitz_interfaces["pid_cmd"].id)

    print("SENDING VALUES -", 
          " KP :", kp,
          " KI :", ki,
          " KD :", kd,
          " target_angle :", target_angle,
          " time :", total_time)
          
ttk.Button(root, text="Send", command=send_pid_values).grid(row=5, column=0, columnspan=3, pady=20)

# ============ Background Serial Listener ============
def serial_listener():
    while True:
        try:
            blitz.blitz_read()
            
            if blitz_interfaces["pid_feedback"].data != None:
                time.sleep(0.003)
                print("SETPOINT : ", round(blitz_interfaces["pid_feedback"].data[0],3),
                      " CURRENT : ", round(blitz_interfaces["pid_feedback"].data[1],3),
                      " PWM : ", blitz_interfaces["pid_feedback"].data[2],
                      " ELAPSED_TIME : ", round(blitz_interfaces["pid_feedback"].data[3],3
                      )
                    )
                      
        except Exception as e:
            print(f"Error in serial thread: {e}")
            break

listener_thread = threading.Thread(target=serial_listener, daemon=True)
listener_thread.start()

# ============ Run GUI ============
root.mainloop()
