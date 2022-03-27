#include "Controller.h"

#define TIMEOUT_MS 3 * 60 * 1000 // 3 min

Controller::Controller(Voltage &voltage, StatusLeds &status_leds, Thermocouple &thermocouple)
    : _voltage(voltage), _status_leds(status_leds), _thermocouple(thermocouple) {}

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
  auto current_duty_cycle = _voltage.getCurrentDutyCycle();
  if (current_duty_cycle != 0 && now - _last_duty_change_timestamp > TIMEOUT_MS) {
    _voltage.setDutyCycle(0);
  }

  _status_leds.setRed(current_duty_cycle != 0);
}

void Controller::handleSerialInput() {
  if (Serial.available() > 0) {
    uint8_t input = Serial.read();
    // Map keyboard 1-9 + 0 as duty cycle 10-90%.
    uint8_t duty = ((input - 48) / 10.0) * 255;
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

  double c = _thermocouple.getHeatbedTemperature();
  if (isnan(c)) {
    Serial.println("Something wrong with thermocouple!");
  } else {
    Serial.print("Heatbed temperature(C) = ");
    Serial.println(c);
  }

  Serial.print("Vin(V) = ");
  Serial.println(_voltage.getVinVoltage());

  Serial.print("Current duty(%) = ");
  Serial.println(_voltage.getCurrentDutyCycle() / 255.0 * 100.0);
}