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

#include "classes/paraScript.h"
#include "classes/paraLogFile.h"
#include "classes/paraFileSystem.h"
#include "classes/paraFont.h"
#include "classes/paraConsole.h"
#include "system/shutdown.h"
#include "system/util.h"
#include "physfs/physfs.h"

using namespace std;

typedef int (*functionPtr)(...);

extern paraScript     paraScriptInstance;
extern paraLogFile    logFile;
extern paraFileSystem fileSystem;
extern paraFont       fontOne;
extern paraConsole    console;