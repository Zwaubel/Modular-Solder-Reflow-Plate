#include "Thermocouple.h"
#include <SPI.h>

#define READ_THERMOCOUPLE_EVERY_MS 100

Thermocouple::Thermocouple(uint8_t cs_pin, uint8_t sck_pin, uint8_t so_pin) : _max31855(sck_pin, cs_pin, so_pin) {}

void Thermocouple::setup() {
  Serial.print("Setting up max31855... ");
  int8_t retries = 3;
  while (!_max31855.begin()) {
    Serial.print("Failed!");
    if (--retries < 0) {
      break;
    }
    Serial.println("Retrying setup of max31855... ");
    delay(200);
  }
  if (retries > 0) {
    Serial.print("Success!");
  }
}

void Thermocouple::handle() {
  auto now = millis();
  if (now - _last_read_timestamp > READ_THERMOCOUPLE_EVERY_MS) {
    _last_read_timestamp = now;
    _ambient_temperature = _max31855.readInternal();
    _bed_temperature = _max31855.readCelsius();
  }
}
