import mysql.connector
from datetime import datetime

# === Set up MySQL connection ===
conn = mysql.connector.connect(
    host="localhost",
    user="loguser",
    password="CFE",
    database="logbook_db"
)
cursor = conn.cursor()

def update_live_can_table(msg):
    arbitration_id = msg.arbitration_id
    data = list(msg.data) + [0] * (8 - len(msg.data))  # pad to 8 bytes
    timestamp = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    count = 1

    # Step 1: Always insert a new row
    insert_query = """
        INSERT INTO live_can (
            id, length, byte0, byte1, byte2, byte3,
            byte4, byte5, byte6, byte7,
            count, timestamp
        ) VALUES (
            %s, %s, %s, %s, %s, %s,
            %s, %s, %s, %s,
            %s, %s
        )
    """
    cursor.execute(insert_query, (
        arbitration_id, msg.dlc, *data, count, timestamp
    ))
    conn.commit()

    # Step 2: Trim table to max 10 rows
    cursor.execute("SELECT COUNT(*) FROM live_can")
    row_count = cursor.fetchone()[0]

    if row_count > 10:
        delete_query = """
            DELETE FROM live_can
            ORDER BY timestamp ASC
            LIMIT %s
        """
        cursor.execute(delete_query, (row_count - 10,))
        conn.commit()

    print(f"[SQL] Inserted ID 0x{arbitration_id:X} at {timestamp}")

