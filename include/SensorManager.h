#pragma once

class SensorManager
{
public:
  void updateSensorData();
  int getLatestData() const;

private:
  int latestFakeData = 0;
  unsigned long lastMillis = 0;
};
