<?php
$enable = isset($_POST['enable']) && $_POST['enable'] === 'true';
$flag = $enable ? 'enable' : 'disable';

$cmd = escapeshellcmd("python3 /home/CFE/Projects-6/CAN/test_functions/toggle_cyclic.py $flag");
$output = shell_exec($cmd);

echo json_encode(["status" => "ok", "output" => $output]);
?>
