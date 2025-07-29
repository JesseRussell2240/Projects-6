<?php 
// Include file that fetches current parameter values from the database
// It populates the $params associative array used to fill in the default values in the UI
include 'parameters/fetch.php'; 
?>
<!DOCTYPE html>
<!--
  BAMOCAR CAN Parameter Control Dashboard
  Authors: Conestoga Electric FSAE Team
  Description: Interface for reading and writing motor controller CAN parameters
  Last Updated: 2025
-->
<html lang="en">
<head>
  <meta charset="UTF-8" />
  <title>BAMOCAR CAN Parameter Control</title>
  <!-- Link external resources -->
  <link rel="stylesheet" href="format.css" />
  <link rel="icon" href="./Media/Photos/Logo.png" type="image/png">
  <script defer src="script.js"></script>

  <!-- Inline style for layout and sidebar -->
  <style>
    #side-menu {
      position: fixed;
      top: 70px;
      left: 0;
      width: 160px;
      height: 100%;
      background-color: #111;
      color: #ffd700;
      padding: 140px 15px 15px 15px;
      box-shadow: 2px 0 6px rgba(0, 0, 0, 0.5);
    }
    #side-menu a {
      display: block;
      margin: 10px 0;
      color: #ffd700;
      text-decoration: none;
      font-weight: bold;
    }
    #side-menu a:hover {
      text-decoration: underline;
    }
    .tab-content {
      margin-left: 180px;
      padding: 20px 40px;
    }
    .subsystem-section {
      display: none;
    }
    .subsystem-section.active {
      display: block;
    }
    
    .overlay-notification {
      position: fixed;
      top: 400px; /* Adjust as needed to sit below your fixed top banner */
      left: 50%;
      transform: translateX(-50%);
      background-color: #cce5ff;
      color: #004085;
      padding: 12px 20px;
      border: 1px solid #b8daff;
      border-radius: 5px;
      z-index: 9999;
      box-shadow: 0 2px 8px rgba(0, 0, 0, 0.2);
    }
    
    .overlay-notification.success {
      background-color: #d4edda;
      color: #155724;
      border: 1px solid #c3e6cb;
    }

  </style>
</head>

<body>
  
  <!-- Notification for bamo CAN refresh -->
<?php if (isset($_GET['status']) && $_GET['status'] === 'updated'): ?>
  <div 
    class="overlay-notification">
    Parameter dictionary has been refreshed from the database.
  </div>
<?php endif; ?>

  <!-- Notification for DB refresh -->
<?php if (isset($_GET['status']) && $_GET['status'] === 'success'): ?>
  <div class="overlay-notification success">
    Parameters successfully written to the database.
  </div>
<?php endif; ?>




<header id="top-banner">
  <div class="banner-content">
    <img src="./Media/Photos/Logo.png" alt="Team Logo" id="logo">
    <h1>Conestoga Electric FSAE</h1>
  </div>
  <nav>
    <a href="index.html">Home</a>
    <a href="Live_Can.html">Live CAN Data</a>
    <a href="BMS_Data.html">BMS Data</a>
    <a href="parameters.php" class="active">Parameters</a>
    <a href="3D_View.html">3D View</a>
    <a href="live_data.html">Live Data</a>
    <a href="data_log.html">Data Log</a>
    <a href="media.html">Weekly Media</a>
    
  </nav>
</header>

<!-- Sidebar for subsystem navigation -->
<!-- This is a persistent sidebar used to switch between different parameter categories -->
<div id="side-menu">
  <a href="#" onclick="showSection('motor-control')">Motor Control</a>
  <a href="#" onclick="showSection('current-limits')">Current & Voltage</a>
  <a href="#" onclick="showSection('speed-config')">Speed & Acceleration</a>
  <a href="#" onclick="showSection('pid-gains')">PID Gains</a>
  <a href="#" onclick="showSection('drive-settings')">Drive Settings</a>
  <a href="#" onclick="showSection('eeprom')">EEPROM</a>
  
  <div style="margin-top: 20px;">
    <form method="POST" action="parameters/update_can.php">
      <button onclick="updateDictionary()" style="width: 100%; padding: 10px; background-color: #ffd700; color: #111; font-weight: bold; border: none; border-radius: 6px; cursor: pointer;">
        Update CAN from Database
      </button>
    </form>
  </div>
  

  
</div>

