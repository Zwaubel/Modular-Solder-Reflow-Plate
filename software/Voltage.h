#ifndef __VOLTAGE_H__
#define __VOLTAGE_H__

#include <Arduino.h>

/**
 * Read voltage in and control "voltage" out/mosfet.
 */
class Voltage {
public:
  /**
   * @brief Construct a new Voltage instance.
   *
   * @param vin_pin ADC pin for reading the current voltage in.
   * @param gate_pin pin for controlling Gate of MOSFET for heatbed.
   */
  Voltage(uint8_t vin_pin, uint8_t gate_pin);

  /**
   * @brief Call from the parent setup() function.
   */
  void setup();
  /**
   * @brief Call from the parent handle() function.
   */
  void handle();

  /**
   * @brief Force update voltate in value. This will block until done.
   */
  void update();

  /**
   * @brief Return max allowed Duty Cycle.
   * The duty cycle will be clamped to this value upon setting the duty cycle.
   */
  uint16_t maxDuty();

  /**
   * @brief Set the duty cycle. Will cap maximum duty cycle to the max allowed duty cycle.
   *
   * Max allowed duty cycle is given by [maxDuty] as well as minimum allowed Voltage in.
   * This method will make sure the voltage in is not below mimium allowed voltage in by
   * increasing the duty cycle until this is true..
   */
  void setDutyCycle(uint16_t duty_cycle);
  /**
   * @brief Get current/real duty cycle.
   */
  uint16_t getDutyCycle() { return _current_duty_cycle; }
  /**
   * @brief Get current/real duty cycle in percent (0.0-100.0).
   */
  float getDutyCyclePercent() { return getDutyCycle() / 1024.0 * 100.0; }

  /**
   * @brief Get current voltage in.
   */
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
