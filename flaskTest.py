from flask import Flask, request, jsonify
import can

app = Flask(__name__)

# CAN Config
CAN_INTERFACE = 'can0'
TX_ID = 0x201
ENABLE_CMD_REGID = 0x51
SPEED_CMD_REGID = 0x31
ENABLE_VALUE = 0x0001
DISABLE_VALUE = 0x0004
RPM_VALUE = 6000

bus = can.interface.Bus(channel=CAN_INTERFACE, bustype='socketcan')

def send_enable_command(enable: bool):
    value = ENABLE_VALUE if enable else DISABLE_VALUE
    data = [ENABLE_CMD_REGID, value & 0xFF, (value >> 8) & 0xFF]
    msg = can.Message(arbitration_id=TX_ID, data=data, is_extended_id=False)
    bus.send(msg)

def send_rpm_command(rpm: int):
    data = [SPEED_CMD_REGID, rpm & 0xFF, (rpm >> 8) & 0xFF]
    msg = can.Message(arbitration_id=TX_ID, data=data, is_extended_id=False)
    bus.send(msg)

@app.route('/motor/start', methods=['POST'])
def start_motor():
    try:
        send_enable_command(True)
        send_rpm_command(RPM_VALUE)
        return jsonify({'status': 'Motor started'}), 200
    except Exception as e:
        return jsonify({'error': str(e)}), 500

@app.route('/motor/stop', methods=['POST'])
def stop_motor():
    try:
        send_enable_command(False)
        return jsonify({'status': 'Motor stopped'}), 200
    except Exception as e:
        return jsonify({'error': str(e)}), 500

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
