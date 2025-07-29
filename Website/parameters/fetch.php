<?php
// Define database connection parameters
$host = 'localhost';             // Host where the database server is located
$db   = 'logbook_db';           // Name of the database to connect to
$user = 'loguser';              // Username with access permissions to the database
$pass = 'CFE';                  // Password for the database user
$charset = 'utf8mb4';           // Character encoding to ensure compatibility with special characters

// Construct the Data Source Name (DSN) for PDO
$dsn = "mysql:host=$host;dbname=$db;charset=$charset";

// Set PDO options for error handling and query preparation
$options = [
    PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION  // Enable exceptions for database errors
];

try {
    // Establish a new PDO database connection using the credentials and options defined above
    $pdo = new PDO($dsn, $user, $pass, $options);

    // Prepare and execute a SQL query to fetch all parameter names and their corresponding values
    $stmt = $pdo->query("SELECT param_name, param_value FROM motor_parameters");

    // Initialize an associative array to hold the parameters
    $params = [];

    // Iterate over the result set, storing each parameter in the array using the parameter name as the key
    while ($row = $stmt->fetch()) {
        $params[$row['param_name']] = $row['param_value'];
    }

} catch (PDOException $e) {
    // If a database connection or query error occurs, terminate the script and show an error message
    die("DB error: " . $e->getMessage());
}
?>
