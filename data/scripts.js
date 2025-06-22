const baseUrl = "";

initSensors();


async function initSensors() {
  const response = await fetch(baseUrl + "/sensors");
  let data = await response.json();
  const sensorDataSection = document.querySelector("section.sensor-data");

  if (data.sensors.length > 0) {
    data.sensors.forEach(sensor => {
      const sensorID = sensor.id;
      const sensorLabel = sensor.label;

      const sensorElementWrapper = document.createElement("div");
      sensorElementWrapper.classList.add("sensor-" + sensorID);

      const sensorElementLabel = document.createElement("h3");
      sensorElementLabel.textContent = sensorLabel;

      sensorElementWrapper.append(sensorElementLabel);

      const sensorElementValue = document.createElement("p");
      sensorElementValue.textContent = "Data: ";
      const sensorElementValuePlaceholder = document.createElement("span");
      sensorElementValuePlaceholder.classList.add("sensor-data-value");
      sensorElementValuePlaceholder.dataset.sensorId = sensorID;
      sensorElementValuePlaceholder.innerHTML = "<span class='lds-dual-ring'></span>";
      sensorElementValue.append(sensorElementValuePlaceholder);
      sensorElementWrapper.append(sensorElementValue);

      sensorDataSection.append(sensorElementWrapper);

      setInterval(function () {
        updateSensor(sensorID);
      }, 1000);
    });
  }
}

/**
 * Update sensor value field.
 */
async function updateSensor(id) {
  const response = await fetch(baseUrl + "/sensor/" + id);
  let data = await response.json();
  const sensorValueElement = document.querySelector(".sensor-" + id + " .sensor-data-value");
  show(sensorValueElement, data);
}


function show(element, data) {
  if (element) {
    element.textContent = data.value;
  } else {
    console.error("Sensor element not found.");
  }
}


/**
 * Update free memory field.
 */
async function getFreeHeap() {
  const response = await fetch(baseUrl + "/heap");
  let data = await response.json();
  const heapElement = document.querySelector(".heap");
  if (heapElement) {
    heapElement.textContent = data.value;
  } else {
    console.error("Heap element not found.");
  }
}

// Update free heap memory.
heapTimer = setInterval(function () {
  getFreeHeap();
}, 5000);
