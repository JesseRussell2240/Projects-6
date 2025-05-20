<?php
$pdo = new PDO("mysql:host=localhost;dbname=logbooks", "siteuser", "sitepass123");
$pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
?>
