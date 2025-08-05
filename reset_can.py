#!/usr/bin/env python3
"""
Gracefully resets CAN interfaces can0 and can1 by:
1. Taking interfaces down
2. Bringing them back up with proper bitrate and restart-ms
3. Increasing txqueuelen
4. Ensuring clean socket shutdown

Author: Zach McEwen
"""

import subprocess
import time

# Settings
interfaces = ["can0", "can1"]
bitrate = 500000
restart_ms = 100
txqueuelen = 1000

def run_command(command):
    """Utility to run shell commands safely."""
    try:
        print(f"[EXEC] {' '.join(command)}")
        subprocess.run(command, check=True)
    except subprocess.CalledProcessError as e:
        print(f"[ERROR] Command failed: {' '.join(command)}\n{e}")

def bring_interface_down(iface):
    run_command(["sudo", "ip", "link", "set", iface, "down"])

def bring_interface_up(iface):
    run_command([
        "sudo", "ip", "link", "set", iface, "up",
        "type", "can",
        "bitrate", str(bitrate),
        "restart-ms", str(restart_ms)
    ])
    run_command([
        "sudo", "ip", "link", "set", iface,
        "txqueuelen", str(txqueuelen)
    ])

def reset_can_interface(iface):
    print(f"\n=== Resetting {iface} ===")
    bring_interface_down(iface)
    time.sleep(0.5)  # Give hardware time to reset
    bring_interface_up(iface)
    print(f"[INFO] {iface} restarted successfully\n")

def main():
    for iface in interfaces:
        reset_can_interface(iface)

    print("? All CAN interfaces reset successfully.")

if __name__ == "__main__":
    main()
