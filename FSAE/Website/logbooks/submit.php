<?php
session_start();
require 'db.php';

$name = $_POST['name'] ?? 'Anonymous';
$message = $_POST['message'] ?? '';
$user_id = $_SESSION['user_id'] ?? null;

if (!empty($message)) {
    $stmt = $pdo->prepare("INSERT INTO entries (name, message, user_id) VALUES (?, ?, ?)");
    $stmt->execute([$name, $message, $user_id]);
}

header("Location: index.php");
exit;
?>
