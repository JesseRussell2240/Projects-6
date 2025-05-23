<?php
session_start();
require 'db.php';
if (!isset($_SESSION['user_id'])) {
    header("Location: login.php");
    exit;
}

if ($_SERVER['REQUEST_METHOD'] === 'GET' && isset($_GET['id'])) {
    $stmt = $pdo->prepare("SELECT * FROM entries WHERE id = ? AND user_id = ?");
    $stmt->execute([$_GET['id'], $_SESSION['user_id']]);
    $entry = $stmt->fetch();
    if (!$entry) { header("Location: index.php"); exit; }
}
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $id = $_POST['id'];
    $title = $_POST['title'];
    $project_section = $_POST['project_section'];
    $message = $_POST['message'];
    $stmt = $pdo->prepare("UPDATE entries SET title=?, project_section=?, message=? WHERE id=? AND user_id=?");
    $stmt->execute([$title, $project_section, $message, $id, $_SESSION['user_id']]);
    header("Location: index.php");
    exit;
}
?>
<!DOCTYPE html>
<html>
<head>
    <title>Edit Entry</title>
    <link rel="stylesheet" href="format.css">
</head>
<body>
<div id="top-banner">
    <span style="font-size:1.5em; font-weight:bold;">Project Logbook</span>
    <nav style="margin-left:40px;">
        <a href="index.php">Home</a>
        <a href="logout.php">Logout</a>
    </nav>
</div>
<section class="subtab-section">
    <h2>Edit Entry</h2>
    <form method="post">
        <input type="hidden" name="id" value="<?= htmlspecialchars($entry['id']) ?>">
        <input type="text" name="title" value="<?= htmlspecialchars($entry['title']) ?>" required><br>
        <select name="project_section" required>
            <option value="">Select Section</option>
            <option value="Electrical" <?= $entry['project_section']=='Electrical'?'selected':'' ?>>Electrical</option>
            <option value="Mechanical" <?= $entry['project_section']=='Mechanical'?'selected':'' ?>>Mechanical</option>
            <option value="Software" <?= $entry['project_section']=='Software'?'selected':'' ?>>Software</option>
            <option value="General" <?= $entry['project_section']=='General'?'selected':'' ?>>General</option>
        </select><br>
        <textarea name="message" required><?= htmlspecialchars($entry['message']) ?></textarea><br>
        <button type="submit">Save Changes</button>
    </form>
</section>
</body>
</html>

