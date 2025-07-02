// Upload the filesystem files with the command
// pio run --target uploadfs
// this will upload the files found in "data" (by default in PlatformIO)
// to the LittleFS filesystem.
//
// NOTE: pio run --target buildfs to build the filesystem image with the
// data if we want to update it OTA.
//
// @TODO:  explore the magic websocket world, es. https://randomnerdtutorials.com/esp32-websocket-server-arduino/

#include <Arduino.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <SensorManager.h>
#include <AsyncWebServerRoutingManager.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define UPDATE_FREQUENCY 1000

#define OTA_AUTH_NAME "francesco"
#define OTA_AUTH_PASSWORD "pwd"

// WebServer server(80);
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
SensorManager sensorManager;
AsyncWebServerRoutingManager routingManager(server, sensorManager);

/*
 * note:If lcd1602 uses PCF8574T, IIC's address is 0x27,
 *      or lcd1602 uses PCF8574AT, IIC's address is 0x3F.
 */
LiquidCrystal_I2C lcd(0x27, 16, 2);

int lastSensorUpdate = 0;
int lcdMillisOn = 0;

void resetWiFi()
{
  WiFiManager wm;
  wm.resetSettings();
  ESP.restart();
}

void setup()
{
  Serial.begin(115200);

  // LCD display init
  Wire.begin();
  lcd.init();                 // LCD driver initialization
  lcd.backlight();            // Open the backlight
  lcd.setCursor(0, 0);        // Move the cursor to row 0, column 0
  lcd.print("My ESP32!"); // The print content is displayed on the LCD

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

  // Add sensors.
  sensorManager.addSensor(SensorType::Movement, 12, "A faje movement sensor");
  sensorManager.addSensor(SensorType::Light, 34, "My fake light sensor");
  sensorManager.addSensor(SensorType::Fake, 0, "The generic fake sensor");
  sensorManager.addSensor(SensorType::Light, 18, "My second (fakek) light sensor");
  sensorManager.addSensor(SensorType::Touch, 4, "A Touch pin, REAL!");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IP Address:");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
}

void loop()
{
  // Update sensor readings.
  int millisNow = millis();
  if (millisNow - lastSensorUpdate > 1000)
  {
    sensorManager.updateAll();
    lastSensorUpdate = millisNow;
  }

  if (touchRead(4) < 60) {
    lcd.backlight();
    lcdMillisOn = millisNow;
  }

  if (millisNow - lcdMillisOn > 10000)
  {
    lcd.noBacklight();
    lcdMillisOn = millisNow;
  }

  // Handle OTA updates.
  ElegantOTA.loop();
}
