#pragma once

#include "main.h"

extern int   numDroidTokens;
extern int   droidBlockPos;
extern int   numPlayerTokens;
extern int   playerBlockPos;
extern float chooseRowDelay;
extern float chooseRowDelayTime;

// Play the tranfer game
void trn_processTransferGame (float tickTime);
