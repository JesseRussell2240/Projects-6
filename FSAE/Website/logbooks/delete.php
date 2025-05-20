<?php
session_start();
require 'db.php';

if (!isset($_SESSION['user_id'])) {
    die("Not logged in.");
}

$id = $_POST['id'] ?? 0;

// Check ownership
$stmt = $pdo->prepare("SELECT user_id FROM entries WHERE id = ?");
$stmt->execute([$id]);
$row = $stmt->fetch();

if ($row && $row['user_id'] == $_SESSION['user_id']) {
    $stmt = $pdo->prepare("DELETE FROM entries WHERE id = ?");
    $stmt->execute([$id]);
}

header("Location: index.php");
exit;
?>
