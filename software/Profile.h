#ifndef __PROFILE_H__
#define __PROFILE_H__

#include <Arduino.h>

#define NUMBER_OF_STEPS 4

/**
 * A single solder profile.
 *
 * Source of profiles: https://www.compuphase.com/electronics/reflowsolderprofiles.htm
 */
class Profile {
public:
  enum class State {
    None,
    /**
     * The preheating zone should increase the temperature at a maximum rate of 3 °C/s.
     *  The purpose of preheating is to allow the solvents to evaporate and to activate the flux.
     */
    Preheat,
    /**
     * The soak zone brings the temperature of all components and board areas to an equal level.
     * Due to differences in thermal inertia, components do not heat up at the same speed.
     */
    Soak,
    /**
     * In the reflow zone, the temperature rises at a rate of approximately 2 °C to a
     * temperature above the melting point (the temperature above liquidus, or TAL).
     * The peak temperature for the reflow zone is at least 25 °C above TAL,
     * because solder both coalesces better with the copper and wets the pads and
     * component pins better when it is hotter —thereby creating better joints.
     * This is especially important for lead-free solders (SAC305), because this
     * solder is less effective at wetting than leaded solders.
     */
    Reflow,
    /**
     * The cooling zone ideally has a ramp of 4 °C/s (to cool down quickly, but avoid thermal stress).
     * Free air cooling is sufficient.
     */
    Cooling,
  };

  struct Step {
    Profile::State state;
    uint16_t target_temperature_c;
    int8_t max_degrees_per_second;
    int16_t runtime_seconds;
  };

  Profile(Profile::Step steps[NUMBER_OF_STEPS]);

public:
  Profile::Step *getStep(Profile::State state);

private:
  Profile::Step *_steps;
};

#endif //__PROFILE_H__
