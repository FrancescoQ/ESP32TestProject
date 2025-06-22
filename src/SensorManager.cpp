#include "SensorManager.h"

void SensorManager::addSensor(SensorType type, int pin)
{
  sensors.emplace_back(type, pin);
}

void SensorManager::updateAll()
{
  for (auto &s : sensors)
  {
    s.update();
  }
}

int SensorManager::getSensorValue(size_t idx) const
{
  if (idx < sensors.size())
    return sensors[idx].value;
  return -1;
}

size_t SensorManager::count() const
{
  return sensors.size();
}
