S<?php
// Database connection credentials
$host = 'localhost';               // Host of the database server (usually localhost)
$db   = 'logbook_db';              // Database name where the parameters are stored
$user = 'loguser';                 // Database user (must have write permissions)
$pass = 'CFE';                     // Password for the user
$charset = 'utf8mb4';              // Character encoding for the database connection

// Data Source Name for PDO
$dsn = "mysql:host=$host;dbname=$db;charset=$charset";

// PDO connection options
$options = [
    PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION   // Throw exceptions on database errors
];

//$pdo->beginTransaction();
try {
    // Establish PDO connection
    $pdo = new PDO($dsn, $user, $pass, $options);

    // Define the valid parameter ranges for each key
    $validRanges = [
        'speedSetpoint' => [0, 10000],     // in RPM
        'torqueSetpoint' => [0, 100],      // as a percentage
        'imax' => [0, 500],                // in Amperes
        'icont' => [0, 300],               // in Amperes
        'udcMin' => [50, 500],             // in Volts
        'udcMax' => [50, 500],             // in Volts
        'tempMax' => [0, 15000],             // in Celsius
        'n100' => [0, 10000],              // max RPM reference
        'nLimPos' => [0, 100],             // % limit
        'nLimNeg' => [0, 100],             // % limit
        'kacc' => [0, 10000],              // Acceleration time (ms)
        'kdec' => [0, 10000],              // Deceleration time (ms)
        'kp' => [0, 1000],                 // PID proportional gain
        'ki' => [0, 1000],                 // PID integral gain
        'kd' => [0, 1000],                 // PID derivative gain
        'canBaud' => [0x4000, 0x4019],     // Acceptable CAN baudrate hex range
        'nodeId' => [0, 127]               // Valid CAN Node IDs
    ];

    // Extract just the parameter names
    $params = array_keys($validRanges);

    // Loop through all defined parameters
    foreach ($params as $p) {
        if (isset($_POST[$p])) {
            $val = $_POST[$p];

            // Validate against range
            if ($val < $validRanges[$p][0] || $val > $validRanges[$p][1]) {
                // Redirect to UI with an error flag if out of range
                header("Location: ../parameters.php?error=$p");
                exit;
            }

            // Insert new or update existing parameter value using ON DUPLICATE KEY
            $stmt = $pdo->prepare(
                "INSERT INTO motor_parameters (param_name, param_value) VALUES (?, ?)
                 ON DUPLICATE KEY UPDATE param_value = VALUES(param_value)"
            );

            // Execute the query with parameter name and value
            $stmt->execute([$p, $val]);
        }
    }

    // If everything is valid, redirect to success
    //$pdo->commit();
    header("Location: ../parameters.php?status=success");
    exit;

} catch (PDOException $e) {
    // Handle database connection or query error
    echo "DB error: " . $e->getMessage();
    //$pdo->rollBack();
}
?>
