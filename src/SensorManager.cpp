#include <SensorManager.h>

void SensorManager::addSensor(SensorType type, int pin, String label)
{
  sensors.emplace_back(type, pin, label);
}

void SensorManager::updateAll()
{
  for (auto &s : sensors) {
    s.update();
  }
}

int SensorManager::getSensorValue(size_t idx) const
{
  if (idx < sensors.size()) {
    return sensors[idx].value;
  }
  return -1;
}

String SensorManager::getSensorLabel(size_t idx) const
{
  if (idx < sensors.size()) {
    return sensors[idx].label;
  }
  return "";
}

String sensorTypeToMachineName(SensorType type)
{
  switch (type) {
    case SensorType::Movement:
      return "movement";
    case SensorType::Light:
      return "light";
    case SensorType::Touch:
      return "touch";
    case SensorType::Fake:
      return "fake";
    default:
      return "";
  }
}

String SensorManager::getSensorType(size_t idx) const
{
  if (idx < sensors.size()) {
    return sensorTypeToMachineName(sensors[idx].type);
  }
  return "";
}

size_t SensorManager::count() const
{
  return sensors.size();
}
