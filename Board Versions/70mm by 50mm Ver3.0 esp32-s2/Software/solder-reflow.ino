#include "Ota.h"
#include "StatusLeds.h"
#include "Thermocouple.h"
#include "Voltage.h"
#include "wifi_credentials.h"
#include <Arduino.h>

// Using libraries
// https://github.com/ayushsharma82/ElegantOTA
// https://github.com/adafruit/Adafruit-MAX31855-library

// Update at http://192.168.1.163:81/update

// WIFI
// See wifi_credentials.h where the following should be defined:
// const char user_hostname[] = "";
// const char user_wifi_ssid[] = "";
// const char user_wifi_passsword[] = "";

// pin definitions
#define GATE_PIN 10
#define LED_RED_PIN 8
#define LED_GREEN_PIN 9
#define VIN_MEASURE_PIN 1
#define MAX31855_CS_PIN 38
#define MAX31855_SO_PIN 37
#define MAX31855_SCK_PIN 36

// Intervals
#define DEBUG_PRINTOUTS_EVERY_MS 1000

// members
Ota _ota(81, user_hostname);
Voltage _voltage(VIN_MEASURE_PIN, GATE_PIN);
unsigned long _last_debug_printout_timestamp = 0;
StatusLeds _status_leds(LED_RED_PIN, LED_GREEN_PIN);
Thermocouple _thermocouple(MAX31855_CS_PIN, MAX31855_SCK_PIN, MAX31855_SO_PIN);

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

void setup() {
  setupSerial();
  setupWifi();
  _status_leds.setup();
  _ota.setup();
  _voltage.setup();
  _thermocouple.setup();
}

void loop() {
  _ota.handle();
  _voltage.handle();
  _status_leds.handle();
  _thermocouple.handle();

  // Debug printouts for now.
  auto now = millis();
  if (now - _last_debug_printout_timestamp > DEBUG_PRINTOUTS_EVERY_MS) {
    _last_debug_printout_timestamp = now;

    Serial.print("Internal temperature = ");
    Serial.println(_thermocouple.getAmbientTemperature());

    double c = _thermocouple.getHeatbedTemperature();
    if (isnan(c)) {
      Serial.println("Something wrong with thermocouple!");
    } else {
      Serial.print("Heatbed temperature = ");
      Serial.println(c);
    }

    Serial.print("Vin = ");
    Serial.println(_voltage.getVinVoltage());
  }
}
