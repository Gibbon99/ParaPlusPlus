#pragma once

#include "wrapper.h"
#include <iostream>
#include <string>
#include <vector>
#include <classes/paraAudio.h>
#include <classes/paraTexture.h>
#include <classes/paraGui.h>
#include <classes/paraSprite.h>
#include <classes/paraStarfield.h>

#ifdef WIN32
#include "SDL.h"
#else

#include "SDL2/SDL.h"

#endif

#define APP_NAME    "Para++"

//#include "../data/scripts/enum.h"
#include "classes/paraScript.h"
#include "classes/paraLogFile.h"
#include "classes/paraFileSystem.h"
#include "classes/paraFont.h"
#include "classes/paraConsole.h"
#include "classes/paraAudio.h"
#include "system/eventsEngine.h"
#include "system/shutdown.h"
//#include "system/util.h"
#include "physfs/physfs.h"
#include "classes/paraRenderer.h"

#define ERROR_COUNT_LIMIT   10

#define HIRES_BACKING_TEXTURE "hiresBackingTexture"
#define GAME_BACKING_TEXTURE "gameBackingTexture"

#define NO_PASS_TILE                40
#define LIFT_TILE                   NO_PASS_TILE + 1
#define HEALING_TILE                LIFT_TILE + 4

#define ALERT_GREEN_TILE            4
#define ALERT_YELLOW_TILE           5
#define ALERT_RED_TILE              6

#define TERMINAL_TOP                51
#define TERMINAL_BOTTOM             52
#define TERMINAL_RIGHT              53
#define TERMINAL_LEFT               54

#define HEALING_TILE                LIFT_TILE + 4

#define SPRITE_SIZE                 24      // Pixels

#define TICKS_PER_SECOND 30.0f
#define MAX_FRAMESKIP 5

using namespace std;

typedef int (*functionPtr) (...);

extern paraScript     paraScriptInstance;
extern paraLogFile    logFile;
extern paraFileSystem fileSystem;
extern paraFont       fontClass;
extern paraConsole    console;
extern paraRenderer   renderer;
extern paraAudio      audio;
extern paraGui        gui;
extern paraStarfield  backgroundStarfield;
extern paraStarfield  sideviewStarfield;

extern paraSprite databaseSprite;

extern std::map<std::string, paraTexture> textures;

extern bool   quitLoop;
extern Uint32 fpsPrint;
extern Uint32 thinkFPSPrint;
extern double percentIntoNextFrame;
extern int    currentMode;
extern double pixelsPerMeter;          // From script
extern bool   d_showPerfStats;
//
// Game levers
//
extern double baseGameSpeed;

extern unsigned long g_debugDroidCount;

extern Uint32 sys_getCurrentTime ();