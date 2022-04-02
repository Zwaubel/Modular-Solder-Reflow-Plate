#ifndef __HEATER_H__
#define __HEATER_H__

#include "Thermocouple.h"
#include "Voltage.h"
#include <Arduino.h>

/**
 * Abstraction to control the heat bed.
 *
 * Pid references, not used currently:
 * https://newton.ex.ac.uk/teaching/CDHW/Feedback/Setup-PID.html
 *
 */
class Heater {
public:
  Heater(Voltage &voltage, Thermocouple &thermocouple);
  void setup();
  void handle();

  void start();
  void requestTemperature(float temperature);
  void stop();

private:
  void evaulate();

private:
  Voltage &_voltage;
  Thermocouple &_thermocouple;

private:
  bool _running = false;
  float _target_temperature = 0;
  unsigned long _last_evalulation_ms = 0;
};

#endif //__HEATER_H__