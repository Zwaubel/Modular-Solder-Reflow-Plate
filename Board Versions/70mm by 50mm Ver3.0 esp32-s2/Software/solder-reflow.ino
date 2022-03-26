#include "MAX31855.h"
#include "wifi_credentials.h"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ElegantOTA.h>

// Using libraries
// https://github.com/ayushsharma82/ElegantOTA
// https://github.com/Zanduino/MAX31855

// Update at http://192.168.1.163:81/update

// WIFI
// See wifi_credentials.h where the following should be defined:
// const char user_hostname[] = "";
// const char user_wifi_ssid[] = "";
// const char user_wifi_passsword[] = "";

// PINS
#define LED1_PIN 8
#define LED2_PIN 9
#define MAX31855_CS_PIN 38
#define MAX31855_SCK_PIN 36
#define MAX31855_SO_PIN 37
#define GATE_PIN 10
#define VIN_MEASURE_PIN 1

// INTERVALS
#define BLINK_LED_EVERY_MS 500
#define READ_THERMOCOUPLE_EVERY_MS 1000

WebServer _elegant_ota_server(81);
bool _last_led_value = false;
unsigned long _last_led_toggle_timestamp = 0;
unsigned long _last_thermocouple_timestamp = 0;
MAX31855_Class _thermocouple; // TODO(johboh): Move to own class

void setupSerial() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Booting...");
}

void setupWifi() {
  Serial.println("Setting up Wifi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(user_wifi_ssid, user_wifi_passsword);
  WiFi.setHostname(user_hostname);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("have wifi");
  Serial.print("IP number: ");
  Serial.println(WiFi.localIP());
}

void setupOta() {
  ArduinoOTA.onStart([]() { Serial.println("OTA: Start"); });
  ArduinoOTA.onEnd([]() { Serial.println("\nOTA: End"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("OTA: Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("OTA: Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
  });
  ArduinoOTA.begin();

  ElegantOTA.begin(&_elegant_ota_server);
  ElegantOTA.setID(user_hostname);
  _elegant_ota_server.begin();
}

void setupThermocouple() {
  auto result = _thermocouple.begin(MAX31855_CS_PIN, MAX31855_SO_PIN, MAX31855_SCK_PIN);
  if (!result) {
    Serial.println("Failed to setup max31855");
  }
}

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  setupSerial();
  setupWifi();
  setupOta();
  setupThermocouple();
}

void loop() {
  ArduinoOTA.handle();
  _elegant_ota_server.handleClient();

  auto now = millis();
  if (now - _last_led_toggle_timestamp > BLINK_LED_EVERY_MS) {
    _last_led_toggle_timestamp = now;
    digitalWrite(LED1_PIN, _last_led_value);
    digitalWrite(LED2_PIN, !_last_led_value);
    _last_led_value = !_last_led_value;
  }

  if (now - _last_thermocouple_timestamp > READ_THERMOCOUPLE_EVERY_MS) {
    _last_thermocouple_timestamp = now;

    int32_t ambient_temperature = _thermocouple.readAmbient(); // retrieve MAX31855 die ambient temperature
    int32_t probe_temperature = _thermocouple.readProbe();     // retrieve thermocouple probe temp
    uint8_t fault_code = _thermocouple.fault();                // retrieve any error codes
    if (fault_code) {                                          // Display error code if present
      if (fault_code & B001) {
        Serial.println(F("Fault: Wire not connected"));
      }
      if (fault_code & B010) {
        Serial.println(F("Fault: Short-circuited to Ground (negative)"));
      }
      if (fault_code & B100) {
        Serial.println(F("Fault: Short-circuited to VCC (positive)"));
      }
    } else {
      // clang-format off
      Serial.print("Ambient Temperature is ");
      Serial.print((float)ambient_temperature / 1000, 3);
      Serial.println("\xC2\xB0""C");
      Serial.print("Probe Temperature is   ");
      Serial.print((float)probe_temperature / 1000, 3);
      Serial.println("\xC2\xB0""C\n");
      // clang-format on
    }
  }
}
