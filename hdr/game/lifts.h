#pragma once

#include "main.h"
#include "shipDecks.h"
#include "system/cpPhysics.h"

struct __tileSensor
{
	float                      width {};
	float                      height {};
	cpVect                     worldPosition = {0, 0};
	cpBody                     *body {};
	cpShape                    *shape {};
	std::shared_ptr<userData_> userData {};
};

extern int currentTunnel;

// Position the player on the requested lift on the new level
cpVect gam_getLiftWorldPosition(int whichLift);

// Setup lifts
void gam_setupLifts();

// Get the positions of lifts
void gam_findLiftPositions(const std::string &levelName);

// Move the lift position
void gam_moveLift(int direction);

// Activate the lift
void gam_performLiftAction();

// Set the deck index
void gam_setCurrentTunnelDeckIndex();

// Return the current tunnel in use
int gam_getCurrentTunnel();

// Clear out memory and free bodies
void gam_clearLifts();