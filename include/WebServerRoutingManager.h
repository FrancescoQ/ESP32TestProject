#pragma once
#include <WebServer.h>
#include "SensorManager.h"
#include "LittleFS.h"
#include <ArduinoJson.h>

class WebServerRoutingManager
{
public:
  WebServerRoutingManager(WebServer &server, SensorManager &sensors);
  void setup();
  void loop();

private:
  WebServer &server;
  SensorManager &sensors;
};
