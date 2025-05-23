<?php
session_start();
require 'db.php';

if (!isset($_SESSION['user_id'])) {
    die("Unauthorized access.");
}

$stmt = $pdo->prepare("INSERT INTO weekly_reports (user_id, report_week, status, activities, project_changes, project_issues, goals_next_week, notes)
VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
$stmt->execute([
    $_SESSION['user_id'],
    $_POST['report_week'],
    $_POST['status'],
    $_POST['activities'],
    $_POST['project_changes'],
    $_POST['project_issues'],
    $_POST['goals_next_week'],
    $_POST['notes']
]);

header("Location: weekly_reports.php");
exit;
?>
