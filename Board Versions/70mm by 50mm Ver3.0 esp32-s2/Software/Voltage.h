#include <Arduino.h>

#ifndef __VOLTAGE_H__
#define __VOLTAGE_H__

/**
 * Read voltage in and control "voltage" out.
 */
class Voltage {
public:
  Voltage(uint8_t vin_pin, uint8_t gate_pin);

  void setup();
  void handle();

  float getVinVoltage() { return _vin_voltage; }

private:
  void readAdc();

private:
  const uint8_t _vin_pin;
  const uint8_t _gate_pin;
  unsigned long _last_vin_read_timestamp = 0;
  float _vin_voltage;
};

#endif //__VOLTAGE_H__
