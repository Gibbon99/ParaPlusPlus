#pragma once

#include "main.h"

// Handle a mouse motion event - also has button state for putting into input state for action key
void io_mouseMovement(Sint32 newPosX, Sint32 newPosY, Sint32 buttonState);

// Map the mouse state into the input state - overriding keyboard and joystick events
void io_mapMouseToInput();

// Getting the mouse button state is too fast using mouse motion event
void io_mouseButtonDown();

// Mouse scroll wheel was used - emulate the up and down keys
void io_mouseWheel(int whichKey);