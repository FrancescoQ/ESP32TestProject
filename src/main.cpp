#include <Arduino.h>
#include "FS.h"
#include "LittleFS.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h>
#include <ElegantOTA.h>

// @TODO:  explore the magic websocket world, es. https://randomnerdtutorials.com/esp32-websocket-server-arduino/

WebServer server(80);
int lastMillis = 0;
int latestFakeData = 0;

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

  // Endpoint to get the latest sensor data
  server.on("/sensor", HTTP_GET, []()
  {
    String json = "{\"value\":" + String(latestFakeData) + "}";
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
    Serial.print("Page not found: ");
    Serial.println(server.uri());
    server.send(404, "text/plain", "Not found");
  });

  // Start the server.
  server.begin();
  ElegantOTA.begin(&server);
  ElegantOTA.setAuth("francesco", "pwd");
}

void loop()
{
  server.handleClient();

  // Fake sensor reading data.
  int millisNow = millis();
  if (millisNow - lastMillis > 1000) // Update every 10 seconds
  {
    lastMillis = millisNow;
    latestFakeData = random(0, 100); // Simulate some sensor data
    Serial.println("Latest sensor data: " + String(latestFakeData));
  }

  // Handle OTA updates.
  ElegantOTA.loop();
}
