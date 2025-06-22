#include <Arduino.h>
#include "LittleFS.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <ElegantOTA.h>
#include <SensorManager.h>

// @TODO:  explore the magic websocket world, es. https://randomnerdtutorials.com/esp32-websocket-server-arduino/

#define UPDATE_FREQUENCY 1000

SensorManager sensorManager;
WebServer server(80);
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
  LittleFS.begin();

  WiFiManager wm;

  // Timeout of config portal
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
    // Connesso!
    Serial.println("Connection successful!");
  }

  // Configurazione post-connessione WiFi
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);

  // Upload the file with the command
  // pio run --target uploadfs
  // this will upload the files found in "data" (by default in PlatformIO)
  // to the LittleFS filesystem.
  //
  // NOTE: pio run --target buildfs to build the filesystem image with the
  // data if we want to update it OTA.

  // Handle "/".
  server.on("/", HTTP_GET, []()
  {
    server.sendHeader("Location", "/start/index.html", true);
    server.send(302, "text/plain", "");
  });

  // Endpoint to get the latest sensor data, MOVED IN THE NOTFOUND HANDLER.
  // server.on("/sensor/movement", HTTP_GET, []()
  // {
  //   int value = sensorManager.getSensorValue(0);
  //   String json = "{\"value\":" + String(value) + "}";
  //   server.send(200, "application/json", json);
  // });

  // server.on("/sensor/light", HTTP_GET, []()
  // {
  //   int value = sensorManager.getSensorValue(1);
  //   String json = "{\"value\":" + String(value) + "}";
  //   server.send(200, "application/json", json);
  // });

  // server.on("/sensor/fake", HTTP_GET, []()
  // {
  //   int value = sensorManager.getSensorValue(2);
  //     String json = "{\"value\":" + String(value) + "}";
  //     server.send(200, "application/json", json);
  // });

  server.on("/heap", HTTP_GET, []()
  {
    float freeHeapKB = ESP.getFreeHeap() / 1024.0; // Free memory in KB
    String json = "{\"value\":" + String(freeHeapKB) + "}";
    server.send(200, "application/json", json);
  });

  // Endpoint to reset WiFi settings.
  // @todo how to handle this in the frontend?
  server.on("/resetwifi", HTTP_GET, []()
  {
    // resetWiFi();
  });

  // Handle all other static files.
  server.serveStatic("/start", LittleFS, "/");

  // Handle the not found case.
  server.onNotFound([]()
  {
    String uri = server.uri(); // e.g., "/sensor/1"
    if (uri.startsWith("/sensor/")) {
      String idxStr = uri.substring(strlen("/sensor/")); // get the part after "/sensor/"
      int idx = idxStr.toInt();
      int value = sensorManager.getSensorValue(idx);
      if (value == -1)
      {
        server.send(404, "application/json", "{\"error\":\"Sensor not found\"}");
        return;
      }
      String json = "{\"value\":" + String(value) + "}";
      server.send(200, "application/json", json);
      return;
    }

    Serial.print("Page not found: ");
    Serial.println(uri);
    server.send(404, "text/plain", "Not found"); });

  // Start the server.
  server.begin();

  // Start the OTA service.
  ElegantOTA.begin(&server);
  ElegantOTA.setAuth("francesco", "pwd");

  sensorManager.addSensor(SensorType::Movement, 12);
  sensorManager.addSensor(SensorType::Light, 34);
  sensorManager.addSensor(SensorType::Fake, 0);
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
