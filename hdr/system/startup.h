#pragma once

#include "../main.h"

// Variables to start everything
extern int gameWinWidth;
extern int gameWinHeight;
extern int windowWidth;
extern int         windowHeight;
extern int         hiresVirtualWidth;
extern int         hiresVirtualHeight;
extern int         consoleNumColumns;
extern int         consoleFontSize;
extern int         windowFullscreen;
extern int         windowFullscreenDesktop;
extern bool        windowBorderless;
extern int         windowInputGrabbed;
extern int         windowInputFocus;
extern bool        windowAllowHighDPI;
extern int         whichRenderer;
extern bool        presentVSync;
extern int         renderScaleQuality;
extern int         g_volumeLevel;
extern int         maxNumChannels;
extern int         guiFontSize;
extern std::string guiFontFileName;
extern std::string consoleFontFilename;
extern int         introFontSize;
extern std::string introFontFileName;
extern bool        enableSound;
extern bool        doScreenEffect;

extern std::string activeBackingTexture;

// Start systems - Logfile, Script, Filesystem ( Physfs ), Event queues
void sys_startSystems ();

// Start to load and prepare everything else
static int sys_startInit(void *ptr);