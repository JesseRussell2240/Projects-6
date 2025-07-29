# ===================== Imports ==========================
import struct
import can
import mysql.connector
import time

# ================== Register Lookup Table ===============
# BAMOCAR D3 Register Map: param_name -> (index, subindex, size)
register_lookup = {
    'speedsetpoint':    0x31,
    'torquesetpoint':   2,
    'imax':             0x4d,
    'icont':            0x4e,
    'udcmin':           2,
    'udcmax':           2,
    'tempmax':          0xa3,
    'n100':             0xc8,
    'nlimpos':          0x3F,
    'nlimneg':          0x3E,
    'kacc':             0x5b,
    'kdec':             0xed,
    'kp':               0x2c,
    'ki':               0x2d,
    'kd':               0x2e, 
    'canbaud':          2,
    'nodeid':           1
}

# ============= Function: Fetch Parameters from DB ==========
def fetch_motor_parameters():
    """
    Connects to MariaDB and retrieves motor controller parameters.
    Returns:
        dict: param_name -> param_value
    """
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
            params[name.lower()] = int(value)  # Ensure lowercase and convert to int

        cursor.close()
        conn.close()

    except mysql.connector.Error as err:
        print(f"Database error: {err}")

    return params
    
# ========== CAN Frame Builder Function ==========
def build_can_frame(param, value, node_id=0x201):
    """
    Builds a 3-byte CAN frame using:
    - Byte 0: Register ID (RegNr from register_lookup)
    - Byte 1: LSB of value
    - Byte 2: MSB of value

    Args:
        param (str): Parameter name (e.g., 'kp', 'imax')
        value (int): Integer value to write
        node_id (int): Destination CAN ID (default = 0x201 for motor controller)

    Returns:
        can.Message: CAN frame ready to transmit
    """
    key = param.lower()

    if key not in register_lookup:
        print(f"[WARN] Unknown parameter: '{param}' (skipped)")
        return None

    reg_id = register_lookup[key]

    if not (0 <= reg_id <= 255):
        print(f"[ERROR] RegNr {reg_id} for '{param}' is out of byte range (0-255)")
        return None

    # Pack data as 3 bytes: [RegNr, LSB(value), MSB(value)]
    data = bytearray([
        reg_id & 0xFF,
        value & 0xFF,
        (value >> 8) & 0xFF
    ])

    return can.Message(arbitration_id=node_id, data=data, is_extended_id=False)

    
# ========== Main Execution ==========
if __name__ == '__main__':
    try:
        # Connect to the CAN interface (Linux socketcan interface must be configured)
        bus = can.interface.Bus(channel='can1', bustype='socketcan')

        print("[INFO] Fetching parameters from database...")
        params = fetch_motor_parameters()  # Get updated parameter values

        print("[INFO] Sending parameters over CAN bus...")
        for param, value in params.items():
            msg = build_can_frame(param, value)
            if msg:
                try:
                    bus.send(msg)  # Transmit the CAN frame
                    print(f"[SENT] {param.upper()} -> ID: 0x{msg.arbitration_id:X} Data: {msg.data.hex()}")
                    time.sleep(0.01)
                    
                except can.CanError as e:
                    print(f"[CAN ERROR] Could not send message for '{param}': {e}")
        
        bus.shutdown()
            
    except Exception as ex:
        print(f"[FATAL ERROR] {ex}")
    
