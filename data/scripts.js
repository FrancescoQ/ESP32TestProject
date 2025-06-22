async function updateSensorValue() {
  const response = await fetch("/sensor");
  let data = await response.json();
  show(data)
}

function show(data) {
  console.log(data);
  const sensorValueElement = document.querySelector(".sensor-1 .sensor-data-value");
  if (sensorValueElement) {
    sensorValueElement.textContent = data.value;
  } else {
    console.error("Element '.sensor-1 .sensor-data-value' not found.");
  }
}

timer = setInterval(function () {
  console.log("Updating sensor value...");
  updateSensorValue();
}, 2000);


document.querySelector(".sensor-1 .stop-monitoring").addEventListener("click", function() {
  console.log("Sensor value clicked, stopping updates.");
  clearInterval(timer);
});
