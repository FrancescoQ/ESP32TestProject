/**
 * @file SensorManager.h
 * @brief The SensorManager utility class.
 *
 * This class will handle the sensors, it allows to add them,
 * update and get sensor values.
 */

#pragma once
#include <Sensor.h>
#include <vector>

class SensorManager
{
public:
  /**
   * @brief Add a sensor to the sensors vector of the sensor manager.
   * @param SensorType type
   * @param int pin
   * @param String label
   */
  void addSensor(SensorType type, int pin, String label);

  /**
   * @brief Update all the sensors.
   * Call the update method of each sensor: each sensor type will hande it in
   * its own way.
   */
  void updateAll();


  /**
   * @brief Returns the value of the given sensor.
   * @param idx the index in the vector of sensors.
   * @return the stored value.
   */
  int getSensorValue(size_t idx) const;

  /**
   * @brief Returns the label of the given sensor.
   * @param idx the index in the vector of sensors.
   * @return the label passed when the sensor was created.
   */
  String getSensorLabel(size_t idx) const;

  /**
   * @brief Returns the number of added sensors.
   * @return the number of available sensors.
   */
  size_t count() const;

private:
  /**
   * @brief The vector that store the sensors.
   */
  std::vector<Sensor> sensors;
};
