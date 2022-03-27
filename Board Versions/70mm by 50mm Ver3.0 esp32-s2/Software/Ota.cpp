#include "Ota.h"
#include <ArduinoOTA.h>
#include <ElegantOTA.h>

Ota::Ota(const uint16_t http_port, const char *hostname) : _hostname(hostname), _elegant_ota_server(http_port) {}

void Ota::setup() {
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
  ElegantOTA.setID(_hostname);
  _elegant_ota_server.begin();
}

void Ota::handle() {
  ArduinoOTA.handle();
  _elegant_ota_server.handleClient();
}
