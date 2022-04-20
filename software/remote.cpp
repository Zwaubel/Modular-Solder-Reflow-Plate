#include "remote.h"

#define FORCE_PUBLISH_INTERVAL_MS 5000
#define FORCE_PUBLISH_INTERVAL_WHEN_RUNNING_MS 750
#define PUBLISH_SETUP_EVERY_MS (60000 * 30) // 30 min

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

const char *HOME_ASSISTANT_TARGET_TEMPERATURE_CONFIG PROGMEM = "{ \
          \"unit_of_measurement\": \"°C\", \
          \"icon\": \"mdi:thermometer\", \
          \"name\": \"Solder reflow plate target temperature\", \
          \"state_topic\": \"solder_reflow_plate/sensor/solder_reflow_plate_target_temperature/state\", \
          \"availability_topic\": \"solder_reflow_plate/status\", \
          \"unique_id\": \"SolderReflowPlate_TargetTemperature\", \
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

const char *HOME_ASSISTANT_DUTY_CYCLE_CONFIG PROGMEM = "{ \
          \"unit_of_measurement\": \"%\", \
          \"icon\": \"mdi:heating-coil\", \
          \"name\": \"Solder reflow plate duty cycle\", \
          \"state_topic\": \"solder_reflow_plate/sensor/solder_reflow_plate_duty_cycle/state\", \
          \"availability_topic\": \"solder_reflow_plate/status\", \
          \"unique_id\": \"SolderReflowPlate_DutyCycle\", \
          \"device\": { \
          \"identifiers\": \"58:cf:79:a4:ee:cc\", \
          \"name\": \"solder_reflow_plate\", \
          \"sw_version\": \"VSC, ElegantOTA 1.0\", \
          \"model\": \"esp32-s2\", \
          \"manufacturer\": \"espressif\" \
          } \
          }";

// TODO (johboh): Get available profiles from Profiles.h
const char *HOME_ASSISTANT_PROFILE_CONFIG PROGMEM = "{ \
          \"icon\": \"mdi:book-open-page-variant-outline\", \
          \"name\": \"Solder reflow plate profile\", \
          \"options\": [\"Sn42Bi58 80x60mm\",\"Sn42Bi58 95x95mm\"], \
          \"state_topic\": \"solder_reflow_plate/select/solder_reflow_plate_profile/state\", \
          \"command_topic\": \"solder_reflow_plate/select/solder_reflow_plate_profile/command\", \
          \"availability_topic\": \"solder_reflow_plate/status\", \
          \"unique_id\": \"SolderReflowPlate_Profile\", \
          \"retain\": true, \
          \"device\": { \
          \"identifiers\": \"58:cf:79:a4:ee:cc\", \
          \"name\": \"solder_reflow_plate\", \
          \"sw_version\": \"VSC, ElegantOTA 1.0\", \
          \"model\": \"esp32-s2\", \
          \"manufacturer\": \"espressif\" \
          } \
          }";

const char *HOME_ASSISTANT_STATE_CONFIG PROGMEM = "{ \
          \"icon\": \"mdi:progress-clock\", \
          \"name\": \"Solder reflow plate state\", \
          \"state_topic\": \"solder_reflow_plate/sensor/solder_reflow_plate_state/state\", \
          \"availability_topic\": \"solder_reflow_plate/status\", \
          \"unique_id\": \"SolderReflowPlate_State\", \
          \"device\": { \
          \"identifiers\": \"58:cf:79:a4:ee:cc\", \
          \"name\": \"solder_reflow_plate\", \
          \"sw_version\": \"VSC, ElegantOTA 1.0\", \
          \"model\": \"esp32-s2\", \
          \"manufacturer\": \"espressif\" \
          } \
          }";

const char *HOME_ASSISTANT_START_CONFIG PROGMEM = "{ \
          \"icon\": \"mdi:play-circle\", \
          \"name\": \"Solder reflow plate start\", \
          \"command_topic\": \"solder_reflow_plate/button/solder_reflow_plate_start/command\", \
          \"availability_topic\": \"solder_reflow_plate/status\", \
          \"unique_id\": \"SolderReflowPlate_Start\", \
          \"retain\": false, \
          \"device\": { \
          \"identifiers\": \"58:cf:79:a4:ee:cc\", \
          \"name\": \"solder_reflow_plate\", \
          \"sw_version\": \"VSC, ElegantOTA 1.0\", \
          \"model\": \"esp32-s2\", \
          \"manufacturer\": \"espressif\" \
          } \
          }";

