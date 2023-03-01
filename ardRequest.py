import serial
import json
import mariadb
import sys
import time

# Configuration variables
ser = serial.Serial('COM3', 9600, timeout=1)
db_config = {
  'host': '127.0.0.1',
  'user': 'admin',
  'password': 'pass',
  'database': 'sensors'
}
logging_table = 'sensors'
repeat_interval = 5 # in seconds

# Function to insert data into the database
def insert_sensor_data(cursor, entity_id, entity_name, entity_value):
    try:
        cursor.execute(f"INSERT INTO {logging_table} (id, name, value) VALUES ({entity_id}, '{entity_name}', {entity_value})")
        print(f"Inserted data: {entity_id}, {entity_name}, {entity_value}")
    except mariadb.Error as e:
        print(f"Error inserting data into database: {e}")

# Connect to the database
try:
    conn = mariadb.connect(**db_config)
except mariadb.Error as e:
    print(f"Error connecting to MariaDB: {e}")
    sys.exit(1)

# Set up the database cursor
cursor = conn.cursor()

def askfor(request :str):
    response = None
    ser.write(request.encode())  # send request to serial
    while not response:
        response = ser.readline().decode('utf-8').strip()  # read response from serial
    return json.loads(response)  # parse json
    

# Main loop
while True:
    # Send "get all" command to serial port
    try:
        for entity in askfor("command get -all\r\n"):
            entity_id = entity['id']
            entity = askfor(f"command get {entity_id}\r\n")[0]
            try:
                insert_sensor_data(cursor, entity_id, entity['name'], entity['value'])
            except KeyError as ke:
                print(f"invalid key'{ke.args}' in response: {entity}")
            conn.commit()
    except KeyboardInterrupt:
        break
    time.sleep(repeat_interval)
    

# Close the database connection
conn.close()
