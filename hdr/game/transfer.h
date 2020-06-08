#pragma once

#include "main.h"

extern int chooseSideTimeOut;  // From script
extern float chooseSideDelayTime; // From script

// Setup the default values for the transfer rows
void trn_initTransferValues (int transferTargetIndex);

// Get ready for the second transfer screen
void trn_initTransferScreenTwo();

// Return true if a transfer mistake is made
bool trn_makeTransferMistake (int whichDroidLevel);

// Process active circuits time
void trn_processCircuits (float tickTime);

// Called to setup the transfer cell values
void trn_setupTransferCellValues ();

// Print out the types of cells
void trn_debugTransferCells(int whichSide);