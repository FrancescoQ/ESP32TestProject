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
async function updateSensorValue() {
  const response = await fetch("/sensor");
  let data = await response.json();
  show(data);
}

function show(data) {
  const sensorValueElement = document.querySelector(".sensor-1 .sensor-data-value");
  if (sensorValueElement) {
    sensorValueElement.textContent = data.value;
  } else {
    console.error("Element '.sensor-1 .sensor-data-value' not found.");
  }
}

// Initial call to update sensor value
updateSensorValue();

// Set an interval to update the sensor value every 2 seconds
dataTimer = setInterval(function () {
  updateSensorValue();
}, 2000);

// Update free heap memory every 3 seconds.
heapTimer = setInterval(function () {
  getFreeHeap();
}, 5000);
