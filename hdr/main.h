#pragma once

#include "wrapper.h"
#include <iostream>
#include <string>
#include <vector>

#ifdef WIN32
  #include "SDL.h"
#else

  #include "SDL2/SDL.h"

#endif

#define APP_NAME    "Para++"

#include "classes/paraScript.h"
#include "classes/paraLogFile.h"
#include "classes/paraFileSystem.h"
#include "classes/paraFont.h"
#include "classes/paraConsole.h"
#include "system/eventsEngine.h"
#include "system/shutdown.h"
#include "system/util.h"
#include "physfs/physfs.h"
#include "classes/paraRenderer.h"

#define ERROR_COUNT_LIMIT   10

#define CONSOLE_BACKING_TEXTURE "consoleBackingTexture"

using namespace std;

typedef int (*functionPtr)(...);

extern paraScript     paraScriptInstance;
extern paraLogFile    logFile;
extern paraFileSystem fileSystem;
extern paraFont       consoleFont;
extern paraConsole    console;
extern paraRenderer   renderer;

extern bool   quitLoop;
extern Uint32 fpsPrint;
extern Uint32 thinkFPSPrint;
extern double percentIntoNextFrame;

extern std::string testVarString;
extern float       testVarFloat;