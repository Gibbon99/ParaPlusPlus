#pragma once

#include "main.h"
#include "game/transferGame.h"

extern int   chooseSideTimeOut;  // From script
extern float chooseSideDelayTime; // From script
extern float activeRowCounter;

// Setup the default values for the transfer rows
void trn_initTransferValues (int transferTargetIndex);

// Get ready for the second transfer screen
void trn_initTransferScreenTwo ();

// Return true if a transfer mistake is made
bool trn_makeTransferMistake (int whichDroidLevel);

// Process active circuits time
void trn_processCircuits ();

// Called to setup the transfer cell values
void trn_setupTransferCellValues ();

// Print out the types of cells
void trn_debugTransferCells (int whichSide);

// Prepare the transfer countdown sequence
void trn_prepareTransferCountDown ();

// Handle the keyboard for choosing a side for transfer game
void trn_handleTransferChooseSide ();

// Process the countdown timer for transfer
void trn_processTransferCountDown ();

// Return the time to display on the HUD
std::string trn_getCountdown ();

// Prepare to start the transfer game
void trn_prepareTransferGame ();

// Check split circuits
void trn_processSplitCircuits(__TRANSFER_ROW checkTransferRow);

// Check the playing sounds for transfer information screens, move on if still playing
void trn_checkTransferScreenSounds ();
