let scene, camera, renderer, model, pivot, controls;
let cumulativeRotation = { x: 0, y: 0, z: 0 };
const sensitivity = 1;

function init() {
  const container = document.getElementById('viewer-container');

  scene = new THREE.Scene();
  camera = new THREE.PerspectiveCamera(75, container.offsetWidth / container.offsetHeight, 0.1, 10000);
  renderer = new THREE.WebGLRenderer();
  renderer.setSize(container.offsetWidth, container.offsetHeight);
  container.appendChild(renderer.domElement);
  scene.background = new THREE.Color(0xffffff);

  // Lighting
  scene.add(new THREE.AmbientLight(0x404040, 1));
  const dirLight = new THREE.DirectionalLight(0xadad85, 1.5);
  dirLight.position.set(5, 5, 5);
  scene.add(dirLight);



  // Load GLB
  const loader = new THREE.GLTFLoader();
  loader.load('./3D_View/fsaechassis2-v1.glb', (gltf) => {
    console.log('GLB loaded!', gltf);

    model = gltf.scene;
    model.scale.set(1, 1, 1); // Keep at 1; auto-zoom handles camera now

    // Get center and size
    const box = new THREE.Box3().setFromObject(model);
    const center = new THREE.Vector3();
    const size = new THREE.Vector3();
    box.getCenter(center);
    box.getSize(size);

    console.log("Model center:", center);
    console.log("Model size:", size);

    // Recenter model to origin
    model.position.sub(center);

    // Use debug material to confirm visibility
    model.traverse((child) => {
      if (child.isMesh) {
        child.material = new THREE.MeshNormalMaterial();
      }
    });

    // Use pivot for gyro rotation
    pivot = new THREE.Object3D();
    pivot.add(model);
    scene.add(pivot);

    // Auto-set camera based on model size
    const maxDim = Math.max(size.x, size.y, size.z);
    const fov = camera.fov * (Math.PI / 180);
    let cameraZ = maxDim / (2 * Math.tan(fov / 2));
    cameraZ *= 2; // Zoom out a bit
    camera.position.set(0, 0, cameraZ);
    camera.lookAt(0, 0, 0);

    // Optional: OrbitControls
    controls = new THREE.OrbitControls(camera, renderer.domElement);
    controls.update();
  });

  animate();
}

async function fetchGyroData() {
  try {
    const response = await fetch('./9DOF.json');
    if (!response.ok) throw new Error("Network response not ok");
    const text = await response.text();
    return JSON.parse(text);
  } catch (error) {
    console.error('Error fetching gyroscope data:', error);
    return null;
  }
}

function updateModelOrientation(gyroData, deltaTime) {
  if (pivot && gyroData?.gyro) {
    cumulativeRotation.x += gyroData.gyro.x * deltaTime * sensitivity;
    cumulativeRotation.y += gyroData.gyro.y * deltaTime * sensitivity;
    cumulativeRotation.z += gyroData.gyro.z * deltaTime * sensitivity;
    pivot.rotation.set(cumulativeRotation.x, cumulativeRotation.y, cumulativeRotation.z);
  }
}

let lastUpdateTime = Date.now();
async function animate() {
  requestAnimationFrame(animate);
  const currentTime = Date.now();
  const deltaTime = (currentTime - lastUpdateTime) / 1000;
  lastUpdateTime = currentTime;

  const gyroData = await fetchGyroData();
  if (gyroData) {
    updateModelOrientation(gyroData, deltaTime);
  }

  if (controls) controls.update();
  renderer.render(scene, camera);
}

document.addEventListener('DOMContentLoaded', init);
