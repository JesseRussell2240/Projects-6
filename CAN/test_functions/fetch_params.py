import mysql.connector

#COnnects to the MariaDB and retirives motor controller parameters.
#Returns: A dictionary with parameter names and keys and their corresponding values

def fetch_motor_parameters():

	# Database connection configuration
	host = 'localhost'
	user = 'loguser'           # Replace with your actual username
	password = 'CFE'    # Replace with your actual password
	database = 'logbook_db'    # Replace with your actual database name

	params = {}

	try:
		#Establish connection to the MariaDB server
		conn = mysql.connector.connect(
			host=host,
            user=user,
            password=password,
            database=database
		)

		#Create Cursor to execute SQL queries
		cursor = conn.cursor()
		
		#SQL query to fetch all parameter names and values
		cursor.execute("SELECT param_name, param_value FROM motor_parameters")
		
		#Fetch all results
		results = cursor.fetchall()
		
		if results:
			for name, value in results:
				params[name] = value
		
		#Clean up
		cursor.close()
		conn.close()

	except mysql.connector.Error as err:
		print(f"Database error: {err}")
    
	return params


fetch_motor_parameters()
