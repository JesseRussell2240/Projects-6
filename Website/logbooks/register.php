<?php
require 'db.php';

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $username = trim($_POST['username']);
    $password = $_POST['password'];
    $confirm  = $_POST['confirm'];

    // Basic validation
    if (strlen($username) < 7 || strlen($password) < 5) {
        $error = "Username must be at least 3 characters and password at least 5 characters.";
    } elseif ($password !== $confirm) {
        $error = "Passwords do not match.";
    } else {
        // Check if username already exists
        $stmt = $pdo->prepare("SELECT id FROM users WHERE username = ?");
        $stmt->execute([$username]);
        if ($stmt->fetch()) {
            $error = "Username is already taken.";
        } else {
            // Hash and insert
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
    <form method="post">
        
        <label for="username">Username:</label>
        <input
            type="text"
            id="username"
            name="username"
            minlength="7"
            maxlength="180"
            required
        />
<p id="char-count">180 characters remaining</p>
        
        <input type="password" name="password" placeholder="Password" required><br>
        <input type="password" name="confirm" placeholder="Confirm Password" required><br>
        <button type="submit">Register</button>
    </form>
</section>
</body>
</html>

