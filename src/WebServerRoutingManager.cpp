#include <WebServerRoutingManager.h>

WebServerRoutingManager::WebServerRoutingManager(WebServer &server, SensorManager &sensors)
    : server(server), sensors(sensors) {}

void WebServerRoutingManager::setup()
{
  // Enable CORS to be able to test easily.
  server.enableCORS();

  // For "/" redirect to our index.
  server.on("/", HTTP_GET, [this]()
  {
    server.sendHeader("Location", "/start/index.html", true);
    server.send(302, "text/plain", "");
  });

  // Memory consumption data.
  server.on("/heap", HTTP_GET, [this]()
  {
    float freeHeapKB = ESP.getFreeHeap() / 1024.0; // Free memory in KB
    String json = "{\"value\":" + String(freeHeapKB) + "}";
    server.send(200, "application/json", json);
  });

  server.on("/sensors", HTTP_GET, [this]()
  {
    JsonDocument jsonDoc;
    String jsonString;

    jsonDoc["count"] = sensors.count();

    for (size_t i = 0; i < sensors.count(); i++)
    {
      String label = sensors.getSensorLabel(i);
      String type = sensors.getSensorType(i);
      jsonDoc["sensors"][i]["id"] = i;
      jsonDoc["sensors"][i]["label"] = label;
      jsonDoc["sensors"][i]["type"] = type;
    }

    serializeJson(jsonDoc, jsonString);
    server.send(200, "application/json", jsonString);
  });

  // Serve all remaining static files, prepend them with /start to avoid
  // conflict with other routes not managed by the webserver
  // (like update of the OTA functionality)
  server.serveStatic("/start", LittleFS, "/");

  // On not found, try to dynamically recognize the requested path.
  server.onNotFound([this]()
  {
    String uri = server.uri();
    // Sensors data.
    if (uri.startsWith("/sensor/")) {
        String idxStr = uri.substring(strlen("/sensor/"));
        int idx = idxStr.toInt();
        int value = sensors.getSensorValue(idx);
        if (value == -1) {
            server.send(404, "application/json", "{\"error\":\"Sensor not found\"}");
            return;
        }
        String json = "{\"value\":" + String(value) + "}";
        server.send(200, "application/json", json);
        return;
    }
    server.send(404, "text/plain", "Not found");
  });

  // Start the server
  server.begin();
}

void WebServerRoutingManager::loop()
{
}
