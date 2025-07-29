let isLogging = localStorage.getItem("isLogging") === "true";
let logData = JSON.parse(localStorage.getItem("logData") || "[]");
let previousSnapshot = JSON.parse(localStorage.getItem("previousSnapshot") || "{}");

const startBtn = document.getElementById("startLoggingBtn");
const stopBtn = document.getElementById("stopLoggingBtn");

function updateButtonStates() {
  startBtn.style.pointerEvents = isLogging ? "none" : "auto";
  startBtn.style.opacity = isLogging ? "0.6" : "1";
  stopBtn.style.pointerEvents = isLogging ? "auto" : "none";
  stopBtn.style.opacity = isLogging ? "1" : "0.6";
}

updateButtonStates();

startBtn.addEventListener("click", () => {
  fetch('data_log/start_logging.php')
    .then(() => {
      isLogging = true;
      logData = [];
      previousSnapshot = {};
      localStorage.setItem("isLogging", "true");
      localStorage.setItem("logData", JSON.stringify(logData));
      localStorage.setItem("previousSnapshot", JSON.stringify(previousSnapshot));
      updateButtonStates();
    });
});

stopBtn.addEventListener("click", () => {
  isLogging = false;
  fetch('data_log/stop_logging.php', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(logData)
  })
    .then(res => res.json())
    .then(res => {
      alert(`Log saved as ${res.filename}`);
    })
    .catch(err => {
      console.error('Failed to save log:', err);
      alert("Failed to save log file.");
    });

  localStorage.setItem("isLogging", "false");
  localStorage.removeItem("logData");
  localStorage.removeItem("previousSnapshot");
  updateButtonStates();
});

setInterval(() => {
  if (!isLogging) return;

  fetch('live_data/get_live_data.php')
    .then(res => res.json())
    .then(data => {
      const now = new Date().toISOString();
      Object.keys(data).forEach(key => {
        if (data[key] !== previousSnapshot[key]) {
          logData.push({ parameter: key, value: data[key], timestamp: now });
          previousSnapshot[key] = data[key];
        }
      });
      localStorage.setItem("logData", JSON.stringify(logData));
      localStorage.setItem("previousSnapshot", JSON.stringify(previousSnapshot));
    });
}, 2000);
