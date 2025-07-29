<?php
// Run the Python fetcher script and capture output
$output = [];
$return_var = 0;

exec("python /home/CFE/Projects-6/CAN/test_functions/send_can.py 2>&1", $output, $return_var);

// If successful, redirect back to parameters.php
if ($return_var === 0) {
    header("Location: ../parameters.php?status=updated");
    exit;
} else {
    // If failed, display the captured output for debugging
    echo "Python script failed to run. Output:<br><pre>" . implode("\n", $output) . "</pre>";
}
?>

