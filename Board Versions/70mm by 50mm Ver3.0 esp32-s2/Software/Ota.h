#include <ArduinoOTA.h>
#include <ElegantOTA.h>

#ifndef __OTA_H__
#define __OTA_H__

class Ota {
public:
  Ota(const uint16_t http_port, const char *hostname);

  void handle();
  void setup();

private:
  const char *_hostname;
  WebServer _elegant_ota_server;
};

#endif //__OTA_H__