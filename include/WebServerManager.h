#pragma once
#include <WebServer.h>
#include <ElegantOTA.h>
#include "SensorManager.h"
#include "LittleFS.h"

class WebServerManager
{
public:
  WebServerManager(WebServer &server, SensorManager &sensor);
  void setup();
  void loop();

private:
  WebServer &server;
  SensorManager &sensor;
};
