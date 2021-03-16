#pragma once

#include "main.h"

extern float staticAnimationDelay;
extern int   lostScreenShowTime;

// Pre rendering of the end screen - prepare all resources
void gui_preRenderLostScreen ();

// Render the lost screen
void gui_renderLostScreen ();

// Process the lost screen
void gui_processLostScreen ();