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

  // Explicitly update values
  void update();

  uint16_t maxDuty(); // Limited
  void setDutyCycle(uint16_t duty_cycle);
  uint16_t getDutyCycle() { return _current_duty_cycle; }
  float getDutyCyclePercent() { return getDutyCycle() / 1024.0 * 100.0; }

  float getVinVoltage() { return _vin_voltage; }

private:
  void readAdc();
  void updatePwm(uint16_t duty_cycle);

private:
  const uint8_t _vin_pin;
  const uint8_t _gate_pin;

private:
  float _vin_voltage;
  uint16_t _current_duty_cycle = 0;
  unsigned long _last_vin_read_timestamp = 0;
};

#endif //__VOLTAGE_H__
