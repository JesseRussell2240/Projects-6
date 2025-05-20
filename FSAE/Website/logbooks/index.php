<?php
session_start();
require 'db.php';

$stmt = $pdo->query("SELECT entries.*, users.username FROM entries LEFT JOIN users ON entries.user_id = users.id ORDER BY timestamp DESC");
$entries = $stmt->fetchAll();
?>

<!DOCTYPE html>
<html>
<head>
    <title>Logbook</title>
</head>
<body>
    <h2>Message Logbook</h2>

    <?php if (isset($_SESSION['username'])): ?>
        <p>Welcome, <?= htmlspecialchars($_SESSION['username']) ?> |
            <a href="logout.php">Logout</a></p>
    <?php else: ?>
        <p><a href="login.php">Login</a> or <a href="register.php">Register</a></p>
    <?php endif; ?>

    <form action="submit.php" method="post">
        Name: <input name="name"><br>
        Message:<br>
        <textarea name="message" rows="4" cols="40"></textarea><br>
        <button type="submit">Submit</button>
    </form>

    <h3>Entries</h3>
    <table border="1" cellpadding="5">
        <tr>
            <th>ID</th><th>Name</th><th>Message</th><th>Timestamp</th><th>By</th><th>Action</th>
        </tr>
        <?php foreach ($entries as $entry): ?>
        <tr>
            <td><?= $entry['id'] ?></td>
            <td><?= htmlspecialchars($entry['name']) ?></td>
            <td><?= nl2br(htmlspecialchars($entry['message'])) ?></td>
            <td><?= $entry['timestamp'] ?></td>
            <td><?= htmlspecialchars($entry['username'] ?? 'Guest') ?></td>
            <td>
                <?php if (isset($_SESSION['user_id']) && $_SESSION['user_id'] == $entry['user_id']): ?>
                    <form action="delete.php" method="post" style="display:inline;">
                        <input type="hidden" name="id" value="<?= $entry['id'] ?>">
                        <button type="submit">Delete</button>
                    </form>
                <?php endif; ?>
            </td>
        </tr>
        <?php endforeach; ?>
    </table>
</body>
</html>
