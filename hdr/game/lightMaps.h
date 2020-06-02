#pragma once

#include <box2d/b2_math.h>
#include "main.h"

// Remove all the lightmaps - ready for new level
void gam_removeAllLightmaps();

// Remove a lightmap - mark inUse to false - pass in bulletID to match
void gam_removeLightmap(Uint32 whichBullet);

// Add a new lightmap to the array
void gam_addNewLightmap(b2Vec2 newWorldPosition, int newType, Uint32 newWhichBullet);

// Render the lightmaps
void gam_renderLightmaps();

// Animate the lightmaps
void gam_animateLightmaps();

// Change the color of lightmaps
void gam_changeAlertColor (int newAlertLevel);