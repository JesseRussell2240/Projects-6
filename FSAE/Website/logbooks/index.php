<?php
require 'db.php';
$stmt = $pdo->query("SELECT * FROM entries ORDER BY timestamp DESC");
$entries = $stmt->fetchAll();
?>

<!DOCTYPE html>
<html>
<head>
    <title>Public Logbook</title>
    <style>
        body { font-family: Arial; padding: 20px; background: #f9f9f9; }
        table { width: 100%; border-collapse: collapse; margin-bottom: 30px; }
        th, td { border: 1px solid #ccc; padding: 10px; }
        form { margin-top: 30px; }
    </style>
</head>
<body>

<h2>Public Logbook</h2>

<table>
    <tr><th>ID</th><th>Name</th><th>Message</th><th>Timestamp</th></tr>
    <?php foreach ($entries as $entry): ?>
    <tr>
        <td><?= htmlspecialchars($entry['id']) ?></td>
        <td><?= htmlspecialchars($entry['name']) ?></td>
        <td><?= nl2br(htmlspecialchars($entry['message'])) ?></td>
        <td><?= $entry['timestamp'] ?></td>
    </tr>
    <?php endforeach; ?>
</table>

<h3>Submit a Message</h3>
<form action="submit.php" method="post">
    <label>Name: <input type="text" name="name" /></label><br><br>
    <label>Message:<br><textarea name="message" rows="5" cols="50" required></textarea></label><br><br>
    <button type="submit">Submit</button>
</form>

</body>
</html>
