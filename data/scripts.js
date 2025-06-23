(() => {
  const baseUrl = "";

  /**
   * Init only after loaded.
   */
  document.addEventListener("DOMContentLoaded", () => {
    init();
  });

  /**
   * Init functions.
   */
  function init() {
    initSensors();
    setInterval(updateFreeHeap, 5000);
  }

  /**
   * Initialize the sensors.
   */
  async function initSensors() {
    try {
      // Fetch the data of all available sensors.
      const response = await fetch(`${baseUrl}/sensors`);
      const data = await response.json();

      if (!data.sensors?.length) {
        return
      };

      const sensorDataSection = document.querySelector("section.sensor-data");
      const template = document.getElementById("sensor-template");

      // Build all the sensor elements.
      data.sensors.forEach(sensor => {
        const element = createSensorElement(sensor, template);
        sensorDataSection.append(element);

        // Update the sensor value regularly.
        setInterval(() => updateSensor(sensor.id), 1000);
      });

    } catch (error) {
      console.error("Failed to initialize sensors:", error);
    }
  }

  /**
   * Create a single sensor element.
   */
  function createSensorElement(sensor, template) {
    const clone = template.content.cloneNode(true);
    const wrapper = clone.querySelector("div");
    wrapper.classList.add(`sensor-${sensor.id}`);

    wrapper.querySelector("h3").textContent = sensor.label;

    const valueEl = wrapper.querySelector(".sensor-data-value");
    valueEl.dataset.sensorId = sensor.id;

    return wrapper;
  }

  /**
   * Update a sensor value.
   */
  async function updateSensor(id) {
    try {
      const response = await fetch(`${baseUrl}/sensor/${id}`);
      const data = await response.json();

      const sensorValueEl = document.querySelector(`.sensor-${id} .sensor-data-value`);
      if (sensorValueEl) {
        sensorValueEl.textContent = data.value;
      } else {
        console.warn(`Sensor element for ID ${id} not found`);
      }
    } catch (error) {
      console.error(`Failed to update sensor ${id}:`, error);
    }
  }

  /**
   * Update Free memory element.
   */
  async function updateFreeHeap() {
    try {
      const response = await fetch(`${baseUrl}/heap`);
      const data = await response.json();

      const heapEl = document.querySelector(".heap");
      if (heapEl) {
        heapEl.textContent = data.value;
      } else {
        console.warn("Heap element not found");
      }
    } catch (error) {
      console.error("Failed to update heap value:", error);
    }
  }
})();
