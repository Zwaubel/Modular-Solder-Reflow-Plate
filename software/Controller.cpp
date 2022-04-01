#include "Controller.h"

#define TIMEOUT_MS 3 * 60 * 1000 // 3 min

/*
 * TODO (johboh):
 * - Expose control to Remote/Home Assistant integration
 * - Implement heating profile:
 *   This is a bigger task of course. But basically we will use two input parameters, Vin and bed temperature.
 *   We have to measure vin to make sure it doesn't fall to low, if it does, we need to lower the duty cycle until
 *   it has reached a decent value again, then we can increase duty cycle and monitor if desired temperature has been
 *   reached.
 */
Controller::Controller(Voltage &voltage, StatusLeds &status_leds, Thermocouple &thermocouple, Logger &logger)
    : _logger(logger), _voltage(voltage), _status_leds(status_leds), _thermocouple(thermocouple) {}

void Controller::setup() {
  _voltage.setup();
  _status_leds.setup();
  _thermocouple.setup();

  // We are already off, but be safe.
  _voltage.setDutyCycle(0);

  // Up and running.
  _status_leds.setGreen(true);
  _status_leds.setRed(false);
}

void Controller::handle() {
  _voltage.handle();
  _status_leds.handle();
  _thermocouple.handle();
  handleSerialInput();

  auto now = millis();
  auto duty_cycle = _voltage.getDutyCycle();
  if (duty_cycle != 0 && now - _last_duty_change_timestamp > TIMEOUT_MS) {
    _voltage.setDutyCycle(0);
  }

  _status_leds.setRed(duty_cycle != 0);

  if (_current_profile != nullptr) {
    switch (_current_profile->getCurrentState()) {
    case Profile::State::Preheat:
      _current_state = State::Preheat;
      break;
    case Profile::State::Soak:
      _current_state = State::Soak;
      break;
    case Profile::State::Reflow:
      _current_state = State::Reflow;
      break;
    case Profile::State::Cooling:
      _current_state = State::Cooling;
      break;
    case Profile::State::None:
      _current_state = State::Idle;
      break;
    }
  }
}

bool Controller::selectProfile(String &profile_name) {
  _logger.log(Logger::Severity::Info, String("Trying to select profile <" + profile_name + ">").c_str());
  auto profile = _profiles.getProfile(profile_name);
  if (profile != nullptr) {
    _current_profile = profile;
    _logger.log(Logger::Severity::Info, String("Profile <" + profile_name + "> selected.").c_str());
    return true;
  } else {
    _logger.log(Logger::Severity::Error, String("Cannot select profile <" + profile_name + ">").c_str());
    _current_state = State::Error;
    return false;
  }
}

bool Controller::start() {
  _logger.log(Logger::Severity::Info, "Trying to start reflow.");
  if (_current_profile != nullptr) {
    _current_state = State::Idle;
    _current_profile->start(_thermocouple.getAmbientTemperature());
    return true;
  } else {
    _logger.log(Logger::Severity::Error, "Cannot start reflow. No profile selected.");
    _current_state = State::NoProfileSelected;
    return false;
  }
}

void Controller::stop() {
  _current_state = State::Idle;
  _voltage.setDutyCycle(0);
  if (_current_profile != nullptr) {
    _current_profile->reset();
  }
}

bool Controller::inRunningState() {
  return _current_state == State::Preheat || _current_state == State::Soak || _current_state == State::Reflow ||
         _current_state == State::Cooling;
}

void Controller::handleSerialInput() {
  if (Serial.available() > 0) {
    uint8_t input = Serial.read();
    // Map keyboard 1-9 + 0 as duty cycle 10-90%.
    uint8_t duty = ((input - 48) / 20.0) * 255;
    _voltage.setDutyCycle(duty);

    // Scrap trailing chars
    while (Serial.available()) {
      Serial.read();
    }

    _last_duty_change_timestamp = millis();
  }
}

void Controller::printDebug() {
  Serial.print("Internal temperature(C) = ");
  Serial.println(_thermocouple.getAmbientTemperature());

  double c = _thermocouple.getBedTemperature();
  if (isnan(c)) {
    Serial.println("Something wrong with thermocouple!");
  } else {
    Serial.print("Bed temperature(C) = ");
    Serial.println(c);
  }

  Serial.print("Vin(V) = ");
  Serial.println(_voltage.getVinVoltage());

  Serial.print("Current duty(%) = ");
  Serial.println(_voltage.getDutyCyclePercent());

  if (_current_profile != nullptr) {
    Serial.print("Current target(C) = ");
    Serial.println(_current_profile->targetTemperature(_thermocouple.getBedTemperature()));
  } else {
    Serial.println("No current target");
  }
}