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
    /**
     * No state.
     */
    None,
  };

  struct Step {
    /**
     * What this step represent in the profile state.
     */
    Profile::State state;
    /**
     * Target temperature.
     */
    uint16_t target_temperature_c;
    /**
     * How loing this step should run, in milliseconds.
     */
    unsigned long runtime_ms;
  };

  Profile(String &name, Profile::Step steps[NUMBER_OF_STEPS]);

public:
  Profile::Step const *getStep(Profile::State state);

  void reset();
  void start(float idle_temperature);

  uint16_t targetTemperature(float current_temperature);

  String const &getName() { return _name; }
  Profile::State getCurrentState() { return _current_state; }

private:
  void calculateKM(float zero_time_temperature);

private:
  String const _name;
  Profile::Step const *_steps;

private:
  Profile::State _current_state = Profile::State::None;
  unsigned long _start_time_ms = 0;
  unsigned long _step_start_time_ms = 0;

  double _k = 0;
  double _m = 0;
};

#endif //__PROFILE_H__
