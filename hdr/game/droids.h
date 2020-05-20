#pragma once

#include "main.h"
#include "droidClass.h"


extern droidClass  testDroid;

// Create the droids for the level and init values
void gam_initDroids (std::string levelName);

// Animate the droid sprites
void gam_animateDroids(std::string levelName);

// Render the droids on the current level
void gam_renderDroids(std::string levelName);

// Test the aStar with a single droid
void createTestDroid();

// Render the test droid
void renderTestDroid();