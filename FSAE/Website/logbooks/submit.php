<?php
require 'db.php';

if ($_SERVER["REQUEST_METHOD"] === "POST") {
    $name = $_POST['name'] ?? 'Anonymous';
    $message = $_POST['message'] ?? '';

    if (!empty($message)) {
        $stmt = $pdo->prepare("INSERT INTO entries (name, message) VALUES (?, ?)");
        $stmt->execute([$name, $message]);
    }
}

header("Location: index.php");
exit;
?>
