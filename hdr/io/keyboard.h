#pragma once

#include "main.h"

// Process the keyboard state and populate the keybindings array
void io_processKeyboardState();

// Read the keyboard events and set the state in the gui class
void io_setKeyboardState(SDL_Scancode keycode, int action, bool repeatPress);

// Called when a key is pressed on the keyboard while in keyinput mode
void io_setNewKeycodeValue(int newKeyCode);

// Function called from the GUI script to change the mode to input a new key value
void io_initNewKeycodeValue(int whichKey);