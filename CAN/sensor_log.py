import time
import json
import board
import busio
import adafruit_bno08x
from adafruit_bno08x.i2c import BNO08X_I2C

# Set up I2C and sensor
i2c = busio.I2C(board.SCL, board.SDA, frequency=400000)
bno = BNO08X_I2C(i2c)

# Enable gyroscope and accelerometer
bno.enable_feature(adafruit_bno08x.BNO_REPORT_ACCELEROMETER)
bno.enable_feature(adafruit_bno08x.BNO_REPORT_GYROSCOPE)

# Output file
json_path = "/home/CFE/Projects-6/Website/9DOF.json"

# Main loop
def main():
    while True:
        try:
            # Get gyro and accel
            gyro_x, gyro_y, gyro_z = bno.gyro
            acc_x, acc_y, acc_z = bno.acceleration

            # Prepare data
            sensor_data = {
                "gyro": {
                    "x": gyro_x,
                    "y": gyro_y,
                    "z": gyro_z
                },
                "acceleration": {
                    "x": acc_x,
                    "y": acc_y,
                    "z": acc_z
                },
                "timestamp": time.time()
            }

            # Write to JSON file
            with open(json_path, "w") as f:
                json.dump(sensor_data, f, indent=4)
            
            # Write to JSON file
            with open(json_path, "w") as f:
                json.dump(sensor_data, f, indent=4)    

            print(f"Updated: {sensor_data}")
            time.sleep(0.1)

        except Exception as e:
            print(f"Sensor error: {e}")
            time.sleep(1)

if __name__ == "__main__":
    main()
