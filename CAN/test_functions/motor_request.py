import can

# CAN Configuration
CAN_INTERFACE = 'can1'      # Use your CAN interface here
BAMOCAR_RX_ID = 0x201       # BAMOCAR receive address
SPEED_REG_ID = 0x30         # Register ID for actual speed
REQ_REG_ID = 0x3D           # BAMOCAR command to request value(s)

def send_speed_request(mode):
    """
    Send a CAN message to BAMOCAR based on the selected mode:
    - 'once'  : Single request of actual speed
    - 'cyclic': Periodic request of actual speed
    """
    try:
        bus = can.interface.Bus(channel=CAN_INTERFACE, bustype='socketcan')

        if mode == 'once':
            data = [REQ_REG_ID, SPEED_REG_ID, 0x00]  # Request speed once
            print("[INFO] Sending single speed request...")
        elif mode == 'cyclic':
            period_ms = 10  # Send every 10 ms
            data = [REQ_REG_ID, SPEED_REG_ID, period_ms]
            print(f"[INFO] Enabling cyclic speed transmission every {period_ms} ms...")
        else:
            print("[ERROR] Unknown mode selected.")
            return

        msg = can.Message(arbitration_id=BAMOCAR_RX_ID, data=data, is_extended_id=False)
        bus.send(msg)
        print(f"[SENT] {msg.data.hex()} to ID 0x{msg.arbitration_id:X}")

    except can.CanError as e:
        print(f"[CAN ERROR] {e}")
    except Exception as ex:
        print(f"[ERROR] {ex}")

# ========== Main User Prompt ==========
if __name__ == "__main__":
    print("Choose an action:")
    print("1. Request speed once")
    print("2. Enable cyclic speed transmission")

    choice = input("Enter 1 or 2: ").strip()

    if choice == "1":
        send_speed_request('once')
    elif choice == "2":
        send_speed_request('cyclic')
    else:
        print("Invalid input. Please enter 1 or 2.")
