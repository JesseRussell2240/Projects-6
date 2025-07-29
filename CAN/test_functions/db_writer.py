import sys
import mysql.connector
from datetime import datetime

# Validate input
if len(sys.argv) != 3:
    print("[ERROR] Usage: python3 write_to_db.py <parameter_name> <value>")
    sys.exit(1)

param = sys.argv[1]
try:
    value = float(sys.argv[2])
except ValueError:
    print("[ERROR] Value must be a number.")
    sys.exit(1)

# Connect to database
conn = mysql.connector.connect(
    host="localhost",
    user="loguser",
    password="CFE",
    database="logbook_db"
)
cursor = conn.cursor()

# SQL query
query = """
INSERT INTO live_data (parameter, value, timestamp)
VALUES (%s, %s, %s)
ON DUPLICATE KEY UPDATE
    value = VALUES(value),
    timestamp = VALUES(timestamp);
"""

timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
cursor.execute(query, (param, value, timestamp))

# Finalize
conn.commit()
cursor.close()
conn.close()

print(f"[DB] Updated {param} = {value} at {timestamp}")
