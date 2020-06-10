#pragma once

#include "main.h"

extern int   redAlertLevel;
extern int   yellowAlertLevel;
extern float updateScoreDelay;

// Modify the score buffer - send in positive or negative amount
void gam_modifyScore (int modifyAmount);

// Process the scoreDelay to see if the printableScore needs updating
void gam_processScore ();

// Return the current score
int gam_getPrintableScore ();

// Return the actual score - used for game over
int gam_getCurrentScore();