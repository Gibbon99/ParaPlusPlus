#pragma once

#include "../main.h"

extern int logicalWinWidth;
extern int logicalWinHeight;
extern int windowWidth;
extern int windowHeight;
extern int windowFullscreen;
extern int windowFullscreenDesktop;
extern int windowBorderless;
extern int windowInputGrabbed;
extern int windowInputFocus;
extern int windowAllowHighDPI;
extern int whichRenderer;
extern int presentVSync;
extern int renderScaleQuality;

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

// Return the render target texture handle
PARA_Texture *sys_getRenderTarget();

// Return if render to texture is in use
bool sys_useRenderTarget();