#include "Voltage.h"

#define READ_VIN_EVERY_MS 200
#define GATE_CHANNEL 0
#define GATE_FREQUENCY 32768
#define GATE_RESOLUTION_BITS 16

Voltage::Voltage(uint8_t vin_pin, uint8_t gate_pin) : _vin_pin(vin_pin), _gate_pin(gate_pin) {}

void Voltage::setup() {
  ledcSetup(GATE_CHANNEL, GATE_FREQUENCY, GATE_RESOLUTION_BITS);
  ledcAttachPin(_gate_pin, GATE_CHANNEL);
  ledcWrite(GATE_CHANNEL, 0); // Turn off
}

void Voltage::handle() {
  if (millis() - _last_vin_read_timestamp > READ_VIN_EVERY_MS) {
    readAdc();
    _last_vin_read_timestamp = millis();
  }
}

void Voltage::readAdc() {
  int val = 0;
  for (uint8_t i = 0; i < 5; i++) {
    val += analogRead(_vin_pin);
    delayMicroseconds(100);
  }
  _vin_voltage = ((val / 5) / 1023.0) * 100.0;
}
