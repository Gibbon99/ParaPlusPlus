#pragma once

#include "main.h"
#include "classes/paraDroid.h"


//extern paraDroid testDroid;
extern float explosionAnimationSpeed;

// Create the droids for the level and init values
void gam_initDroids(const std::string &levelName);

// Animate the droid sprites
void gam_animateDroids();

// Render the droids on the current level
void gam_renderDroids();

// Test the aStar with a single droid
void createTestDroid();

// Render the test droid
void renderTestDroid();

// Process the AI for each droid
void gam_processAI();

// Process damage to a droid
//
// damageSource can be either a bullet, explosion or a collision with player or another droid
void gam_damageToDroid(int targetDroid, int damageSource, int sourceDroid);

// Process the counter to ignore collisions
void gam_processCollision(int droidA);

// Check for any droids that need to be removed - died last tick
void gam_removeDroids();

// Show the current droids attack target
void gam_debugShowTarget(paraDroid whichDroid);

// Reset the droid information for a new game
void gam_resetDroids();

// Work out the droid animation speed based on health
void gam_setHealthAnimation(int targetDroid);

// See if droid is dead - set explosion sprite and change mode
void gam_checkDroidHealth(paraDroid *whichDroid);

// See if a droid sees another droid get shot, or get transferred into
void gam_checkActionWitness(int sourceDroid);

// Set velocity to zero
void gam_setDroidVelocity(int targetDroid, cpVect newVelocity);

// Get the current mode of the droid
int gam_getDroidCurrentMode(int targetDroid);