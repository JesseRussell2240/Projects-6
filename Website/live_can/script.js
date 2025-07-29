// ======== GLOBAL VARIABLES =========
const dropdown = document.getElementById('can-id-filter');
const tbody = document.getElementById('can-table-body');
const waveformContainer = document.getElementById('waveform-container');

let lastData = [];              // Cache for latest data
window.chartInstances = [];    // Track active Chart.js instances

// ======== HEX FORMATTER =========
function hex(num, len = 2) {
  return '0x' + Number(num).toString(16).toUpperCase().padStart(len, '0');
}

// ======== TABLE POPULATION =========
function populateTable(data, selectedId) {
  tbody.innerHTML = '';

  data.forEach(msg => {
    const idHex = hex(msg.id, 3);
    if (selectedId !== 'all' && idHex !== selectedId) return;

    const row = document.createElement('tr');
    row.innerHTML = `
      <td>${idHex}</td>
      <td>${hex(msg.length)}</td>
      <td>${hex(msg.byte0)}</td>
      <td>${hex(msg.byte1)}</td>
      <td>${hex(msg.byte2)}</td>
      <td>${hex(msg.byte3)}</td>
      <td>${hex(msg.byte4)}</td>
      <td>${hex(msg.byte5)}</td>
      <td>${hex(msg.byte6)}</td>
      <td>${hex(msg.byte7)}</td>
      <td>${msg.count}</td>
      <td>${msg.timestamp}</td>
    `;
    tbody.appendChild(row);
  });
}

// ======== DROPDOWN UPDATE =========
function updateDropdownOptions(data) {
  const ids = [...new Set(data.map(msg => hex(msg.id, 3)))];
  const existingOptions = Array.from(dropdown.options).map(o => o.value);

  ids.forEach(id => {
    if (!existingOptions.includes(id)) {
      const option = document.createElement('option');
      option.value = id;
      option.textContent = id;
      dropdown.appendChild(option);
    }
  });
}

// ======== MULTI-CHART WAVEFORM DRAWER =========
function drawWaveformSet(data) {
  waveformContainer.innerHTML = '';
  window.chartInstances.forEach(chart => chart.destroy());
  window.chartInstances = [];

  data.forEach((msg, index) => {
    const bytes = [
      msg.byte0, msg.byte1, msg.byte2, msg.byte3,
      msg.byte4, msg.byte5, msg.byte6, msg.byte7
    ];

    // Convert bytes to bitstream
    let bitstream = [];
    let hexLabels = [];

    bytes.forEach(byte => {
      for (let i = 7; i >= 0; i--) {
        bitstream.push((byte >> i) & 1);
      }
      hexLabels.push(hex(byte)); // For visual overlay
    });

    // Build bitstream waveform
    const labels = [];
    const values = [];
    for (let i = 0; i < bitstream.length; i++) {
      labels.push(`b${i}`);
      values.push(bitstream[i] === 1 ? 0 : 5);  // Dominant bits = 0V
    }

    // Chart container
    const section = document.createElement('div');
    section.style.marginBottom = '40px';

    const label = document.createElement('p');
    label.style.color = '#ffd700';
    label.textContent = `CAN ID: ${hex(msg.id, 3)} | Timestamp: ${msg.timestamp}`;
    section.appendChild(label);

    const canvas = document.createElement('canvas');
    canvas.id = `waveformChart${index}`;
    canvas.width = 1000;
    canvas.height = 200;
    section.appendChild(canvas);

    // Add hex overlay labels
    const hexOverlay = document.createElement('div');
    hexOverlay.style.display = 'flex';
    hexOverlay.style.justifyContent = 'space-between';
    hexOverlay.style.color = '#ffd700';
    hexOverlay.style.fontSize = '0.9rem';
    hexOverlay.style.marginTop = '4px';
    hexOverlay.style.width = canvas.width + 'px';

    hexLabels.forEach(hexVal => {
      const hexEl = document.createElement('div');
      hexEl.textContent = hexVal;
      hexEl.style.flex = '1';
      hexEl.style.textAlign = 'center';
      hexOverlay.appendChild(hexEl);
    });

    section.appendChild(hexOverlay);
    waveformContainer.appendChild(section);

    // Create Chart
    const ctx = canvas.getContext('2d');
    const chart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: labels,
        datasets: [{
          label: 'Bitstream',
          data: values,
          borderColor: '#ffd700',
          backgroundColor: 'rgba(255, 215, 0, 0.2)',
          borderWidth: 2,
          stepped: true,
          pointRadius: 0
        }]
      },
      options: {
        responsive: false,
        //animation: false,
        scales: {
          y: {
            beginAtZero: true,
            suggestedMax: 5,
            ticks: {
              callback: (val) => `${val}V`
            },
            title: {
              display: true,
              text: 'Voltage (V)',
              color: '#ffd700'
            }
          },
          x: {
            ticks: { color: '#ccc' }
          }
        },
        plugins: {
          legend: {
            labels: { color: '#ffd700' }
          }
        }
      }
    });

    window.chartInstances.push(chart);
  });
}


// ======== EVENT BINDINGS =========
dropdown.addEventListener('change', () => {
  const filtered = dropdown.value === 'all'
    ? lastData
    : lastData.filter(msg => hex(msg.id, 3) === dropdown.value);

  populateTable(filtered, dropdown.value);
  drawWaveformSet(filtered);
});

// ======== DATA FETCH INTERVAL =========
setInterval(() => {
  fetch('live_can/live_can_table.php')
    .then(res => res.json())
    .then(data => {
      lastData = data;
      updateDropdownOptions(data);

      const filtered = dropdown.value === 'all'
        ? data
        : data.filter(msg => hex(msg.id, 3) === dropdown.value);

      populateTable(filtered, dropdown.value);
      drawWaveformSet(filtered);
    })
    .catch(err => console.error("[CAN Fetch Error]", err));
}, 2000);
