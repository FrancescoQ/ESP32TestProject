#pragma once
#include <Arduino.h>
#include <String.h>

// Define some sensor types used in the project.
enum class SensorType
{
  Movement,
  Light,
  Fake
};

// Sensor class to handle different types of sensors.
struct Sensor
{
  SensorType type;
  int pin;
  int value;
  String label;

  Sensor(SensorType t, int p, const String &l) : type(t), pin(p), label(l), value(0) {}

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
