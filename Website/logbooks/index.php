<?php
session_start();
require 'db.php';
?>
<!DOCTYPE html>
<html>
<head>
    <title>Project Logbook</title>
    <link rel="stylesheet" href="format.css">
</head>
<body>
<div id="top-banner">
    <span style="font-size:1.5em; font-weight:bold;">Home</span>
    <nav style="margin-left:40px;">
        <a href="../project.html">Home</a>
	<a href="index.php" class="active">Weekly Logbooks
        <?php if (isset($_SESSION['user_id'])): ?>
            <a href="logout.php">Logout</a>
        <?php else: ?>
            <a href="login.php">Login</a>
        <?php endif; ?>
    </nav>
</div>

<section class="subtab-section">
    <h2>Logbook Entries</h2>
    <table id="schedule">
        <tr>
            <th>Section</th>
            <th>Title</th>
            <th>Message</th>
            <th>User</th>
            <th>Timestamp</th>
            <?php if (isset($_SESSION['user_id'])) echo '<th>Actions</th>'; ?>
        </tr>
        <?php
        $stmt = $pdo->query("SELECT entries.*, users.username FROM entries LEFT JOIN users ON entries.user_id = users.id ORDER BY timestamp DESC");
        while ($row = $stmt->fetch()):
        ?>
        <tr>
            <td><?= htmlspecialchars($row['project_section']) ?></td>
            <td><?= htmlspecialchars($row['title']) ?></td>
            <td><?= nl2br(htmlspecialchars($row['message'])) ?></td>
            <td><?= htmlspecialchars($row['username']) ?></td>
            <td><?= $row['timestamp'] ?></td>
            <?php if (isset($_SESSION['user_id']) && $_SESSION['user_id'] == $row['user_id']): ?>
            <td>
                <a href="edit.php?id=<?= $row['id'] ?>">Edit</a>
                <form method="post" action="delete.php" style="display:inline;">
                    <input type="hidden" name="entry_id" value="<?= $row['id'] ?>">
                    <button type="submit" onclick="return confirm('Delete this entry?');">Delete</button>
                </form>
            </td>
            <?php elseif (isset($_SESSION['user_id'])): ?>
            <td></td>
            <?php endif; ?>
        </tr>
        <?php endwhile; ?>
    </table>

    <?php if (isset($_SESSION['user_id'])): ?>
        <h3>Add New Entry</h3>
        <form method="post" action="submit.php">
            <input type="text" name="title" placeholder="Title" required><br>
            <select name="project_section" required>
                <option value="">Select Section</option>
                <option value="Electrical">Electrical</option>
                <option value="Mechanical">Mechanical</option>
                <option value="Software">Software</option>
                <option value="General">General</option>
            </select><br>
            <textarea name="message" placeholder="Your message" required></textarea><br>
            <button type="submit">Submit</button>
        </form>
    <?php else: ?>
        <p><a href="login.php">Log in to add entries</a></p>
    <?php endif; ?>
</section>
</body>
</html>

