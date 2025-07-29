<?php
$host = 'localhost';
$db   = 'logbook_db';
$user = 'loguser';           // Make sure you create this user in MariaDB and give it access
$pass = 'secretpass';        // Use your actual password!
$charset = 'utf8mb4';

$dsn = "mysql:host=$host;dbname=$db;charset=$charset";
$options = [PDO::ATTR_ERRMODE => PDO::ERRMODE_EXCEPTION];

try {
    $pdo = new PDO($dsn, $user, $pass, $options);

    $params = ['speedSetpoint', 'torqueSetpoint', 'imax', 'icont', 'udcMin', 'udcMax', 'tempMax',
               'n100', 'nLimPos', 'nLimNeg', 'kacc', 'kdec', 'kp', 'ki', 'kd', 'canBaud', 'nodeId'];

    foreach ($params as $p) {
        if (isset($_POST[$p])) {
            $stmt = $pdo->prepare("INSERT INTO motor_parameters (param_name, param_value) VALUES (?, ?)");
            $stmt->execute([$p, $_POST[$p]]);
        }
    }

    header("Location: parameters.html?status=success");
} catch (PDOException $e) {
    echo "DB error: " . $e->getMessage();
}
?>
