#pragma once

#include "../main.h"

// Start the Windowing system
void sys_createScreen();

// Start systems - Logfile, Script, Filesystem ( Physfs ), Event queues
void sys_startSystems();

// Return the current renderer pointer
PARA_Renderer *sys_getRenderer();

// Return the suface contained by the current window
PARA_Surface *sys_getScreenSurface();

// Return the current window handle
PARA_Window *sys_getWindow();
