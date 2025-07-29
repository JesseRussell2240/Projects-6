<?php
$logFolder = __DIR__ . '/logs/';
if (!isset($_GET['file'])) {
  http_response_code(400);
  echo "Missing filename.";
  exit;
}
$filename = basename($_GET['file']);
$fullPath = $logFolder . $filename;
if (!file_exists($fullPath)) {
  http_response_code(404);
  echo "File not found.";
  exit;
}
$logData = json_decode(file_get_contents($fullPath), true);
header('Content-Type: text/csv');
header('Content-Disposition: attachment; filename="' . pathinfo($filename, PATHINFO_FILENAME) . '.csv"');
$output = fopen('php://output', 'w');
fputcsv($output, ['Parameter', 'Value', 'Timestamp']);
foreach ($logData as $row) {
  fputcsv($output, [$row['parameter'], $row['value'], $row['timestamp']]);
}
fclose($output);
