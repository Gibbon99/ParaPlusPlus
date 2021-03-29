#pragma once

#include "main.h"
#include "game/droidClass.h"

extern droidClass playerDroid;
extern double     playerFriction;      // From script
extern float      influenceTimelimit;
extern float      influenceTimelimtDelay;
extern float      influenceTimeLeftWarning;


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

// Return the type of droid for checking access in database view
int gam_getDroidType();