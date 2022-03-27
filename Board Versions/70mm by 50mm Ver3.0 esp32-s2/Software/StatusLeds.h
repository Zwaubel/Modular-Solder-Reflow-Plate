#include <Arduino.h>

#ifndef __STATUS_LEDS_H__
#define __STATUS_LEDS_H__

class StatusLeds {
public:
  StatusLeds(uint8_t led_red_pin, uint8_t led_green_pin);
  void setup();
  void handle();

private:
  const uint8_t _led_red_pin;
  const uint8_t _led_green_pin;
  bool _last_led_value = false;
  unsigned long _last_led_toggle_timestamp = 0;
};

#endif //__STATUS_LEDS_H__