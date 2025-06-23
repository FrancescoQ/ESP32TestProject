/**
 * @file Sensor.h
 * @brief This represent a single sensor instance handled by the
 * SensorManager class.
 */
#pragma once
#include <Arduino.h>
#include <String.h>

/**
 * @brief Definition of the sensor types available to be added to the project.
 */
enum class SensorType
{
  Movement,
  Light,
  Fake
};

/**
 * @brief The sensor instance.
 */
struct Sensor
{
  /**
   * The sensor type
   */
  SensorType type;
  /**
   * @brief The pin where the sensor is connected to the board.
   */
  int pin;

  /**
   * @brief The current value.
   * The value can be updated by the update() method below, this is the
   * property with the stored value.
   */
  int value;

  /**
   * @brief The sensor label as passed when the sensor is added.
   */
  String label;

  /**
   * @brief Constructor to build the sensor.
   * The value is always initialized to 0
   * @param SensorType t
   * @param int p
   * @param String l
   */
  Sensor(SensorType t, int p, const String &l) : type(t), pin(p), label(l), value(0) {}

  /**
   * @brief The update method to get the new value to be stored.
   */
  void update()
  {
    switch (type)
    {
    case SensorType::Movement:
      // value = digitalRead(pin);
      value = random(0, 2); // Fake movement
      break;
    case SensorType::Light:
      // value = analogRead(pin);
      value = random(0, 1024); // Fake light
      break;
    case SensorType::Fake:
      value = random(0, 100);
      break;
    }
  }
};
