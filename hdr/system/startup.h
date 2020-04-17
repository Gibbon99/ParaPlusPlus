#pragma once

#include "../main.h"

extern int windowFullscreen;
extern int windowFullscreenDesktop;
extern int windowBorderless;
extern int windowInputGrabbed;
extern int windowInputFocus;
extern int windowAllowHighDPI;
extern int whichRenderer;
extern int presentVSync;

// Return the current renderer pointer
PARA_Renderer *sys_getRenderer();

// Return the current window handle
PARA_Window *sys_getWindow();

// Start the Windowing system
void sys_createScreen();

// Start systems - Logfile, Script, Filesystem ( Physfs ), Event queues
void sys_startSystems();

// Destroy the current window and renderer and create a new one
void sys_createNewScreen(int winWidth, int winHeight, int newRenderer);

