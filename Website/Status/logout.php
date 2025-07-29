<?php
session_start();
session_destroy();
header("Location: weekly_reports.php");
exit;
?>

