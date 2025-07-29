<?php
header('Content-Type: application/json');

$logFolder = __DIR__ . '/logs/';
$flagPath = __DIR__ . '/log_active.flag';

if (file_exists($flagPath)) {
    unlink($flagPath);
}

$data = json_decode(file_get_contents('php://input'), true);

if (!$data || !is_array($data)) {
    http_response_code(400);
    echo json_encode(['error' => 'Invalid log data']);
    exit;
}

if (!file_exists($logFolder)) {
    if (!mkdir($logFolder, 0777, true)) {
        http_response_code(500);
        echo json_encode(['error' => 'Failed to create logs directory']);
        exit;
    }
}

$timestamp = date('Y-m-d_H-i-s');
$filename = "log_{$timestamp}.json";
$fullPath = $logFolder . $filename;

if (file_put_contents($fullPath, json_encode($data, JSON_PRETTY_PRINT)) === false) {
    http_response_code(500);
    echo json_encode(['error' => 'Failed to write log file']);
    exit;
}

echo json_encode(['status' => 'success', 'filename' => $filename]);
