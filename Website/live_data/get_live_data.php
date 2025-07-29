<?php
header('Content-Type: application/json');

// Load database credentials and connection as $pdo
require '../logbooks/db.php';

try {
    $stmt = $pdo->query('SELECT parameter, value FROM live_data');
    $rows = $stmt->fetchAll();

    $output = [];

    foreach ($rows as $row) {
        $output[$row['parameter']] = $row['value'];
    }

    echo json_encode($output);

} catch (PDOException $e) {
    http_response_code(500);
    echo json_encode(['error' => 'Database query failed: ' . $e->getMessage()]);
}
