#pragma once

#include "main.h"
#include "game/droidClass.h"

extern droidClass playerDroid;
extern double     playerFriction;      // From script

// Setup the player droid - run once
void gam_setupPlayerDroid ();

// Process player movement
void gam_processPlayerMovement ();

// Start all the actions resulting from pressing the action key
void gam_processActionKey ();