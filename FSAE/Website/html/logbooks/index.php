<?php session_start(); require 'db.php'; ?>

<!DOCTYPE html>
<html>
<head>
    <title>Logbook</title>
</head>
<body>
<h2>Logbook Entries</h2>

<?php
$stmt = $pdo->query("SELECT entries.*, users.username FROM entries LEFT JOIN users ON entries.user_id = users.id ORDER BY timestamp DESC");
while ($row = $stmt->fetch()):
?>
    <div style="border:1px solid #ccc; margin:10px; padding:10px;">
        <strong><?= htmlspecialchars($row['name']) ?></strong> 
        (<?= htmlspecialchars($row['username']) ?>)<br>
        <?= nl2br(htmlspecialchars($row['message'])) ?><br>
        <small><?= $row['timestamp'] ?></small>
        <?php if (isset($_SESSION['user_id']) && $_SESSION['user_id'] == $row['user_id']): ?>
            <form method="post" action="delete.php" style="display:inline;">
                <input type="hidden" name="entry_id" value="<?= $row['id'] ?>">
                <button type="submit">Delete</button>
            </form>
        <?php endif; ?>
    </div>
<?php endwhile; ?>

<?php if (isset($_SESSION['user_id'])): ?>
    <h3>Add Entry</h3>
    <form method="post" action="submit.php">
        <input type="text" name="name" placeholder="Your name" required><br>
        <textarea name="message" placeholder="Your message" required></textarea><br>
        <button type="submit">Submit</button>
    </form>
<?php else: ?>
    <p><a href="login.php">Log in to add entries</a></p>
<?php endif; ?>

<?php if (isset($_SESSION['user_id'])): ?>
    <p><a href="logout.php">Logout</a></p>
<?php endif; ?>

</body>
</html>
