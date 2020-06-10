#pragma once

#include "main.h"

extern double healingAnimSpeed;
extern float  healingDelayCounter;
extern int    healingAmountPerTick;

// Find out where on the map the healing tiles are
// Store in vector for this deck for animation
void gam_findHealingTiles (std::string deckName);

// Render current door frames onto map
void gam_renderHealingFrames (const string &deckName);

// Animate healing tiles on current level
void gam_animateHealing (const std::string &deckName);

// Check if the player is over a healing tile
void gam_processHealingTile ();