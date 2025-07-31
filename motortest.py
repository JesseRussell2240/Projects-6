import can
import time
import tkinter as tk
from tkinter import messagebox
from abc import ABC, abstractmethod

# Custom exceptions
class CANCommunicationError(Exception):
    pass

class InvalidRPMError(Exception):
    def __init__(self, rpm, message="RPM must be between 0 and 8000"):
        self.rpm = rpm
        self.message = message
        super().__init__(f"{message}: {rpm}")

# Abstract CAN device
class CANDevice(ABC):
    def __init__(self, interface: str):
        self.bus = can.interface.Bus(channel=interface, bustype='socketcan')

    def send(self, arbitration_id: int, data: list):
        msg = can.Message(arbitration_id=arbitration_id, data=data, is_extended_id=False)
        try:
            self.bus.send(msg)
        except can.CanError as e:
            raise CANCommunicationError(f"Failed to send CAN message: {e}")

# Motor controller class
class MotorController(CANDevice):
    def __init__(self, interface='can1', tx_id=0x201):
        super().__init__(interface)
        self.tx_id = tx_id
        self.enable_reg = 0x51
        self.speed_reg = 0x31

    def enable(self):
        data = [self.enable_reg, 0x01, 0x00]
        self.send(self.tx_id, data)

    def disable(self):
        data = [self.enable_reg, 0x04, 0x00]
        self.send(self.tx_id, data)

    def set_rpm(self, rpm):
        if rpm < 0 or rpm > 8000:
            raise InvalidRPMError(rpm)
        data = [self.speed_reg, rpm & 0xFF, (rpm >> 8) & 0xFF]
        self.send(self.tx_id, data)

# GUI using tkinter
motor = MotorController()

def start_motor():
    try:
        motor.enable()
        motor.set_rpm(6000)  # You can also use a user-input value here
        messagebox.showinfo("Motor", "Motor started at 6000 RPM")
    except (CANCommunicationError, InvalidRPMError) as e:
        messagebox.showerror("Error", str(e))

def stop_motor():
    try:
        motor.disable()
        messagebox.showinfo("Motor", "Motor stopped")
    except CANCommunicationError as e:
        messagebox.showerror("Error", str(e))

# Build UI
root = tk.Tk()
root.title("Motor Test with Exception Handling")

frame = tk.Frame(root, padx=20, pady=20)
frame.pack()

tk.Label(frame, text="Motor Controller", font=("Arial", 16)).pack(pady=10)
tk.Button(frame, text="Start Motor", font=("Arial", 14), command=start_motor).pack(pady=5)
tk.Button(frame, text="Stop Motor", font=("Arial", 14), command=stop_motor).pack(pady=5)
tk.Button(frame, text="Exit", font=("Arial", 12), command=root.destroy).pack(pady=10)

root.mainloop()
