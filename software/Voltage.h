#ifndef __VOLTAGE_H__
#define __VOLTAGE_H__

#include <Arduino.h>

/**
 * Read voltage in and control "voltage" out.
 */
class Voltage {
public:
  Voltage(uint8_t vin_pin, uint8_t gate_pin);

  void setup();
  void handle();

  void setDutyCycle(uint8_t duty_cycle);
  uint8_t getDutyCycle() { return _current_duty_cycle; }
  float getDutyCyclePercent() { return getDutyCycle() / 255.0 * 100.0; }

  float getVinVoltage() { return _vin_voltage; }

private:
  void readAdc();

private:
  const uint8_t _vin_pin;
  const uint8_t _gate_pin;

private:
  float _vin_voltage;
  uint8_t _current_duty_cycle = 0;
  unsigned long _last_vin_read_timestamp = 0;
};

#endif //__VOLTAGE_H__
