#pragma once

#include "main.h"

// Reset all the emitters ready for a new deck
void gam_clearEmitters();

// Add a new emitter to the array
void gam_addEmitter(cpVect newWorldPos, int newType, Uint32 newBulletID);

// Remove an emitter from an attached bullet
void gam_removeEmitter(Uint32 whichBulletID);

// Render the particles
void gam_renderParticles();

// Animate the particles
void gam_animateParticles();

// Remove dead emitters
void gam_removeDeadEmitters();