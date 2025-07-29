import sys
import can

# CAN Configuration
CAN_INTERFACE = 'can0'
BAMOCAR_RX_ID = 0x201
REQ_REG_ID = 0x3D  # Command to request cyclic or single-shot data

# Register IDs
SPEED_REG_ID = 0x30
VOLTAGE_REG_ID = 0xEB
CURRENT_REG_ID = 0x20
VQ_REG_ID = 0x29
VD_REG_ID = 0x2A

# Shared function
def send_cyclic_request(bus, reg_id, enable):
    period = 100 if enable else 0xFF  # 0xFF disables cyclic
    msg = can.Message(
        arbitration_id=BAMOCAR_RX_ID,
        data=[REQ_REG_ID, reg_id, period],
        is_extended_id=False
    )
    bus.send(msg)
    print(f"[SENT] {'Enable' if enable else 'Disable'} for reg 0x{reg_id:02X} ? Data: {msg.data.hex()}")

# Main handler
def send_all_requests(enable):
    try:
        bus = can.interface.Bus(channel=CAN_INTERFACE, bustype='socketcan')

        print(f"[INFO] {'Enabling' if enable else 'Disabling'} cyclic updates...")
        send_cyclic_request(bus, SPEED_REG_ID, enable)
        send_cyclic_request(bus, CURRENT_REG_ID, enable)
        send_cyclic_request(bus, VQ_REG_ID, enable)
        send_cyclic_request(bus, VD_REG_ID, enable)

# CLI entrypoint
if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("[ERROR] Usage: python3 toggle_cyclic.py <enable|disable>")
        sys.exit(1)

    mode = sys.argv[1].strip().lower()

    if mode not in ["enable", "disable"]:
        print(f"[ERROR] Invalid mode: {mode}")
        sys.exit(1)

    send_all_requests(mode == "enable")
