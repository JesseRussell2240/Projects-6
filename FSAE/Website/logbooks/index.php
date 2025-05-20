<?php
session_start();
require 'db.php';

if (!isset($_SESSION['user_id'])) {
    header("Location: login.php");
    exit();
}

$stmt = $pdo->prepare("SELECT username FROM users WHERE id = ?");
$stmt->execute([$_SESSION['user_id']]);
$current_user = $stmt->fetch()['username'];

if ($_SERVER["REQUEST_METHOD"] === "POST" && isset($_POST['entry'])) {
    $stmt = $pdo->prepare("INSERT INTO entries (user_id, content) VALUES (?, ?)");
    $stmt->execute([$_SESSION['user_id'], $_POST['entry']]);
}

$stmt = $pdo->query("SELECT e.content, e.timestamp, u.username 
                     FROM entries e 
                     JOIN users u ON e.user_id = u.id 
                     ORDER BY e.timestamp DESC");
$entries = $stmt->fetchAll();
?>

<!DOCTYPE html>
<html>
<head><title>Logbooks</title></head>
<body>
    <h1>Welcome, <?= htmlspecialchars($current_user) ?></h1>
    <form method="POST">
        <textarea name="entry" rows="4" cols="50" required></textarea><br>
        <button type="submit">Add to <?= htmlspecialchars($current_user) ?>'s Logbook</button>
    </form>

    <h2>All Logbook Entries</h2>
    <?php foreach ($entries as $entry): ?>
        <p>
            <strong><?= htmlspecialchars($entry['username']) ?></strong>: 
            <?= nl2br(htmlspecialchars($entry['content'])) ?>
            <em>(<?= $entry['timestamp'] ?>)</em>
        </p>
    <?php endforeach; ?>

    <p><a href="logout.php">Logout</a></p>
</body>
</html>
