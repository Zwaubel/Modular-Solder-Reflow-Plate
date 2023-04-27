#include "StatusLeds.h"

StatusLeds::StatusLeds(uint8_t neopixel_data_pin) : _neopixel_data_pin(neopixel_data_pin) {
  FastLED.setMaxPowerInVoltsAndMilliamps(LED_VOLTAGE, LED_MAX_CURRRENT);
  FastLED.addLeds<LED_TYPE, PIN_LED_DATALINE, LED_COLOR_ORDER>(leds, LED_NUMBER).setCorrection(TypicalLEDStrip);
}

void StatusLeds::setup() {}

void StatusLeds::handle() {}

void StatusLeds::setRed() {
  for (uint8_t i = 0; i < LED_NUMBER; i++) {
    leds[i] = CRGB::Red;
  }
  FastLED.show();
}

void StatusLeds::setGreen() {
  for (uint8_t i = 0; i < LED_NUMBER; i++) {
    leds[i] = CRGB::Green;
  }
  FastLED.show();
}

void StatusLeds::setOff() {
  for (uint8_t i = 0; i < LED_NUMBER; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}