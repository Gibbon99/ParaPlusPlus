#pragma once

#include "main.h"
#include "droidClass.h"


extern droidClass testDroid;
extern float      explosionAnimationSpeed;

// Create the droids for the level and init values
void gam_initDroids (const std::string& levelName);

// Animate the droid sprites
void gam_animateDroids ();

// Render the droids on the current level
void gam_renderDroids (std::string levelName);

// Test the aStar with a single droid
void createTestDroid ();

// Render the test droid
void renderTestDroid ();

// Process the AI for each droid
void gam_processAI ();

// Process damage to a droid
//
// damageSource can be either a bullet, explosion or a collision with player or another droid
void gam_damageToDroid (int targetDroid, int damageSource, int sourceDroid);

// Process the counter to ignore collisions
void gam_processCollision (int droidA);

// Check for any droids that need to be removed - died last tick
void gam_removeDroids ();

// Show the current droids attack target
void gam_debugShowTarget(droidClass whichDroid);

// Reset the droid information for a new game
void gam_resetDroids ();