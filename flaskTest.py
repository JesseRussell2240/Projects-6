from flask import Flask, jsonify, request
import can
from abc import ABC, abstractmethod

# Base class
class CANDevice(ABC):
    def __init__(self, interface: str):
        self.bus = can.interface.Bus(channel=interface, bustype='socketcan')

    def send(self, arbitration_id: int, data: list):
        msg = can.Message(arbitration_id=arbitration_id, data=data, is_extended_id=False)
        self.bus.send(msg)

# Derived class
class MotorController(CANDevice):
    def __init__(self, interface='can0', tx_id=0x201):
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
        data = [self.speed_reg, rpm & 0xFF, (rpm >> 8) & 0xFF]
        self.send(self.tx_id, data)

# Flask App
app = Flask(__name__)
motor = MotorController()

@app.route('/motor/start', methods=['POST'])
def start_motor():
    try:
        motor.enable()
        motor.set_rpm(6000)
        return jsonify({'status': 'Motor started'})
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/motor/stop', methods=['POST'])
def stop_motor():
    try:
        motor.disable()
        return jsonify({'status': 'Motor stopped'})
    except Exception as e:
        return jsonify({'error': str(e)}), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
