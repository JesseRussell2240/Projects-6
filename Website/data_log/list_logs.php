<?php
header('Content-Type: application/json');
$logFolder = __DIR__ . '/logs/';
$files = [];
if (file_exists($logFolder)) {
  foreach (glob($logFolder . 'log_*.json') as $filepath) {
    $files[] = basename($filepath);
  }
}
echo json_encode($files);
