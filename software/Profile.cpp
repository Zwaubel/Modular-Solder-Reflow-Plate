#include "Profile.h"

Profile::Profile(String &name, Profile::Step steps[4]) : _name(name), _steps(steps) {}

Profile::Step const *Profile::getStep(Profile::State state) {
  for (uint8_t i = 0; i < NUMBER_OF_STEPS; ++i) {
    if (_steps[i].state == state) {
      return &_steps[i];
    }
  }
  return nullptr;
}

void Profile::reset() {
  _current_state == Profile::State::None;
  _k = 0;
  _m = 0;
}

void Profile::start(float idle_temperature) {
  auto now = millis();
  _start_time_ms = now;
  _step_start_time_ms = now;
  _current_state = Profile::State::Preheat;

  calculateKM(idle_temperature);
}

uint16_t Profile::targetTemperature(float current_temperature) {
  auto step = getStep(_current_state);
  if (step != nullptr) {
    unsigned long x = millis() - _step_start_time_ms;

    if (x > step->runtime_ms) {
      Serial.print("runtime exceeded, switching current state: ");
      switch (_current_state) {
      case State::Preheat:
        _current_state = State::Soak;
        Serial.println("Preheat -> Soak");
        break;
      case State::Soak:
        _current_state = State::Reflow;
        Serial.println("Soak -> Reflow");
        break;
      case State::Reflow:
        _current_state = State::Cooling;
        Serial.println("Reflow -> Cooling");
        break;
      case State::Cooling:
        _current_state = State::None;
        Serial.println("Cooling -> None");
        break;
      default:
        Serial.println("Unknown!");
        break;
      }

      calculateKM(current_temperature);
      x = 0;
      _step_start_time_ms = millis();
    }

    uint16_t target_temperature = _k * x + _m;

    return target_temperature;
  } else {
    // Oh no!
    return 0;
  }
}

void Profile::calculateKM(float zero_time_temperature) {
  auto step = getStep(_current_state);
  if (step != nullptr) {
    _m = zero_time_temperature;
    _k = ((double)step->target_temperature_c - _m) / ((double)step->runtime_ms);
  } else {
    _k = 0;
    _m = 0;
  }
  Serial.println("Caulate KM for " + String(zero_time_temperature, 2) + ", k: " + String(_k, 10) +
                 ", m: " + String(_m, 10));
}