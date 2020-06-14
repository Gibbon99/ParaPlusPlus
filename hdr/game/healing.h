#pragma once

#include "main.h"

extern double healingAnimSpeed;
extern float  healingDelayCounter;
extern int    healingAmountPerTick;

// Find out where on the map the healing tiles are
// Store in vector for this deck for animation
void gam_findHealingTiles (std::string deckName);

// Find out where on the map the healing tiles are and create a physic sensor for the current level
void gam_findHealingTilesPhysics ();

// Render current door frames onto map
void gam_renderHealingFrames ();

// Animate healing tiles on current level
void gam_animateHealing ();

// Check if the player is over a healing tile
void gam_processHealingTile ();

// Clear out memory and free bodies
void gam_clearHealing ();