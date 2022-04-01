#include "Profiles.h"

Profile::Step sn42bi58_preheat = {
    .state = Profile::State::Preheat,
    .target_temperature_c = 108,
    .ramp_up_ms = 60000,
    .total_runtime_ms = 80000,
};

Profile::Step sn42bi58_soak = {
    .state = Profile::State::Soak,
    .target_temperature_c = 140,
    .ramp_up_ms = 40000,
    .total_runtime_ms = 80000,
};

Profile::Step sn42bi58_reflow = {
    .state = Profile::State::Reflow,
    .target_temperature_c = 170,
    .ramp_up_ms = 20000,
    .total_runtime_ms = 40000,
};

Profile::Step sn42bi58_cooling = {
    .state = Profile::State::Cooling,
    .target_temperature_c = 20,
    .ramp_up_ms = 1,
    .total_runtime_ms = 5000,
};

String sn42bi58_name = "Sn42Bi58";

Profile::Step sn42bi58_steps[4] = {sn42bi58_preheat, sn42bi58_soak, sn42bi58_reflow, sn42bi58_cooling};

Profile::Step debug_preheat = {
    .state = Profile::State::Preheat,
    .target_temperature_c = 50,
    .ramp_up_ms = 2000,
    .total_runtime_ms = 5000,
};

Profile::Step debug_soak = {
    .state = Profile::State::Soak,
    .target_temperature_c = 100,
    .ramp_up_ms = 2000,
    .total_runtime_ms = 5000,
};

Profile::Step debug_reflow = {
    .state = Profile::State::Reflow,
    .target_temperature_c = 150,
    .ramp_up_ms = 2000,
    .total_runtime_ms = 5000,
};

Profile::Step debug_cooling = {
    .state = Profile::State::Cooling,
    .target_temperature_c = 20,
    .ramp_up_ms = 1,
    .total_runtime_ms = 5000,
};

String debug_name = "debug";

Profile::Step debug_steps[4] = {debug_preheat, debug_soak, debug_reflow, debug_cooling};

Profiles::Profiles() : _profiles({Profile(sn42bi58_name, sn42bi58_steps), Profile(debug_name, debug_steps)}) {}

Profile *Profiles::getProfile(String &name) {
  uint8_t number_of_profiles = getNumberOfProfiles();
  for (uint8_t i = 0; i < number_of_profiles; ++i) {
    Profile *profile = &_profiles[i];
    if (name == profile->getName()) {
      return profile;
    }
  }
  return nullptr;
}
