#ifndef __STATUS_LEDS_H__
#define __STATUS_LEDS_H__

#include "../../HAL/PinDef.h"
#include <Arduino.h>
#include <FastLED.h>

/**
 * Control status leds.
 */
class StatusLeds {
public:
  /**
   * @brief Construct a new Status Leds instance.
   *
   * @param neopixel_data_pin pin where neopixel LED's data line is connected.
   */
  StatusLeds(uint8_t neopixel_data_pin);

  /**
   * @brief Call from the parent setup() function.
   */
  void setup();
  /**
   * @brief Call from the parent handle() function.
   */
  void handle();

  /**
   * @brief Set led color to red
   */
  void setRed();

  /**
   * @brief Set led color to green.
   */
  void setGreen();

  /**
   * @brief Turn leds off.
   */
  void setOff();

private:
  const uint8_t _neopixel_data_pin;
  const CRGBArray<LED_NUMBER> leds;
};

#endif //__STATUS_LEDS_H__