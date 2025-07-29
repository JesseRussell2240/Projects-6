
import can
import time
import tkinter as tk
from tkinter import messagebox

# Set CAN interface and Bamocar IDs
CAN_INTERFACE = 'can1'
TX_ID = 0x201  # Default Bamocar receive ID (commands go here)
ENABLE_CMD_REGID = 0x51  # Control word register ID
SPEED_CMD_REGID = 0x31  # Speed command register ID
ENABLE_VALUE = 0x0001  # Enable motor (bit 0 = 1)
DISABLE_VALUE = 0x0004  # Disable motor
RPM_VALUE = 200  # Desired speed in RPM (100%)

# Setup CAN bus
bus = can.interface.Bus(channel=CAN_INTERFACE, bustype='socketcan')

def send_enable_command(enable: bool):
    value = ENABLE_VALUE if enable else DISABLE_VALUE
    data = [ENABLE_CMD_REGID, value & 0xFF, (value >> 8) & 0xFF]
    msg = can.Message(arbitration_id=TX_ID, data=data, is_extended_id=False)
    try:
        bus.send(msg)
        print("Sent {} command".format("ENABLE" if enable else "DISABLE"))
    except can.CanError as e:
        print("CAN send failed:", e)
        messagebox.showerror("CAN Error", f"Failed to send command: {e}")

def send_rpm_command(rpm: int):
    data = [SPEED_CMD_REGID, rpm & 0xFF, (rpm >> 8) & 0xFF]
    msg = can.Message(arbitration_id=TX_ID, data=data, is_extended_id=False)
    try:
        bus.send(msg)
        print(f"Sent RPM command: {rpm} RPM")
    except can.CanError as e:
        print("CAN send failed:", e)
        messagebox.showerror("CAN Error", f"Failed to send RPM: {e}")

# Tkinter GUI
root = tk.Tk()
root.title("Bamocar Motor Control")

frame = tk.Frame(root, padx=20, pady=20)
frame.pack()

title = tk.Label(frame, text="Bamocar PG-D3 Motor Control", font=("Arial", 16))
title.pack(pady=10)

start_button = tk.Button(frame, text="Start Motor", font=("Arial", 14), width=15,
                          command=lambda: [send_enable_command(True), send_rpm_command(RPM_VALUE)])
start_button.pack(pady=5)

stop_button = tk.Button(frame, text="Stop Motor", font=("Arial", 14), width=15,
                         command=lambda: send_enable_command(False))
stop_button.pack(pady=5)

exit_button = tk.Button(frame, text="Exit", font=("Arial", 12), command=root.destroy)
exit_button.pack(pady=10)

root.mainloop()
