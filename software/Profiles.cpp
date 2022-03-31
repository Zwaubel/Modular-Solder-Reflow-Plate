#include "Profiles.h"

Profile::Step sn42bi58_preheat = {
    .state = Profile::State::Preheat,
    .target_temperature_c = 108,
    .max_degrees_per_second = 3,
    .runtime_seconds = 80,
};

Profile::Step sn42bi58_soak = {
    .state = Profile::State::Soak,
    .target_temperature_c = 140,
    .max_degrees_per_second = 2,
    .runtime_seconds = 80,
};

Profile::Step sn42bi58_reflow = {
    .state = Profile::State::Reflow,
    .target_temperature_c = 170,
    .max_degrees_per_second = 5,
    .runtime_seconds = 40,
};

Profile::Step sn42bi58_cooling = {
    .state = Profile::State::Cooling,
    .target_temperature_c = 20,
    .max_degrees_per_second = -1,
    .runtime_seconds = -1,
};

Profile::Step sn42bi58_steps[4] = {sn42bi58_preheat, sn42bi58_soak, sn42bi58_reflow, sn42bi58_cooling};

Profiles::Profiles() : _profiles({Profile(sn42bi58_steps)}) {}