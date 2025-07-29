<?php
header('Content-Type: application/json');

$flagPath = __DIR__ . '/log_active.flag';
file_put_contents($flagPath, date('Y-m-d H:i:s'));

echo json_encode(['status' => 'started']);
