<?php
session_start();
require 'db.php';
?>
<!DOCTYPE html>
<html>
<head>
    <title>Weekly Status Reports</title>
    <link rel="stylesheet" href="format.css">
</head>
<body>
<div id="top-banner">
    <span style="font-size:1.5em; font-weight:bold;">Conestoga FSAE</span>
    <nav style="margin-left:40px;">
        <a href="../project.html">Home</a>
        <a href="weekly_reports.php" class="active">Status Reports</a>
        <?php if (isset($_SESSION['user_id'])): ?>
            <a href="logout.php">Logout</a>
        <?php else: ?>
            <a href="login.php">Login</a>
        <?php endif; ?>
    </nav>
</div>

<section class="subtab-section">
    <h2>Weekly Reports</h2>
    <table class="report-table">
        <tr>
            <th>Week</th>
            <th>Status</th>
            <th>Activities</th>
            <th>Project Changes</th>
            <th>Issues</th>
            <th>Goals</th>
            <th>Notes</th>
            <th>User</th>
            <th>Submitted</th>
        </tr>
        <?php
        $stmt = $pdo->query("SELECT wr.*, u.username FROM weekly_reports wr JOIN users u ON wr.user_id = u.id ORDER BY report_week DESC");
        while ($row = $stmt->fetch()):
        ?>
        <tr>
            <td><?= htmlspecialchars($row['report_week']) ?></td>
            <td><?= htmlspecialchars($row['status']) ?></td>
            <td><?= nl2br(htmlspecialchars($row['activities'])) ?></td>
            <td><?= nl2br(htmlspecialchars($row['project_changes'])) ?></td>
            <td><?= nl2br(htmlspecialchars($row['project_issues'])) ?></td>
            <td><?= nl2br(htmlspecialchars($row['goals_next_week'])) ?></td>
            <td><?= nl2br(htmlspecialchars($row['notes'])) ?></td>
            <td><?= htmlspecialchars($row['username']) ?></td>
            <td><?= $row['created_at'] ?></td>
        </tr>
        <?php endwhile; ?>
    </table>
</section>





<?php if (isset($_SESSION['user_id'])): ?>
    <section>
        <h3>Submit New Report</h3>
        <form method="post" action="submit_reports.php">
            <label>Week (YYYY-MM-DD): <input type="date" name="report_week" required></label><br>
            <label>Status:
                <select name="status" required>
                    <option value="">Select</option>
                    <option value="Good">Good</option>
                    <option value="OK">OK</option>
                    <option value="Bad">Bad</option>
                </select>
            </label><br>
            <textarea name="activities" placeholder="This week's activities"></textarea><br>
            <textarea name="project_changes" placeholder="Project changes"></textarea><br>
            <textarea name="project_issues" placeholder="Issues"></textarea><br>
            <textarea name="goals_next_week" placeholder="Goals for next week"></textarea><br>
            <textarea name="notes" placeholder="Other notes"></textarea><br>
            <button type="submit">Submit Report</button>
        </form>
    </section>
<?php endif; ?>

</body>
</html>