const char *HOME_ASSISTANT_STOP_CONFIG PROGMEM = "{ \
          \"icon\": \"mdi:stop-circle\", \
          \"name\": \"Solder reflow plate stop\", \
          \"command_topic\": \"solder_reflow_plate/button/solder_reflow_plate_stop/command\", \
          \"availability_topic\": \"solder_reflow_plate/status\", \
          \"unique_id\": \"SolderReflowPlate_Stop\", \
          \"retain\": false, \
          \"device\": { \
          \"identifiers\": \"58:cf:79:a4:ee:cc\", \
          \"name\": \"solder_reflow_plate\", \
          \"sw_version\": \"VSC, ElegantOTA 1.0\", \
          \"model\": \"esp32-s2\", \
          \"manufacturer\": \"espressif\" \
          } \
          }";

Remote::Remote(Controller &controller, Thermocouple &thermocouple, Voltage &voltage, Logger &logger, String host,
               String username, String password)
    : _logger(logger), _voltage(voltage), _controller(controller), _thermocouple(thermocouple), _mqtt(_wifi_client),
      _last_publish_ms(0), _host(host), _username(username), _password(password) {}

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

    const String topic_str = String(topic);
    const bool profile_topic = topic_str.equals("solder_reflow_plate/select/solder_reflow_plate_profile/command");
    const bool start_topic = topic_str.equals("solder_reflow_plate/button/solder_reflow_plate_start/command");
    const bool stop_topic = topic_str.equals("solder_reflow_plate/button/solder_reflow_plate_stop/command");

    if (profile_topic) {
      char buff[length + 1];
      memset(buff, 0, length + 1);
      mempcpy(buff, payload, length);
      String str(buff);
      _controller.selectProfile(str);
    } else if (start_topic) {
      const bool press = strncmp((char *)payload, "PRESS", length) == 0;
      if (press) {
        _controller.start();
      }
    } else if (stop_topic) {
      const bool press = strncmp((char *)payload, "PRESS", length) == 0;
      if (press) {
        _controller.stop();
      }
    }
  });

  _logger.setCallback([this](Logger::Severity severity, const char message[]) {
    switch (severity) {
    case Logger::Severity::Info:
      _mqtt.publish("solder_reflow_plate/debug/info", message);
      break;
    case Logger::Severity::Warning:
      _mqtt.publish("solder_reflow_plate/debug/waring", message);
      break;
    case Logger::Severity::Error:
      _mqtt.publish("solder_reflow_plate/debug/error", message);
      break;
    }
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

  bool published_target =
      _mqtt.publish_P("homeassistant/sensor/solder_reflow_plate/solder_reflow_plate_target_temperature/config",
                      (const uint8_t *)HOME_ASSISTANT_TARGET_TEMPERATURE_CONFIG,
                      strlen(HOME_ASSISTANT_TARGET_TEMPERATURE_CONFIG), true);
  Serial.println("Published target temperature HA: " + String(published_target));

  bool published_voltage_in = _mqtt.publish_P(
      "homeassistant/sensor/solder_reflow_plate/solder_reflow_plate_voltage_in/config",
      (const uint8_t *)HOME_ASSISTANT_VOLTAGE_IN_CONFIG, strlen(HOME_ASSISTANT_VOLTAGE_IN_CONFIG), true);
  Serial.println("Published voltage in HA: " + String(published_voltage_in));

  bool published_duty_cycle = _mqtt.publish_P(
      "homeassistant/sensor/solder_reflow_plate/solder_reflow_plate_duty_cycle/config",
      (const uint8_t *)HOME_ASSISTANT_DUTY_CYCLE_CONFIG, strlen(HOME_ASSISTANT_DUTY_CYCLE_CONFIG), true);
  Serial.println("Published duty cycle HA: " + String(published_duty_cycle));

  bool published_state =
      _mqtt.publish_P("homeassistant/sensor/solder_reflow_plate/solder_reflow_plate_state/config",
                      (const uint8_t *)HOME_ASSISTANT_STATE_CONFIG, strlen(HOME_ASSISTANT_STATE_CONFIG), true);
  Serial.println("Published state HA: " + String(published_state));

  bool published_profile =
      _mqtt.publish_P("homeassistant/select/solder_reflow_plate/solder_reflow_plate_profile/config",
                      (const uint8_t *)HOME_ASSISTANT_PROFILE_CONFIG, strlen(HOME_ASSISTANT_PROFILE_CONFIG), true);
  Serial.println("Published profile HA: " + String(published_profile));
  _mqtt.subscribe("solder_reflow_plate/select/solder_reflow_plate_profile/command");

  bool published_start =
      _mqtt.publish_P("homeassistant/button/solder_reflow_plate/solder_reflow_plate_start/config",
                      (const uint8_t *)HOME_ASSISTANT_START_CONFIG, strlen(HOME_ASSISTANT_START_CONFIG), true);
  Serial.println("Published start HA: " + String(published_start));
  _mqtt.subscribe("solder_reflow_plate/button/solder_reflow_plate_start/command");

  bool published_stop =
      _mqtt.publish_P("homeassistant/button/solder_reflow_plate/solder_reflow_plate_stop/config",
                      (const uint8_t *)HOME_ASSISTANT_STOP_CONFIG, strlen(HOME_ASSISTANT_STOP_CONFIG), true);
  Serial.println("Published stop HA: " + String(published_stop));
  _mqtt.subscribe("solder_reflow_plate/button/solder_reflow_plate_stop/command");

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
  unsigned long interval =
      _controller.inRunningState() ? FORCE_PUBLISH_INTERVAL_WHEN_RUNNING_MS : FORCE_PUBLISH_INTERVAL_MS;
  bool force_publish = now - _last_publish_ms > interval;

  if (force_publish) {
    String strval = String(_thermocouple.getAmbientTemperature());
    _mqtt.publish("solder_reflow_plate/sensor/solder_reflow_plate_internal_temperature/state", strval.c_str());

    auto bed_temperature = _thermocouple.getBedTemperature();
    if (!isnan(bed_temperature)) {
      strval = String(bed_temperature);
      _mqtt.publish("solder_reflow_plate/sensor/solder_reflow_plate_bed_temperature/state", strval.c_str());
    }

    strval = String(_controller.getTargetTemperature());
    _mqtt.publish("solder_reflow_plate/sensor/solder_reflow_plate_target_temperature/state", strval.c_str());

    strval = String(_voltage.getVinVoltage());
    _mqtt.publish("solder_reflow_plate/sensor/solder_reflow_plate_voltage_in/state", strval.c_str());

    strval = String(_voltage.getDutyCyclePercent() * 100.0);
    _mqtt.publish("solder_reflow_plate/sensor/solder_reflow_plate_duty_cycle/state", strval.c_str());

    auto current_profile = _controller.getSelectedProfile();
    if (current_profile != nullptr) {
      _mqtt.publish("solder_reflow_plate/select/solder_reflow_plate_profile/state", current_profile->getName().c_str());
    }

    switch (_controller.getCurrentState()) {
    case Controller::State::Idle:
      strval = String("idle");
      break;
    case Controller::State::NoProfileSelected:
      strval = String("no_profile_selected");
      break;
    case Controller::State::Error:
      strval = String("error");
      break;
    case Controller::State::Preheat:
      strval = String("preheating");
      break;
    case Controller::State::Soak:
      strval = String("soaking");
      break;
    case Controller::State::Reflow:
      strval = String("reflowing");
      break;
    default:
      strval = String("unknown");
      break;
    }
    _mqtt.publish("solder_reflow_plate/sensor/solder_reflow_plate_state/state", strval.c_str());

    _last_publish_ms = now;
  }

  if (now - _last_publish_ha_setup_ms > PUBLISH_SETUP_EVERY_MS) {
    publishHASetup();
    _last_publish_ha_setup_ms = now;
  }
}