<?php
header('Content-Type: application/json');
require '../logbooks/db.php';

try {
    $stmt = $pdo->query("SELECT * FROM live_can ORDER BY timestamp DESC LIMIT 10");
    echo json_encode($stmt->fetchAll(PDO::FETCH_ASSOC));
} catch (PDOException $e) {
    http_response_code(500);
    echo json_encode(['error' => 'Query failed: ' . $e->getMessage()]);
}
?>
