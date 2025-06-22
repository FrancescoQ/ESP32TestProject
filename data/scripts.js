/**
 * Update free memory field.
 */
async function getFreeHeap() {
  const response = await fetch("/heap");
  let data = await response.json();
  const heapElement = document.querySelector(".heap");
  if (heapElement) {
    heapElement.textContent = data.value;
  } else {
    console.error("Heap element not found.");
  }
}

/**
 * Update sensor value field.
 */
async function updateMovementSensorValue() {
  const response = await fetch("/sensor/0");
  let data = await response.json();
  const sensorValueElement = document.querySelector(".sensor-1 .sensor-data-value");
  show(sensorValueElement, data);
}

async function updateLightSensorValue() {
  const response = await fetch("/sensor/1");
  let data = await response.json();
  const sensorValueElement = document.querySelector(".sensor-2 .sensor-data-value");
  show(sensorValueElement, data);
}

async function updateFakeSensorValue() {
  const response = await fetch("/sensor/2");
  let data = await response.json();
  const sensorValueElement = document.querySelector(".sensor-3 .sensor-data-value");
  show(sensorValueElement, data);
}

function show(element, data) {
  if (element) {
    element.textContent = data.value;
  } else {
    console.error("Sensor element not found.");
  }
}

// Initial call to update sensor value
updateMovementSensorValue();
updateLightSensorValue();
updateFakeSensorValue();

// Set an interval to update the sensor value every 2 seconds
dataTimer = setInterval(function () {
  updateMovementSensorValue();
  updateLightSensorValue();
  updateFakeSensorValue();
}, 2000);

// Update free heap memory every 3 seconds.
heapTimer = setInterval(function () {
  getFreeHeap();
}, 5000);
