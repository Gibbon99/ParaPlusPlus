#pragma once

#include "main.h"

struct __TRANSFER_ROW
{
	bool  rightSideActive;
	bool  leftSideActive;
	bool  rightSideActiveIsOn;
	bool  leftSideActiveIsOn;
	int   index;
	int   leftSideType;
	int   rightSideType;
	int   currentColor;
	float leftSideActiveCounter;
	float rightSideActiveCounter;
	float startX;
	float startY;
	float rightSideActiveAlphaColor;
	float leftSideActiveAlphaColor;
};

extern int                         transferPlayerWhichSide;
extern int                         playerBlockPos;
extern float                       transferDelayTime;
extern int                         transferTimeOut;
extern int                         transferTimeoutCountdown;
extern int                         transferResultDelay;
extern std::vector<__TRANSFER_ROW> transferRows;

// Play the transfer game - mode MODE_TRANSFER_GAME
void trn_processTransferGame ();

// Return the string showing the transfer countdown
std::string trn_getTransferCountdown ();

// Place a token onto a circuit
void trn_placeToken (int rowIndex, int whichSide);

// Transfer into the droid
void trn_transferIntoDroid ();

// Lost the transfer - back to 001
void trn_transferLostGame ();

// Deadlock the transfer - wait for sound to finish
void trn_processDeadlock ();