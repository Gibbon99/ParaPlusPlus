#pragma once

#include "wrapper.h"
#include <iostream>
#include <string>
#include <vector>
#include <classes/paraAudio.h>
#include <classes/paraTexture.h>
#include <classes/paraGui.h>
#include <classes/paraSprite.h>

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
#include "system/util.h"
#include "physfs/physfs.h"
#include "classes/paraRenderer.h"

#define ERROR_COUNT_LIMIT   10

#define HIRES_BACKING_TEXTURE "consoleBackingTexture"

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

extern paraSprite   databaseSprite;

extern std::map<std::string, paraTexture> textures;

extern bool   quitLoop;
extern Uint32 fpsPrint;
extern Uint32 thinkFPSPrint;
extern double percentIntoNextFrame;
extern int    currentMode;

//
// Game levers
//
extern double baseGameSpeed;

extern std::string testVarString;
extern float       testVarFloat;