#pragma once

#include "main.h"

// Init a joystick if found
void io_initJoystick();

void io_joyMovement(SDL_JoystickID whichJoystick, Uint8 whichAxis, Sint16 axisValue);

void io_mapJoyToInput();

// Close any open joysticks
void io_closeJoystick();