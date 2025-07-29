setInterval(() => {
  fetch('live_data/get_live_data.php')
    .then(res => res.json())
    .then(data => {
      document.getElementById('motor-rpm').textContent = data.rpm + ' rpm';
      document.getElementById('motor-voltage').textContent = data.voltage + ' V';
      document.getElementById('motor-current').textContent = data.current + ' A';
      document.getElementById('battery-soc').textContent = data.soc + ' %';
      //document.getElementById('cell-temp').textContent = data.cell_temp + ' °C';
      //document.getElementById('motor-temp').textContent = data.inverter_temp + ' °C';
    })
    .catch(err => {
      console.error('Failed to fetch live data:', err);
    });
}, 2000); // every 2 seconds


//Polling the DB to update data from the live CAN.

let cyclicEnabled = false;

const toggleBtn = document.getElementById("toggleCyclicBtn");

toggleBtn.addEventListener("click", () => {
  cyclicEnabled = !cyclicEnabled;
  toggleBtn.textContent = cyclicEnabled ? "Disable Cyclic Updates" : "Enable Cyclic Updates";

  fetch("live_data/toggle_cyclic.php", {
    method: "POST",
    headers: { "Content-Type": "application/x-www-form-urlencoded" },
    body: `enable=${cyclicEnabled}`
  })
    .then(res => res.json())
    .then(res => {
      console.log(res.output || "Toggled cyclic updates.");
    })
    .catch(err => {
      console.error("Toggle failed:", err);
    });
});
