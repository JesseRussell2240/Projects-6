window.addEventListener("DOMContentLoaded", () => {
  const sessionSelect = document.getElementById("logSessionSelect");
  const exportBtn = document.getElementById("exportCsvBtn");

  fetch("data_log/list_logs.php")
    .then((res) => res.json())
    .then((logs) => {
      sessionSelect.innerHTML = "";
      if (!logs.length) {
        const opt = document.createElement("option");
        opt.textContent = "-- No logs found --";
        opt.disabled = true;
        opt.selected = true;
        sessionSelect.appendChild(opt);
        exportBtn.style.pointerEvents = "none";
        exportBtn.style.opacity = "0.6";
      } else {
        logs.forEach((filename) => {
          const opt = document.createElement("option");
          opt.value = filename;
          opt.textContent = filename;
          sessionSelect.appendChild(opt);
        });
        exportBtn.style.pointerEvents = "auto";
        exportBtn.style.opacity = "1";
      }
    });

  exportBtn.addEventListener("click", () => {
    const selected = sessionSelect.value;
    if (!selected || selected.startsWith("--")) return;
    exportBtn.href = `data_log/export_csv.php?file=${encodeURIComponent(selected)}`;
  });
});
