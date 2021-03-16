#pragma once

#include "main.h"
#include "droidClass.h"

extern int visibleFadeValue;

// Check each droid to see if the player can see them or not - set isVisible flag to reflect
void gam_checkLOS(droidClass &droidItr);