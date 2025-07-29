import can
import time

def main():
    # Setup CAN interface (socketcan uses Linux can0 interface)
    bus = can.interface.Bus(channel='can0', bustype='socketcan')

    # Create a CAN message
    msg = can.Message(
        arbitration_id=0x123,            # 11-bit CAN ID
        data=[0x11, 0x69, 0x69, 0x42],   # Up to 8 bytes of data
        
        is_extended_id=False             # Standard frame
    )

    print("Starting CAN frame sender... (Press Ctrl+C to stop)")
    try:
        while True:
            bus.send(msg)
            print(f"Sent: ID=0x{msg.arbitration_id:X}, Data={msg.data}")
            time.sleep(1.1)  # Wait 1 second
    except KeyboardInterrupt:
        print("\nStopped by user.")
    except can.CanError as e:
        print(f"CAN error: {e}")

if __name__ == "__main__":
    main()
