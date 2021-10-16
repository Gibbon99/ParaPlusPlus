#pragma once

#include "main.h"
#include "classes/paraDroid.h"

extern paraDroid playerDroid;
extern double     playerFriction;           // From script
extern float      influenceTimelimit;
extern float      influenceTimelimtDelay;
extern float      influenceTimeLeftWarning;
extern int        maxNumBumps;              // From script
extern float      bounceCounterDelay;       // From script


// Setup the player droid - run once
void gam_setupPlayerDroid ();

// Process player movement
void gam_processPlayerMovement ();

// Start all the actions resulting from pressing the action key
void gam_processActionKey ();

// Render the test circle
void gam_moveTestCircle ();

// Recharge player weapon
void gam_weaponRechargePlayer ();

// Damage to player droid
void gam_damageToPlayer (int damageSource, int sourceDroid);

// Check the players health, and set low health for animation
void gam_checkPlayerHealth ();

// Set the influence time limit - based on droid class
void gam_setInfluenceTimelimit (int targetDroidClass);

// Process how long a droid can be controlled for
void gam_processInfluenceTime ();

// Check and decrement the bump counter
void gam_checkBumpCounter ();

// Create a breadcrumb trail as the player moves around
void gam_createTrail ();

// Show the player trail
void gam_debugShowPlayerTrail();

// Reset the trail data when changing to a new level
void gam_clearPlayerTrail();

// Return the coordinates from the last position in the trial array
cpVect gam_getLastPlayerTrail();

// Reset the Influence time flag - so player doesn't change color
void gam_resetInfluenceTimeLeftFlag();