#include "Controller.h"

Controller::Controller(Voltage &voltage, StatusLeds &status_leds, Thermocouple &thermocouple)
    : _voltage(voltage), _status_leds(status_leds), _thermocouple(thermocouple) {}

void Controller::setup() {
  _voltage.setup();
  _status_leds.setup();
  _thermocouple.setup();

  // We are already off, but be safe.
  _voltage.setDutyCycle(0);
}

void Controller::handle() {
  _voltage.handle();
  _status_leds.handle();
  _thermocouple.handle();
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
  Serial.println(_voltage.getCurrentDutyCycle());
}