<?php
require 'db.php';

$users = [
    ['Hayden', 'Hayden'],
    ['Zach', 'Zach'],
    ['Jesse', 'Jesse'],
    ['Josh', 'Josh']
];

foreach ($users as $u) {
    $username = $u[0];
    $password = password_hash($u[1], PASSWORD_DEFAULT);
    $stmt = $pdo->prepare("INSERT INTO users (username, password) VALUES (?, ?)");
    $stmt->execute([$username, $password]);
}

echo "Users added.\n";
?>
