#include "wifi_credentials.h"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ElegantOTA.h>

// Using libraries
// https://github.com/ayushsharma82/ElegantOTA

// Update at http://192.168.1.163:81/update

// WIFI
// See wifi_credentials.h where the following should be defined:
// const char user_hostname[] = "";
// const char user_wifi_ssid[] = "";
// const char user_wifi_passsword[] = "";

// PINS
#define LED1_PIN 8
#define LED2_PIN 9

WebServer _elegant_ota_server(81);
bool _last_led_value = false;
unsigned long _last_led_toggle_time = 0;

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

void setup() {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  setupSerial();
  setupWifi();
  setupOta();
}

void loop() {
  ArduinoOTA.handle();
  _elegant_ota_server.handleClient();

  if (millis() - _last_led_toggle_time > 500) {
    _last_led_toggle_time = millis();
    digitalWrite(LED1_PIN, _last_led_value);
    digitalWrite(LED2_PIN, !_last_led_value);
    _last_led_value = !_last_led_value;
  }
}
