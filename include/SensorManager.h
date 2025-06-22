#pragma once
#include "Sensor.h"
#include <vector>

class SensorManager
{
public:
  void addSensor(SensorType type, int pin, String label);
  void updateAll();
  int getSensorValue(size_t idx) const;
  String getSensorLabel(size_t idx) const;
  size_t count() const;

private:
  std::vector<Sensor> sensors;
};
