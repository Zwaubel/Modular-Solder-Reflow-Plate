#include "heater.h"

#define EVALUATE_EVERY_MS 100

Heater::Heater(Voltage &voltage, Thermocouple &thermocouple) : _voltage(voltage), _thermocouple(thermocouple) {}

void Heater::setup() { _voltage.setDutyCycle(0); }

void Heater::handle() {
  if (_running) {
    auto now = millis();
    if (now - _last_evalulation_ms > EVALUATE_EVERY_MS) {
      evaulate();
      _last_evalulation_ms = now;
    }
  } else {
    _voltage.setDutyCycle(0);
  }
}

void Heater::start() {
  _running = true;
  _last_evalulation_ms = 0;
}

void Heater::requestTemperature(float temperature) { _target_temperature = temperature; }

void Heater::stop() {
  _voltage.setDutyCycle(0);
  _running = false;
}

// TODO, PID
void Heater::evaulate() {
  auto now = millis();
  auto max_duty = _voltage.maxDuty();
  auto duty = _voltage.getDutyCycle();

  if (_target_temperature <= 20) {
    _voltage.setDutyCycle(0);
    return;
  }

  _thermocouple.update();
  float temperature = _thermocouple.getBedTemperature();
  if (isnan(temperature)) {
    // Oh no, no temperature! Try again soon.
    _last_evalulation_ms = millis() + EVALUATE_EVERY_MS / 2;
    return;
  }

  if (_target_temperature > temperature && duty < max_duty) {
    // How far away are we?
    float distance = _target_temperature - temperature;
    auto new_duty = distance * (max_duty / 10.0);
    _voltage.setDutyCycle(new_duty);
  } else if (_target_temperature < temperature && duty > 0) {
    // We want to decrease.
    _voltage.setDutyCycle(0);
  }

  _last_evalulation_ms = millis();
}