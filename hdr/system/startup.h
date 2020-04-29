#pragma once

#include "../main.h"

// Variables to start everything
extern int         logicalWinWidth;
extern int         logicalWinHeight;
extern int         windowWidth;
extern int         windowHeight;
extern int         consoleVirtualWidth;
extern int         consoleVirtualHeight;
extern int         consoleNumColumns;
extern int         consoleFontSize;
extern int         windowFullscreen;
extern int         windowFullscreenDesktop;
extern int         windowBorderless;
extern int         windowInputGrabbed;
extern int         windowInputFocus;
extern int         windowAllowHighDPI;
extern int         whichRenderer;
extern int         presentVSync;
extern int         renderScaleQuality;
extern int         volumeLevel;
extern int         maxNumChannels;
extern std::string consoleFontFilename;

extern std::string activeBackingTexture;

// Start systems - Logfile, Script, Filesystem ( Physfs ), Event queues
void sys_startSystems ();
