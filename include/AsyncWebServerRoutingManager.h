#pragma once
#include <ESPAsyncWebServer.h>
#include <SensorManager.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

class AsyncWebServerRoutingManager
{
public:
  AsyncWebServerRoutingManager(AsyncWebServer &server, SensorManager &sensors);
  void setup();
  void loop();

private:
  AsyncWebServer &server;
  SensorManager &sensors;
};
