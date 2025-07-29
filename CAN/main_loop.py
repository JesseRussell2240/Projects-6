# main_loop.py
import time
from can_listen import listen_can

def master_loop():
    print("[INFO] Starting master loop...")
    try:
        while True:
            listen_can()  # CAN listener
            # Placeholder for other tasks
            # update_dashboard()
            # log_to_sd_card()
            # check_inputs()
            time.sleep(0.05)  # Small delay to reduce CPU usage

    except KeyboardInterrupt:
        print("\n[INFO] Master loop interrupted. Shutting down gracefully...")

if __name__ == "__main__":
    master_loop()
