#include "Profile.h"

Profile::Profile(Profile::Step steps[4]) : _steps(steps) {}

Profile::Step *Profile::getStep(Profile::State state) {
  for (uint8_t i = 0; i < NUMBER_OF_STEPS; ++i) {
    if (_steps[i].state == state) {
      return &_steps[i];
    }
  }
  return nullptr;
}