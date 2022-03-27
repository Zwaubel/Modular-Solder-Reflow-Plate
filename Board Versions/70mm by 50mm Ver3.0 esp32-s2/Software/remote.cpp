#include "remote.h"

#define FORCE_PUBLISH_INTERVAL_MS 30000
#define PUBLISH_SETUP_EVERY_MS (60000 * 5) // 5 min

const char *HOME_ASSISTANT_INTERNAL_TEMPERATURE_CONFIG PROGMEM = "{ \
          \"unit_of_measurement\": \"°C\", \
          \"icon\": \"mdi:thermometer\", \
          \"name\": \"Solder reflow plate internal temperature\", \
          \"state_topic\": \"solder_reflow_plate/sensor/solder_reflow_plate_internal_temperature/state\", \
          \"availability_topic\": \"solder_reflow_plate/status\", \
          \"unique_id\": \"SolderReflowPlate_InternalTemperature\", \
          \"device\": { \
          \"identifiers\": \"58:cf:79:a4:ee:cc\", \
          \"name\": \"solder_reflow_plate\", \
          \"sw_version\": \"VSC, ElegantOTA 1.0\", \
          \"model\": \"esp32-s2\", \
          \"manufacturer\": \"espressif\" \
          } \
          }";

const char *HOME_ASSISTANT_BED_TEMPERATURE_CONFIG PROGMEM = "{ \
          \"unit_of_measurement\": \"°C\", \
          \"icon\": \"mdi:thermometer\", \
          \"name\": \"Solder reflow plate bed temperature\", \
          \"state_topic\": \"solder_reflow_plate/sensor/solder_reflow_plate_bed_temperature/state\", \
          \"availability_topic\": \"solder_reflow_plate/status\", \
          \"unique_id\": \"SolderReflowPlate_BedTemperature\", \
          \"device\": { \
          \"identifiers\": \"58:cf:79:a4:ee:cc\", \
          \"name\": \"solder_reflow_plate\", \
          \"sw_version\": \"VSC, ElegantOTA 1.0\", \
          \"model\": \"esp32-s2\", \
          \"manufacturer\": \"espressif\" \
          } \
          }";

const char *HOME_ASSISTANT_VOLTAGE_IN_CONFIG PROGMEM = "{ \
          \"unit_of_measurement\": \"V\", \
          \"icon\": \"mdi:power-plug\", \
          \"name\": \"Solder reflow plate voltage in\", \
          \"state_topic\": \"solder_reflow_plate/sensor/solder_reflow_plate_voltage_in/state\", \
          \"availability_topic\": \"solder_reflow_plate/status\", \
          \"unique_id\": \"SolderReflowPlate_VoltageIn\", \
          \"device\": { \
          \"identifiers\": \"58:cf:79:a4:ee:cc\", \
          \"name\": \"solder_reflow_plate\", \
          \"sw_version\": \"VSC, ElegantOTA 1.0\", \
          \"model\": \"esp32-s2\", \
          \"manufacturer\": \"espressif\" \
          } \
          }";

Remote::Remote(Thermocouple &thermocouple, Voltage &voltage, String host, String username, String password)
    : _thermocouple(thermocouple), _voltage(voltage), _mqtt(_wifi_client), _last_publish_ms(0), _host(host),
      _username(username), _password(password) {}

void Remote::setup() {
  _mqtt.setServer(_host.c_str(), 1883);
  _mqtt.setCallback([this](char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    Serial.println();
  });
}

void Remote::publishHASetup() {
  bool published_internal =
      _mqtt.publish_P("homeassistant/sensor/solder_reflow_plate/solder_reflow_plate_internal_temperature/config",
                      (const uint8_t *)HOME_ASSISTANT_INTERNAL_TEMPERATURE_CONFIG,
                      strlen(HOME_ASSISTANT_INTERNAL_TEMPERATURE_CONFIG), true);
  Serial.println("Published internal temperature HA: " + String(published_internal));

  bool published_bed = _mqtt.publish_P(
      "homeassistant/sensor/solder_reflow_plate/solder_reflow_plate_bed_temperature/config",
      (const uint8_t *)HOME_ASSISTANT_BED_TEMPERATURE_CONFIG, strlen(HOME_ASSISTANT_BED_TEMPERATURE_CONFIG), true);
  Serial.println("Published bed temperature HA: " + String(published_bed));

  bool published_voltage_in = _mqtt.publish_P(
      "homeassistant/sensor/solder_reflow_plate/solder_reflow_plate_voltage_in/config",
      (const uint8_t *)HOME_ASSISTANT_VOLTAGE_IN_CONFIG, strlen(HOME_ASSISTANT_VOLTAGE_IN_CONFIG), true);
  Serial.println("Published bed temperature HA: " + String(published_voltage_in));

  const char *online = "online";
  bool publish_status = _mqtt.publish("solder_reflow_plate/status", (const uint8_t *)online, strlen(online), true);
  Serial.println("Published status: " + String(publish_status));
}

bool Remote::mqttMaybeReconnect() {
  if (_mqtt.connected()) {
    return true; // EARLY RETURN
  }

  if (_retry_to_connect_at_ms == 0 || millis() > _retry_to_connect_at_ms) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (_mqtt.connect("SolderReflowPlate", _username.c_str(), _password.c_str(), "solder_reflow_plate/status", 0, true,
                      "offline")) {
      Serial.println("connected");

      publishHASetup();

    } else {
      Serial.print("failed, rc=");
      Serial.print(_mqtt.state());
      Serial.println(" try again in 5 seconds");
      _retry_to_connect_at_ms = millis() + 5000;
    }
  }

  return _mqtt.connected();
}

void Remote::handle() {
  if (!mqttMaybeReconnect()) {
    return; // EARLY RETURN
  }

  _mqtt.loop();

  unsigned long now = millis();
  bool force_publish = now - _last_publish_ms > FORCE_PUBLISH_INTERVAL_MS;

  if (force_publish) {
    String strval = String(_thermocouple.getAmbientTemperature());
    _mqtt.publish("solder_reflow_plate/sensor/solder_reflow_plate_internal_temperature/state", strval.c_str());

    auto bed_temperature = _thermocouple.getBedTemperature();
    if (!isnan(bed_temperature)) {
      strval = String(bed_temperature);
      _mqtt.publish("solder_reflow_plate/sensor/solder_reflow_plate_bed_temperature/state", strval.c_str());
    }

    strval = String(_voltage.getVinVoltage());
    _mqtt.publish("solder_reflow_plate/sensor/solder_reflow_plate_voltage_in/state", strval.c_str());

    _last_publish_ms = now;
  }

  if (now - _last_publish_ha_setup_ms > PUBLISH_SETUP_EVERY_MS) {
    publishHASetup();
    _last_publish_ha_setup_ms = now;
  }
}