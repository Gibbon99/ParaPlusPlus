#pragma once

#include "main.h"

// Start a new game
void gam_startNewGame();

// Clear previous level - remove physics objects from droid before changing level name
void gam_clearDroidPhysics(std::string levelName);