// Upload the filesystem files with the command
// pio run --target uploadfs
// this will upload the files found in "data" (by default in PlatformIO)
// to the LittleFS filesystem.
//
// NOTE: pio run --target buildfs to build the filesystem image with the
// data if we want to update it OTA.
// @TODO:  explore the magic websocket world, es. https://randomnerdtutorials.com/esp32-websocket-server-arduino/

#include <Arduino.h>
#include "LittleFS.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <ElegantOTA.h>
#include <SensorManager.h>
#include "WebServerRoutingManager.h"

#define UPDATE_FREQUENCY 1000

#define OTA_AUTH_NAME "francesco"
#define OTA_AUTH_PASSWORD "pwd"

SensorManager sensorManager;
WebServer server(80);
WebServerRoutingManager routingManager(server, sensorManager);

int lastMillis = 0;

void resetWiFi()
{
  WiFiManager wm;
  wm.resetSettings();
  ESP.restart();
}

void setup()
{
  Serial.begin(115200);

  // Initialize the LittleFS filesystem.
  LittleFS.begin();

  // Timeout of config portal
  WiFiManager wm;
  wm.setConfigPortalTimeout(180); // 3 minuti
  wm.setTitle("ESP32 WiFi Setup");

  // Try to connect to the last saved WiFi credentials or start the config portal.
  bool res = wm.autoConnect("MY_ESP32_AP");

  if (!res)
  {
    Serial.println("Connection failed. Starting config portal...");
    ESP.restart();
  }
  else
  {
    Serial.println("Successfully connected!");
  }

  // WiFi configs after connection.
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);

  // Setup the routing manager.
  routingManager.setup();

  // Start the OTA service.
  ElegantOTA.begin(&server);
  ElegantOTA.setAuth(OTA_AUTH_NAME, OTA_AUTH_PASSWORD);

  sensorManager.addSensor(SensorType::Movement, 12, "Some movement sensor");
  sensorManager.addSensor(SensorType::Light, 34, "My light sensor");
  sensorManager.addSensor(SensorType::Fake, 0, "The fake sensor");
  sensorManager.addSensor(SensorType::Light, 18, "My second light sensor");
}

void loop()
{
  server.handleClient();

  // Update sensor readings.
  int millisNow = millis();
  if (millisNow - lastMillis > 1000)
  {
    sensorManager.updateAll();
    lastMillis = millisNow;
  }

  // Handle OTA updates.
  ElegantOTA.loop();
}
