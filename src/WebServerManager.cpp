#include "WebServerManager.h"

WebServerManager::WebServerManager(WebServer &server, SensorManager &sensor)
    : server(server), sensor(sensor) {}

void WebServerManager::setup()
{
  // Example: Redirect root to your main page
  server.on("/", HTTP_GET, [this]()
            {
        server.sendHeader("Location", "/start/index.html", true);
        server.send(302, "text/plain", ""); });

  // Example: Dynamic sensor endpoint
  server.onNotFound([this]()
                    {
        String uri = server.uri();
        if (uri.startsWith("/sensor/")) {
            String idxStr = uri.substring(strlen("/sensor/"));
            int idx = idxStr.toInt();
            int value = sensor.getSensorValue(idx);
            if (value == -1) {
                server.send(404, "application/json", "{\"error\":\"Sensor not found\"}");
                return;
            }
            String json = "{\"value\":" + String(value) + "}";
            server.send(200, "application/json", json);
            return;
        }
        server.send(404, "text/plain", "Not found"); });

  // Serve static files
  server.serveStatic("/start", LittleFS, "/");

  // OTA setup
  ElegantOTA.begin(&server);
  ElegantOTA.setAuth("francesco", "pwd");

  // Start the server
  server.begin();
}

void WebServerManager::loop()
{
  ElegantOTA.loop();
}
