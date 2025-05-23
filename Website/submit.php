<?php
session_start();
require 'db.php';
if (!isset($_SESSION['user_id'])) {
    header("Location: login.php");
    exit;
}
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $title = $_POST['title'];
    $project_section = $_POST['project_section'];
    $message = $_POST['message'];
    $user_id = $_SESSION['user_id'];

    $stmt = $pdo->prepare("INSERT INTO entries (user_id, project_section, title, message) VALUES (?, ?, ?, ?)");
    $stmt->execute([$user_id, $project_section, $title, $message]);
}
header("Location: index.php");
exit;
?>

