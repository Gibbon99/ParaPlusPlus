#pragma once

#include "../main.h"

// Process the keyboard state and populate the keybindings array
void io_processKeyboardState();

// Read the keyboard events and set the state in the gui class
void io_setKeyboardState (SDL_Scancode keycode, int action, bool repeatPress);