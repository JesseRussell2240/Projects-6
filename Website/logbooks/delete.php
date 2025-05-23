<?php
session_start();
require 'db.php';
if (!isset($_SESSION['user_id'])) {
    header("Location: login.php");
    exit;
}
if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['entry_id'])) {
    $entry_id = $_POST['entry_id'];
    $stmt = $pdo->prepare("DELETE FROM entries WHERE id = ? AND user_id = ?");
    $stmt->execute([$entry_id, $_SESSION['user_id']]);
}
header("Location: index.php");
exit;
?>

