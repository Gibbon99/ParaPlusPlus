﻿#pragma once

#define _CRT_NO_VA_START_VALIDATION

#include "wrapper.h"
#include <iostream>
#include <string>
#include <vector>
#include <classes/paraAudio.h>
#include <classes/paraTexture.h>
#include <classes/paraGui.h>
#include <classes/paraSprite.h>
#include <classes/paraStarfield.h>
#include <classes/paraHighScore.h>

#ifdef __WIN32__
#include <SDL2/SDL.h>
#else

#include "SDL2/SDL.h"

#endif

#define APP_NAME    "ParadroidPlusPlus"

#define DEBUG_AI2 1     // Toggle logging for paraAI2
// #define DEBUG_SCRIPT 1
// #define DEBUG_GUI_SETUP 1
// #define MY_DEBUG 1
// #define DEBUG_CONSOLE 1
// #define DEBUG_AUDIO 1

#include "classes/paraScript.h"
#include "classes/paraLogFile.h"
#include "classes/paraFileSystem.h"
#include "classes/paraFont.h"
#include "classes/paraConsole.h"
#include "classes/paraAudio.h"
#include "system/eventsEngine.h"
#include "system/shutdown.h"
#include "physfs/physfs.h"
#include "classes/paraRenderer.h"
#include "chipmunk/chipmunk.h"

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

typedef int (*functionPtr)(...);

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
extern paraStarfield  deckviewStarfield;
extern paraSprite     databaseSprite;
extern paraHighScore  highScores;

extern std::map<std::string, paraTexture> textures;

extern bool   quitLoop;
extern Uint32 fpsPrint;
extern Uint32 thinkFPSPrint;
extern double percentIntoNextFrame;
extern int    currentMode;
extern double pixelsPerMeter;          // From script
extern bool   d_showPerfStats;
extern Uint8  difficultyValue;
//
// Game levers
//
extern double baseGameSpeed;

extern unsigned long g_debugDroidCount;

extern Uint32 sys_getCurrentTime();

struct userData_
{
	cpCollisionType userType;
	Uint32          bulletID;
	int             dataValue;
	bool            ignoreCollisionDroid;
	bool            ignoreCollisionPlayer;
};

//-----------------------------------------------------------------------------
//
// structure used for doors
//
//-----------------------------------------------------------------------------

struct doorTrigger_
{
	int    direction {};
	int    tileIndex {};
	int    currentFrame {};            // which frame are we on
	float  height {};
	float  width {};
	float  frameDelay {};                // speed to animate them at
	bool   inUse {};
	cpVect topLeft {};
	cpVect topRight {};
	cpVect botLeft {};
	cpVect botRight {};

	cpVect worldPosition  = {0, 0};
	cpVect renderPosition = {0, 0};

	cpBody                     *body {nullptr};
	cpShape                    *shape {nullptr};
	std::shared_ptr<userData_> userData {};
};
