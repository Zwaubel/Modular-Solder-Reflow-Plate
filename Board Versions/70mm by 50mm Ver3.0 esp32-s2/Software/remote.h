#ifndef __REMOTE_H__
#define __REMOTE_H__

#include "Thermocouple.h"
#include "Voltage.h"
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

/**
 * Handles all remote/external communication.
 * In this case, responsible for communicating with MQTT server for
 * interating into Home Assistant (https://www.home-assistant.io/)
 */
class Remote {
public:
  /**
   * @param host MQTT hostname.
   * @param usenrame MQTT username.
   * @param password MQTT password.
   */
  Remote(Thermocouple &thermocouple, Voltage &voltage, String host, String username, String password);
  void setup();
  void handle();

private:
  void publishHASetup();
  bool mqttMaybeReconnect();

private:
  Voltage &_voltage;
  Thermocouple &_thermocouple;
  unsigned long _last_publish_ms = 0;
  unsigned long _last_publish_ha_setup_ms = 0;

  String _host;
  String _username;
  String _password;

  WiFiClient _wifi_client;
  PubSubClient _mqtt;
  unsigned long _retry_to_connect_at_ms = 0;
};

#endif //__REMOTE_H__