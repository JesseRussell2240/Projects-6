const dropdown = document.getElementById('can-id-filter');
const tbody = document.getElementById('can-table-body');
const waveformContainer = document.getElementById('waveform-container');

let lastData = [];
let accelerationChart, velocityChart;
const MAX_POINTS = 50;

// === Integration Tracking ===
let velocityEstimate = { x: 0, y: 0, z: 0 };
const SAMPLE_INTERVAL = 2.0; // seconds (matches 2000ms interval)

// ========== Utilities ==========
function hex(num, len = 2) {
  return '0x' + Number(num).toString(16).toUpperCase().padStart(len, '0');
}

function hexToRgb(hex) {
  hex = hex.replace(/^#/, '');
  const bigint = parseInt(hex, 16);
  return [(bigint >> 16) & 255, (bigint >> 8) & 255, bigint & 255].join(',');
}

// ========== Table ==========
function populateTable(data, selectedId) {
  tbody.innerHTML = '';
  data.forEach(msg => {
    const idHex = hex(msg.id, 3);
    if (selectedId !== 'all' && idHex !== selectedId) return;
    const row = `
      <tr>
        <td>${idHex}</td><td>${hex(msg.length)}</td>
        ${[...Array(8).keys()].map(i => `<td>${hex(msg[`byte${i}`])}</td>`).join('')}
        <td>${msg.count}</td><td>${msg.timestamp}</td>
      </tr>
    `;
    tbody.insertAdjacentHTML('beforeend', row);
  });
}

function updateDropdownOptions(data) {
  const uniqueIds = [...new Set(data.map(msg => hex(msg.id, 3)))];
  const existing = Array.from(dropdown.options).map(o => o.value);
  uniqueIds.forEach(id => {
    if (!existing.includes(id)) {
      const opt = new Option(id, id);
      dropdown.appendChild(opt);
    }
  });
}

// ========== Waveform Viewer ==========
function drawWaveformSet(data) {
  waveformContainer.innerHTML = '';
  data.forEach((msg, i) => {
    const bytes = Array.from({ length: 8 }, (_, j) => msg[`byte${j}`]);
    const bits = bytes.flatMap(b => [...Array(8)].map((_, k) => (b >> (7 - k)) & 1));
    const values = bits.map(b => (b === 1 ? 0 : 5));
    const canvas = document.createElement('canvas');
    canvas.width = 1000; canvas.height = 200;

    const container = document.createElement('div');
    container.style.marginBottom = '30px';
    container.innerHTML = `<p style="color:#ffd700;">CAN ID: ${hex(msg.id, 3)} | Timestamp: ${msg.timestamp}</p>`;
    container.appendChild(canvas);

    const overlay = document.createElement('div');
    overlay.style = "display:flex;justify-content:space-between;color:#ffd700;font-size:0.9rem;margin-top:4px;width:1000px";
    bytes.forEach(b => {
      const div = document.createElement('div');
      div.textContent = hex(b);
      div.style = "flex:1;text-align:center;";
      overlay.appendChild(div);
    });
    container.appendChild(overlay);
    waveformContainer.appendChild(container);

    new Chart(canvas.getContext('2d'), {
      type: 'line',
      data: {
        labels: bits.map((_, i) => `b${i}`),
        datasets: [{
          label: 'Bitstream',
          data: values,
          borderColor: '#ffd700',
          backgroundColor: 'rgba(255,215,0,0.2)',
          stepped: true,
          borderWidth: 2,
          pointRadius: 0
        }]
      },
      options: {
        responsive: false,
        scales: {
          y: {
            min: 0,
            max: 5,
            title: { display: true, text: 'Voltage (V)', color: '#ffd700' },
            ticks: { callback: v => `${v}V` }
          },
          x: { ticks: { color: '#ccc' } }
        },
        plugins: {
          legend: { labels: { color: '#ffd700' } }
        }
      }
    });
  });
}


// ========== Charts ==========
function createGraph(ctx, labelSet, colors, yMin = -100, yMax = 100) {
  return new Chart(ctx, {
    type: 'line',
    data: {
      labels: [],
      datasets: labelSet.map((d, i) => ({
        label: d,
        data: [],
        borderColor: colors[i],
        backgroundColor: `rgba(${hexToRgb(colors[i])}, 0.2)`,
        fill: true
      }))
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      animation: false,
      scales: {
        x: { title: { display: true, text: 'Time (s)' } },
        y: {
          min: yMin,
          max: yMax,
          title: { display: true, text: 'Value' }
        }
      },
      plugins: {
        legend: { labels: { color: '#333' } }
      }
    }
  });
}

function updateGraph(chart, data) {
  const now = new Date().toLocaleTimeString();
  chart.data.labels.push(now);
  chart.data.datasets[0].data.push(data.x);
  chart.data.datasets[1].data.push(data.y);
  chart.data.datasets[2].data.push(data.z);
  if (chart.data.labels.length > MAX_POINTS) {
    chart.data.labels.shift();
    chart.data.datasets.forEach(ds => ds.data.shift());
  }
  chart.update();
}

// ========== Fetch test_data.json ==========
async function fetchTestGraphData() {
  try {
    const response = await fetch('./9DOF.json');
    return await response.json();
  } catch (err) {
    console.error('[Test Data Fetch Error]', err);
    return null;
  }
}

// ========== Dropdown Filter ==========
dropdown.addEventListener('change', () => {
  const filtered = dropdown.value === 'all'
    ? lastData
    : lastData.filter(msg => hex(msg.id, 3) === dropdown.value);

  populateTable(filtered, dropdown.value);
  drawWaveformSet(filtered);
});

// ========== Main Interval ==========
setInterval(() => {
  // Live CAN table and waveform
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

  // Acceleration/velocity graphs from test_data.json
  fetchTestGraphData().then(test => {
    if (!test) return;

    const acc = test.acceleration || { x: 0, y: 0, z: 0 };

    // Estimate velocity from integrated acceleration
    velocityEstimate.x += acc.x * SAMPLE_INTERVAL;
    velocityEstimate.y += acc.y * SAMPLE_INTERVAL;
    velocityEstimate.z += acc.z * SAMPLE_INTERVAL;

    updateGraph(accelerationChart, acc);
    updateGraph(velocityChart, velocityEstimate);
  });
}, 2000);

// ========== On Load ==========
document.addEventListener('DOMContentLoaded', () => {
  const accCtx = document.getElementById('accelerationGraph')?.getContext('2d');
  const velCtx = document.getElementById('velocityGraph')?.getContext('2d');

  if (accCtx && velCtx) {
    // Updated scale ranges
    accelerationChart = createGraph(accCtx, ['X', 'Y', 'Z'], ['red', 'green', 'blue'], -20, 20);
    velocityChart     = createGraph(velCtx, ['X', 'Y', 'Z'], ['orange', 'purple', 'cyan'], -1000, 1000);
  }
});

// ========== Optional: Velocity Reset Button ==========
function resetVelocity() {
  velocityEstimate = { x: 0, y: 0, z: 0 };
}
