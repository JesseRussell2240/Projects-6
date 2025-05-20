<?php
session_start();
require 'db.php';

if (!isset($_SESSION['user_id'])) {
    die("Access denied. Please log in.");
}

$name = $_POST['name'] ?? '';
$message = $_POST['message'] ?? '';

if ($name && $message) {
    $stmt = $pdo->prepare("INSERT INTO entries (name, message, user_id) VALUES (?, ?, ?)");
    $stmt->execute([$name, $message, $_SESSION['user_id']]);
}

header("Location: index.php");
exit;
?>

