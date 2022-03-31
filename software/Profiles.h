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

  enum class SolderPasteType {
    Sn42Bi58, // Lead-free
  };

public:
  Profile getProfile(SolderPasteType solder_paste_type);

private:
  Profile _profiles[1];
};

#endif //__PROFILES_H__
