#ifndef __PROFILES_H__
#define __PROFILES_H__

#include "Profile.h"
#include <Arduino.h>

/**
 * Handle soldering profiles.
 */
class Profiles {
public:
  /**
   * @brief Construct a new Profiles instance, containing all available profiles.
   */
  Profiles();

public:
  /**
   * @brief Get a profile given its name. Will return nullptr if the profile cannot be found.
   */
  Profile *getProfile(String &name);
  /**
   * @brief Get the total number of profiles.
   */
  uint8_t getNumberOfProfiles() { return sizeof(_profiles) / sizeof(Profile); }

private:
  Profile _profiles[2];
};

#endif //__PROFILES_H__
