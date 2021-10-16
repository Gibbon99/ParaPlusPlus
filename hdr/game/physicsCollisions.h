#pragma once

#include "main.h"
#include "system/cpPhysics.h"

extern cpBool doWallCollisions;

// Setup all the collision handlers
void sys_setupCollisionHandlers();

// Set player vs wall collisions
void gam_setWallCollisions(bool newState);

