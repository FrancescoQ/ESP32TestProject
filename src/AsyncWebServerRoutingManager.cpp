#include <AsyncWebServerRoutingManager.h>
#include <Sensor.h>

AsyncWebServerRoutingManager::AsyncWebServerRoutingManager(AsyncWebServer &server, SensorManager &sensors)
    : server(server), sensors(sensors) {}

void AsyncWebServerRoutingManager::setup()
{
  // Enable CORS to be able to test easily.
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

  // For "/" redirect to our index.
  server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "");
    response->addHeader("Location", "/start/index.html", true);
    request->send(response);
  });

  // Memory consumption data.
  server.on("/heap", HTTP_GET, [this](AsyncWebServerRequest *request) {
    float freeHeapKB = ESP.getFreeHeap() / 1024.0; // Free memory in KB
    String json = "{\"value\":" + String(freeHeapKB) + "}";
    request->send(200, "application/json", json);
  });

  server.on("/setThreshold", HTTP_POST, [](AsyncWebServerRequest *request)
  {
    if (request->hasParam("value", true)) {
        int newVal = request->getParam("value", true)->value().toInt();
        //touchThreshold = newVal;
        AsyncWebServerResponse *response = request->beginResponse(302, "text/plain", "");
        response->addHeader("Location", "/start/index.html", true);
        request->send(response);
    }
  });

  server.on("/sensors", HTTP_GET, [this](AsyncWebServerRequest *request) {
    JsonDocument jsonDoc;
    String jsonString;

    jsonDoc["count"] = sensors.count();

    for (size_t i = 0; i < sensors.count(); i++)
    {
      String label = sensors.getSensorLabel(i);
      String type = sensors.getSensorType(i);
      Serial.println(type);

      jsonDoc["sensors"][i]["id"] = i;
      jsonDoc["sensors"][i]["label"] = label;
      jsonDoc["sensors"][i]["type"] = type;
      if (type == "touch") {
        //jsonDoc["sensors"][i]["threshold"] = touchThreshold;
      }
    }

    serializeJson(jsonDoc, jsonString);
    request->send(200, "application/json", jsonString);
  });

  // Serve all remaining static files, prepend them with /start to avoid
  // conflict with other routes not managed by the webserver
  // (like update of the OTA functionality)
  server.serveStatic("/start", LittleFS, "/").setDefaultFile("index.html");

  // On not found, try to dynamically recognize the requested path.
  server.onNotFound([this](AsyncWebServerRequest *request) {
    String uri = request->url();
    // Sensors data.
    if (uri.startsWith("/sensor/")) {
      JsonDocument jsonDoc;
      String jsonString;

      String idxStr = uri.substring(strlen("/sensor/"));
      int idx = idxStr.toInt();
      int value = sensors.getSensorValue(idx);
      if (value == -1) {
        request->send(404, "application/json", "{\"error\":\"Sensor not found\"}");
        return;
      }

      jsonDoc["value"] = String(value);
      if (sensors.getSensorType(idx) == "touch") {
        //jsonDoc["threshold"] = touchThreshold;
      }

      serializeJson(jsonDoc, jsonString);
      request->send(200, "application/json", jsonString);

      return;
    }
    request->send(404, "text/plain", "Not found");
  });

  // Start the server
  server.begin();
}

void AsyncWebServerRoutingManager::loop()
{
}
