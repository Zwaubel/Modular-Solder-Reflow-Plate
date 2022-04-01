#ifndef __PROFILES_H__
#define __PROFILES_H__

#include "Profile.h"
#include <Arduino.h>

/**
 * Handle soldering profiles.
 * Probably a bit overkill.
 */
class Profiles {
public:
  Profiles();

public:
  Profile *getProfile(String &name);
  uint8_t getNumberOfProfiles() { return sizeof(_profiles) / sizeof(Profile); }

private:
  Profile _profiles[2];
};

#endif //__PROFILES_H__