<div class="tab-content" id="parameters-container">

    <!-- MOTOR CONTROL SECTION -->
  <section class="subsystem-section active" id="motor-control">
    <h2>Motor Control</h2>

    <div class="control-group">
      <label>Motor Commands:</label>
      <button id="motorEnableToggle" onclick="startMotor()">Start Motor</button>
      <button onclick="stopMotor()">Stop Motor</button>
    </div>

    <form method="POST" action="parameters/post.php">
        <div class="control-group">
          <label for="speedSetpoint">Speed Setpoint (RPM):</label>
          <!-- Display current value (from DB) -->
          <input type="text" value="<?php echo $params['speedSetpoint'] ?? 'N/A'; ?>" class="readonly-field" readonly />
          <!-- Field to enter new value -->
          <input type="number" id="speedSetpoint" name="speedSetpoint" placeholder="Enter 0-10000 RPM" required />
          <button onclick="">Send</button>
      </div>
    </form>

    <form method="POST" action="parameters/post.php">
      <div class="control-group">
          <label for="torqueSetpoint">Torque Setpoint (%):</label>
          <!-- Display current value (from DB) -->
          <input type="text" value="<?php echo $params['torqueSetpoint'] ?? 'N/A'; ?>" class="readonly-field" readonly />
          <!-- Field to enter new value -->
          <input type="number" id="torqueSetpoint" name="torqueSetpoint" placeholder="Enter 0-100%" required />
          <button onclick="">Send</button>
      </div>
    </form>
  </section>


  <!-- CURRENT & VOLTAGE LIMITS -->
  <section class="subsystem-section" id="current-limits">
    
    <h2>Current and Voltage Limits</h2>
    
    <!-- IMAX -->
    <form method="POST" action="parameters/post.php">
      <div class="control-group">
        <label for="imax">Imax (A):</label>
        <!-- Display current value (from DB) -->
        <input type="text" value="<?php echo $params['imax'] ?? 'N/A'; ?>" class="readonly-field" readonly />
        <!-- Field to enter new value -->
        <input type="number" id="imax" name="imax" placeholder="Enter 0-500A" required />
        <button type="submit">Write</button>
      </div>
    </form>

    <!-- ICONT -->
    <form method="POST" action="parameters/post.php">
      <div class="control-group">
        <label for="icont">Icont (A):</label>
        <!-- Display current value (from DB) -->
        <input type="text" value="<?php echo $params['icont'] ?? 'N/A'; ?>" class="readonly-field" readonly />
        <!-- Field to enter new value -->
        <input type="number" id="icont" name="icont" placeholder="Enter 0-300A" required />
        
        <button type="submit">Write</button>
      </div>
    </form>

    
    <!-- UDCMIN -->
    <form method="POST" action="parameters/post.php">
      <div class="control-group">
        <label for="udcMin">UDC Min (V):</label>
        <!-- Display current value (from DB) -->
        <input type="text" value="<?php echo $params['udcMin'] ?? 'N/A'; ?>" class="readonly-field" readonly />
        <!-- Field to enter new value -->
        <input type="number" id="udcMin" name="udcMin" placeholder="Enter 50-500V" required />
        <button type="submit">Write</button>
      </div>
    </form>

    
    <!-- UDCMAX -->
    <form method="POST" action="parameters/post.php">
      <div class="control-group">
        <label for="udcMax">UDC Max (V):</label>
        <!-- Display current value (from DB) -->
        <input type="text" value="<?php echo $params['udcMax'] ?? 'N/A'; ?>" class="readonly-field" readonly />
        <!-- Field to enter new value -->
        <input type="number" id="udcMax" name="udcMax" placeholder="Enter 50-500V" required />
        <button type="submit">Write</button>
      </div>
    </form>

    <!-- TEMPMAX -->
    <form method="POST" action="parameters/post.php">
      <div class="control-group">
        <label for="tempMax">Max Temperature (C):</label>
        <!-- Display current value (from DB) -->
        <input type="text" value="<?php echo $params['tempMax'] ?? 'N/A'; ?>" class="readonly-field" readonly />
        <!-- Field to enter new value -->
        <input type="number" id="tempMax" name="tempMax" placeholder="Enter 0-15000C" required />
        <button type="submit">Write</button>
      </div>
    </form>
  
  </section>
  <!-- SPEED & ACCELERATION CONFIG -->
  
  
  <section class="subsystem-section" id="speed-config">
    <h2>Speed and Acceleration</h2>
    
    <!-- MAXRPM% -->
    <form method="POST" action="parameters/post.php">
      <div class="control-group">
        <label for="n100">N-100% (Max RPM):</label>
        <!-- Display current value (from DB) -->
        <input type="text" value="<?php echo $params['n100'] ?? 'N/A'; ?>" class="readonly-field" readonly />
        <!-- Field to enter new value -->
        <input type="number" id="n100" name="n100" placeholder="Enter 0-10000 max RPM reference" required />
        <button type="submit">Write</button>
      </div>
    </form>

    <!-- MAXSPEED% -->
    <form method="POST" action="parameters/post.php">
        <div class="control-group">
        <label for="nLimPos">Speed Limit + (%):</label>
        <!-- Display current value (from DB) -->
        <input type="text" value="<?php echo $params['nLimPos'] ?? 'N/A'; ?>" class="readonly-field" readonly />
        <!-- Field to enter new value -->
        <input type="number" id="nLimPos" name="nLimPos" placeholder="Enter 0-100% Limit" required />
        <button type="submit">Write</button>
      </div>
    </form>

    
    <!-- KACC -->
    <form method="POST" action="parameters/post.php">
      <div class="control-group">
        <label for="kacc">Kacc (Accel Ramp ms):</label>
        <!-- Display current value (from DB) -->
        <input type="text" value="<?php echo $params['kacc'] ?? 'N/A'; ?>" class="readonly-field" readonly />
        <!-- Field to enter new value -->
        <input type="number" id="kacc" name="kacc" placeholder="Enter 0-10000 (ms)" required />
        <button type="submit">Write</button>
      </div>
    </form>
    
    <!-- Kdec -->
    <form method="POST" action="parameters/post.php">
      <div class="control-group">
        <label for="kdec">Kdec (Decel Ramp ms):</label>
        <!-- Display current value (from DB) -->
        <input type="text" value="<?php echo $params['kdec'] ?? 'N/A'; ?>" class="readonly-field" readonly />
        <!-- Field to enter new value -->
        <input type="number" id="kdec" name="kdec" placeholder="Enter 0-10000 (ms)" required />
        <button onclick="">Write</button>
      </div>
    </form>
  </section>

    <!-- PID GAINS CONFIG -->
  <section class="subsystem-section" id="pid-gains">
    <h2>PID Control Gains</h2>
    
    <!-- Kp -->
    <form method="POST" action="parameters/post.php">
      <div class="control-group">
        <label for="kp">P Gain:</label>
        <!-- Display current value (from DB) -->
        <input type="text" value="<?php echo $params['kp'] ?? 'N/A'; ?>" class="readonly-field" readonly />
        <!-- Field to enter new value -->
        <input type="number" id="kp" name="kp" placeholder="Enter 0-1000" required />
        <button type="submit">Write</button>
      </div>
    </form>

    
    <!-- Ki -->
    <form method="POST" action="parameters/post.php">
      <div class="control-group">
        <label for="ki">I Gain:</label>
        <!-- Display current value (from DB) -->
        <input type="text" value="<?php echo $params['ki'] ?? 'N/A'; ?>" class="readonly-field" readonly />
        <!-- Field to enter new value -->
        <input type="number" id="ki" name="ki" placeholder="Enter 0-1000" required />
        <button type="submit">Write</button>
      </div>
    </form>

    
    <!-- Kd -->
    <form method="POST" action="parameters/post.php">
      <div class="control-group">
        <label for="kd">D Gain:</label>
         <!-- Display current value (from DB) -->
        <input type="text" value="<?php echo $params['kd'] ?? 'N/A'; ?>" class="readonly-field" readonly />
        <!-- Field to enter new value -->
        <input type="number" id="kd" name="kd" placeholder="Enter 0-1000" required />
        <button type="submit">Write</button>
      </div>
    </form>

    
    
  </section>

  <!-- DRIVE SETTINGS -->
  <section class="subsystem-section" id="drive-settings">
    <h2>Drive Settings</h2>
    
    <!-- CAN baudrate Selection -->
    <form method="POST" action="parameters/post.php">
      <div class="control-group">
        <label for="canBaud">CAN Baudrate (0x73):</label>
        <select id="canBaud" onchange=""> <!-- ADD FUNCTION CALL HERE -->
          <option value="0x4014">250 kbps</option>
          <option value="0x4019">500 kbps (default)</option>
          <option value="0x4002">1 Mbps</option>
        </select>
        <button onclick="">Write</button> <!-- ADD FUNCTION CALL HERE -->
      </div>
    <form method="POST" action="parameters/post.php">
    
    <!-- CAN Node ID -->
    <form method="POST" action="parameters/post.php">
      <div class="control-group">
        <label for="nodeId">CAN Node ID:</label>
         <!-- Display current value (from DB) -->
        <input type="text" value="<?php echo $params['nodeId'] ?? 'N/A'; ?>" class="readonly-field" readonly />
        <!-- Field to enter new value -->
        <input type="number" id="nodeId" name="nodeId" placeholder="Enter new value" required />
        <button type="submit">Write</button>
      </div>
    </form>
  </section>

  <!-- EEPROM ACTIONS -->
  <section class="subsystem-section" id="eeprom">
    <h2>EEPROM Management</h2>
    <div class="control-group">
      <button onclick="">Save Parameters to EEPROM</button> <!-- ADD FUNCTION CALL HERE -->
    </div>
    <div class="control-group">
      <button onclick="">Load Parameters from EEPROM</button> <!-- ADD FUNCTION CALL HERE -->
    </div>
  </section>
</div>


<script>
  
  function showSection(id) {
    document.querySelectorAll('.subsystem-section').forEach(section => {
      section.classList.remove('active');
    });
    document.getElementById(id).classList.add('active');
  }
  
  async function startMotor() {
    const res = await fetch('http://192.168.1.221:5000/motor/start', { method: 'POST' });

    const data = await res.json();
    alert(data.status || data.error);
  }

  async function stopMotor() {
    const res = await fetch('http://192.168.1.221:5000/motor/stop', { method: 'POST' });

    const data = await res.json();
    alert(data.status || data.error);
  }
  
  
    //Parameters updated success banner
    window.addEventListener('DOMContentLoaded', () => {
    const urlParams = new URLSearchParams(window.location.search);
    
    // Show error popup if out-of-range
    if (urlParams.has('error')) {
      const param = urlParams.get('error');
      alert("Error: Value for '" + param + "' is not within acceptable range.");
    }
  });
  
</script>
</body>
</html>
