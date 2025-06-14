<?php
session_start();
require 'db.php';

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $username = trim($_POST['username']);
    $password = $_POST['password'];
    $confirm  = $_POST['confirm'];

    if (strlen($username) < 3 || strlen($password) < 5) {
        $error = "Username must be at least 3 characters and password at least 5 characters.";
    } elseif ($password !== $confirm) {
        $error = "Passwords do not match.";
    } else {
        $stmt = $pdo->prepare("SELECT id FROM users WHERE username = ?");
        $stmt->execute([$username]);
        if ($stmt->fetch()) {
            $error = "Username is already taken.";
        } else {
            $hash = password_hash($password, PASSWORD_DEFAULT);
            $stmt = $pdo->prepare("INSERT INTO users (username, password_hash) VALUES (?, ?)");
            $stmt->execute([$username, $hash]);
            $success = "User registered! <a href='login.php'>Login here</a>.";
        }
    }
}
?>
<!DOCTYPE html>
<html>
<head>
    <title>Register</title>
    <link rel="stylesheet" href="format.css">
</head>
<body>
<div id="top-banner">
    <span style="font-size:1.5em; font-weight:bold;">Project Logbook</span>
    <nav style="margin-left:40px;">
        <a href="index.php">Home</a>
        <a href="login.php">Login</a>
        <a href="register.php" class="active">Register</a>
    </nav>
</div>
<section class="subtab-section">
    <h2>Register</h2>
    <?php if (isset($error)) echo "<p style='color:red;'>$error</p>"; ?>
    <?php if (isset($success)) echo "<p style='color:green;'>$success</p>"; ?>
    <form method="post" class="register-form">
        <div class="form-row">
            <label for="username">Username</label>
            <input type="text" id="username" name="username" required>
        </div>
        <div class="form-row">
            <label for="password">Password</label>
            <input type="password" id="password" name="password" required>
        </div>
        <div class="form-row">
            <label for="confirm">Confirm</label>
            <input type="password" id="confirm" name="confirm" required>
        </div>
        <button type="submit">Register</button>
    </form>
</section>
</body>
</html>
