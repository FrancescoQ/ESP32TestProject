#include <SensorManager.h>

void SensorManager::addSensor(SensorType type, int pin, String label)
{
  sensors.emplace_back(type, pin, label);
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

String SensorManager::getSensorLabel(size_t idx) const
{
  if (idx < sensors.size())
    return sensors[idx].label;
  return "";
}

size_t SensorManager::count() const
{
  return sensors.size();
}
