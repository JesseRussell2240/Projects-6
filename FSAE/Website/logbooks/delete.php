<?php
session_start();
require 'db.php';

if (!isset($_SESSION['user_id'])) {
    die("Unauthorized");
}

$entry_id = $_POST['entry_id'] ?? 0;

// Verify ownership
$stmt = $pdo->prepare("SELECT user_id FROM entries WHERE id = ?");
$stmt->execute([$entry_id]);
$row = $stmt->fetch();

if ($row && $row['user_id'] == $_SESSION['user_id']) {
    $stmt = $pdo->prepare("DELETE FROM entries WHERE id = ?");
    $stmt->execute([$entry_id]);
}

header("Location: index.php");
exit;
?>
