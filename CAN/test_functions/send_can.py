# ===================== Imports ==========================
import struct
import can
import mysql.connector
import time

# ================== Register Lookup Table ===============
register_lookup = {
    #'speedsetpoint':     0x31,
    #'torquesetpoint':    0x32,
    'motor_enable':      0x51,
    'imax':              0x4D,
    'icont':             0x4E,
    'udcmin':            0x6E,
    'udcmax':            0x6F,
    'n100':              0xC8,
    'nlimpos':           0x3F,
    'nlimneg':           0x3E,
    'kacc':              0x5B,
    'kdec':              0xED,
    'kp':                0x2C,
    'ki':                0x2D,
    'kd':                0x2E,
    'canbaud':           0x71
}


# ============= Fetch Parameters from DB ==================
def fetch_motor_parameters():
    host = 'localhost'
    user = 'loguser'
    password = 'CFE'
    database = 'logbook_db'
    params = {}

    try:
        conn = mysql.connector.connect(
            host=host,
            user=user,
            password=password,
            database=database
        )
        cursor = conn.cursor()
        cursor.execute("SELECT param_name, param_value FROM motor_parameters")
        results = cursor.fetchall()

        for name, value in results:
            params[name.lower()] = int(value)  # Force lowercase and int

        cursor.close()
        conn.close()

    except mysql.connector.Error as err:
        print(f"[DB ERROR] {err}")

    return params


# ================== Build CAN Frame ======================
def build_can_frame(param, value, node_id=0x201):
    key = param.lower()
    if key not in register_lookup:
        #print(f"[SKIPPED] Unknown parameter: '{param}'")
        return None

    reg_id = register_lookup[key]
    if not isinstance(reg_id, int) or not (0 <= reg_id <= 255):
        #print(f"[ERROR] Invalid register ID for '{param}': {reg_id}")
        return None

    data = bytearray([
        reg_id & 0xFF,
        value & 0xFF,
        (value >> 8) & 0xFF
    ])

    return can.Message(arbitration_id=node_id, data=data, is_extended_id=False)


# ==================== Main Execution =====================
if __name__ == '__main__':
    try:
        bus = can.interface.Bus(channel='can0', bustype='socketcan')
        #print(f"[INFO] Using CAN interface: {bus.channel_info}")
        
        #print("[INFO] Fetching parameters from database...")
        params = fetch_motor_parameters()
        #print("[DEBUG] Parameters fetched:", params)

        #print("[INFO] Sending parameters over CAN bus...")
        success_count = 0
        fail_count = 0

        for param, value in params.items():
            #print(f"[DEBUG] Building: {param} = {value}")
            msg = build_can_frame(param, value)

            if msg:
                try:
                    bus.send(msg, timeout=1.0)
                    #print(f"[SENT ?] {param.upper()} -> ID: 0x{msg.arbitration_id:X} Data: {msg.data.hex()}")
                    success_count += 1
                except can.CanError as e:
                    #print(f"[SENT ?] FAILED to send {param.upper()} -> {e}")
                    fail_count += 1
                time.sleep(0.01)
            else:
                fail_count += 1

       # print("\n========== SEND SUMMARY ==========")
       # print(f"? Sent:     {success_count}")
       # print(f"? Failed:   {fail_count}")
       # print("==================================")

        bus.shutdown()

    except Exception as ex:
        print(f"[FATAL ERROR] {ex}")
