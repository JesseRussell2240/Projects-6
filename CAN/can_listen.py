# can_listener.py
import can
import cantools
import struct
import math
import mysql.connector
from datetime import datetime
from can_logger import update_live_can_table

# === Load the unified DBC file ===
db = cantools.database.load_file('rpi2.dbc')  # Both BAMOCAR and Orion messages

# === Set up CAN bus interface ===
bus = can.Bus(interface='socketcan', channel='can1')

NMAX_RPM = 6600

# === Set up MySQL connection ===
conn = mysql.connector.connect(
    host="localhost",
    user="loguser",
    password="CFE",
    database="logbook_db"
)
cursor = conn.cursor()

def write_to_db(parameter, value):
    timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    query = """
        INSERT INTO live_data (parameter, value, timestamp)
        VALUES (%s, %s, %s)
        ON DUPLICATE KEY UPDATE
            value = VALUES(value),
            timestamp = VALUES(timestamp);
    """
    cursor.execute(query, (parameter, value, timestamp))
    conn.commit()

def listen_can():
    msg = bus.recv(timeout=1)
    last_vq = None
    last_vd = None
    
    if msg is None:
        return

    update_live_can_table(msg)
    print(f"[DEBUG] Raw 0x{msg.arbitration_id:X}: {msg.data.hex()} (len={msg.dlc})")

    try:
        decoded = db.decode_message(msg.arbitration_id, msg.data)

        if msg.arbitration_id == 0x701:
            soc = decoded['Pack_SOC']
            print(f"[INFO] BMS SOC: {soc:.1f} %")
            write_to_db("soc", soc)

        elif msg.arbitration_id == 0x181:
            raw = decoded['RawValue']
            reg_id = msg.data[0]

            if reg_id == 0x30:
                raw_speed = struct.unpack_from('<h', msg.data, 1)[0]
                scaled_speed = (raw_speed / 32767.0) * NMAX_RPM
                print(f"[INFO] Speed: {scaled_speed:.2f} rpm")
                write_to_db("rpm", scaled_speed)

            elif reg_id == 0x29:  # Vq
                vq_raw = struct.unpack_from('<h', msg.data, 1)[0]
                last_vq = vq_raw / 10.0  # Apply scaling

            elif reg_id == 0x2A:  # Vd
                vd_raw = struct.unpack_from('<h', msg.data, 1)[0]
                last_vd = vd_raw / 10.0  # Apply scaling

            # If both Vq and Vd are available, compute Vtotal
            if last_vq is not None and last_vd is not None:
                v_total = math.sqrt(last_vq**2 + last_vd**2)
                print(f"[INFO] Vtotal: {v_total:.2f} V")
                write_to_db("voltage", v_total)
                last_vq = None  # Reset after use to avoid duplicates
                last_vd = None


            else:
                print(f"[INFO] Unknown RegisterID: 0x{reg_id:X}, Raw: {raw}")

        elif msg.arbitration_id == 0x110:
            freq = decoded.get("PWM_Freq")
            duty = decoded.get("PWM_Duty")
            status = decoded.get("PWM_Status")
            print(f"[INFO] PWM FREQ: {freq} Hz")
            print(f"[INFO] PWM DUTY: {duty:.1f} %")
            print(f"[INFO] PWM STATUS: {status}")
            write_to_db("pwm_freq", freq)
            write_to_db("pwm_duty", duty)
            write_to_db("pwm_status", status)

        else:
            print(f"[INFO] CAN ID 0x{msg.arbitration_id:X} decoded: {decoded}")

    except Exception as e:
        print(f"[ERROR] Failed to decode message 0x{msg.arbitration_id:X}: {e}")
