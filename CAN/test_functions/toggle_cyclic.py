import sys
import can

# =====================[ CONFIGURATION ]=====================
# CAN interface name used by the system
CAN_INTERFACE = 'can0'

# BAMOCAR motor controller settings
BAMOCAR_RX_ID = 0x201          # Controller's arbitration ID
REQ_REG_ID = 0x3D              # Register to request cyclic data

# BAMOCAR Register IDs for different parameters
SPEED_REG_ID = 0x30            # Speed register
VOLTAGE_REG_ID = 0xEB          # Voltage register (optional if used)
CURRENT_REG_ID = 0x20          # Current register
VQ_REG_ID = 0x29               # Vq (quadrature voltage)
VD_REG_ID = 0x2A               # Vd (direct axis voltage)

# =====================[ FUNCTION: SEND CYCLIC REQUEST ]=====================
def send_cyclic_request(bus, reg_id, enable):
    """
    Sends a CAN message to enable or disable cyclic updates for a specific register.

    Args:
        bus: Initialized CAN bus interface.
        reg_id: Register ID to control.
        enable: Boolean; True to enable cyclic, False to disable.

    Sends:
        CAN message formatted as [REQ_REG_ID, reg_id, period], where:
        - period = 100 ms for cyclic enable
        - period = 0xFF to disable
    """
    period = 100 if enable else 0xFF
    msg = can.Message(
        arbitration_id=BAMOCAR_RX_ID,
        data=[REQ_REG_ID, reg_id, period],
        is_extended_id=False
    )
    try:
        bus.send(msg)
        print(f"[SENT] {'Enable' if enable else 'Disable'} reg 0x{reg_id:02X} ? Data: {msg.data.hex()}")
    except can.CanError as e:
        print(f"[ERROR] Failed to send message for reg 0x{reg_id:02X}: {e}")

# =====================[ FUNCTION: SEND ALL REQUESTS ]=====================
def send_all_requests(enable):
    """
    Initializes the CAN bus and sends cyclic update requests for multiple parameters.
    """
    try:
        bus = can.interface.Bus(channel=CAN_INTERFACE, bustype='socketcan')
        print(f"[INFO] {'Enabling' if enable else 'Disabling'} cyclic updates on {CAN_INTERFACE}...")

        # Send cyclic update request for each desired register
        send_cyclic_request(bus, SPEED_REG_ID, enable)
        send_cyclic_request(bus, CURRENT_REG_ID, enable)
        send_cyclic_request(bus, VQ_REG_ID, enable)
        send_cyclic_request(bus, VD_REG_ID, enable)

    except Exception as e:
        print(f"[ERROR] CAN initialization or message failure: {e}")

# =====================[ ENTRY POINT ]=====================
if __name__ == "__main__":
    # Verify proper command-line arguments
    if len(sys.argv) < 2:
        print("[ERROR] Usage: python3 toggle_cyclic.py <enable|disable>")
        sys.exit(1)

    # Normalize input mode
    mode = sys.argv[1].strip().lower()
    if mode not in ["enable", "disable"]:
        print(f"[ERROR] Invalid mode: '{mode}'. Use 'enable' or 'disable'.")
        sys.exit(1)

    # Execute command
    send_all_requests(mode == "enable")
