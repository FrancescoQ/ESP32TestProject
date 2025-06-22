#include "SensorManager.h"
#include <Arduino.h>

void SensorManager::updateSensorData()
{
  unsigned long millisNow = millis();
  if (millisNow - lastMillis > 1000)
  { // Update every second
    lastMillis = millisNow;
    latestFakeData = random(0, 100); // Simulate sensor data
  }
}

int SensorManager::getLatestData() const
{
  return latestFakeData;
}
