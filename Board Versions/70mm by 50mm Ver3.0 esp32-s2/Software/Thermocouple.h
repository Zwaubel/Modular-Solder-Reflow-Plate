#ifndef __THERMOCOUPLE_H__
#define __THERMOCOUPLE_H__

#include "Adafruit_MAX31855.h"
#include <Arduino.h>

/**
 * Read Thermocouple for internal and heatbed temperature.
 */
class Thermocouple {
public:
  Thermocouple(uint8_t cs_pin, uint8_t sck_pin, uint8_t so_pin);

  void setup();
  void handle();

  double getAmbientTemperature() { return _ambient_temperature; }
  // Must check for isnan() before usage.
  double getHeatbedTemperature() { return _heatbed_temperature; }

private:
  Adafruit_MAX31855 _max31855;
  unsigned long _last_read_timestamp = 0;
  double _ambient_temperature = NAN;
  double _heatbed_temperature = NAN;
};

#endif //__THERMOCOUPLE_H__
