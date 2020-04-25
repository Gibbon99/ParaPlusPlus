#pragma once

#include "../main.h"

// Variables to start everything
extern int logicalWinWidth;
extern int logicalWinHeight;
extern int windowWidth;
extern int windowHeight;
extern int consoleWinWidth;
extern int consoleWinHeight;
extern int consoleNumColumns;
extern int consoleFontSize;
extern int windowFullscreen;
extern int windowFullscreenDesktop;
extern int windowBorderless;
extern int windowInputGrabbed;
extern int windowInputFocus;
extern int windowAllowHighDPI;
extern int whichRenderer;
extern int presentVSync;
extern int renderScaleQuality;
extern std::string consoleFontFilename;

extern std::string activeBackingTexture;

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
PARA_Texture *sys_getRenderTarget(const std::string& textureName);

// Return if render to texture is in use
bool sys_useRenderTarget();

// Create the render target texture
void sys_createRenderTargetTexture(const std::string& textureName, int targetWidth, int targetHeight);

std::string sys_getCurrentBackingTexture();

// Make a new backing texture active
void sys_setCurrentBackingTexture(const std::string& newActiveTexture);

// Display the current backing texture information to the console
void sys_debugBackingTextures();

// Get information on the current renderer
void sys_debugGetCurrentRenderer();