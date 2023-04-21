#ifndef __OTA_H__
#define __OTA_H__

#include <Arduino.h>
#include <WebServer.h>

/**
 * Take care of OTA releated stuff (Arduino OTA and HTTP OTA)
 */
class Ota {
public:
  /**
   * @brief Construct a new Ota instance.
   *
   * @param http_port port to use for HTTP OTA.
   * @param hostname hostname to report for OTA.
   */
  Ota(const uint16_t http_port, const char *hostname);

  /**
   * @brief Call from the parent setup() function.
   */
  void setup();
  /**
   * @brief Call from the parent handle() function.
   */
  void handle();

private:
  const char *_hostname;
  WebServer _elegant_ota_server;
};

#endif //__OTA_H__
