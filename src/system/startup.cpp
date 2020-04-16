#include "../../hdr/system/startup.h"
#include "../../hdr/system/scriptEngine.h"
#include "../../hdr/system/scriptConfig.h"
#include "../../hdr/io/fileSystem.h"

#define APP_NAME    "Para++"

//Screen dimension constants
const int SCREEN_WIDTH  = 800;
const int SCREEN_HEIGHT = 600;

// The window we'll be rendering to
PARA_Window *window = nullptr;

// The surface contained by the window
PARA_Surface *screenSurface = nullptr;

//
PARA_Renderer    *renderer;

//----------------------------------------------------------------------------------------------------------------------
//
// Return the current renderer pointer
PARA_Renderer *sys_getRenderer()
//----------------------------------------------------------------------------------------------------------------------
{
	return renderer;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the suface contained by the current window
PARA_Surface *sys_getScreenSurface()
//----------------------------------------------------------------------------------------------------------------------
{
	//Get window surface
	return SDL_GetWindowSurface(window);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the current window handle
PARA_Window *sys_getWindow()
//----------------------------------------------------------------------------------------------------------------------
{
	return window;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Start the Windowing system
void sys_createScreen()
//----------------------------------------------------------------------------------------------------------------------
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		sys_shutdownWithError(sys_getString("SDL could not initialize. [ %s ]", SDL_GetError()));
	else
	{
		window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
		                          SDL_WINDOW_SHOWN);
		if (window == nullptr)
			sys_shutdownWithError(sys_getString("Window could not be created. [ %s ]", SDL_GetError()));
		else
		{
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (nullptr == renderer)
			{
				SDL_DestroyWindow(window);
				sys_shutdownWithError(sys_getString("Renderer could not be created. [ %s ]", SDL_GetError()));
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Start systems - Logfile, Script, Filesystem ( Physfs ), Event queues
void sys_startSystems()
//----------------------------------------------------------------------------------------------------------------------
{
	if (!logFile.open("paraLogFile.txt"))
		sys_shutdownWithError("Error: Could not start paraLogFile. Check write permissions on folder.");
	logFile.write("Logfile started.");
	console.add(sys_getString("Console started [ %s ]", APP_NAME));

	sys_createScreen();
	console.add("Window system started.");

	if (!fileSystem.init(logFile, "data", "data"))
		sys_shutdownWithError("Error. Could not start filesystem. Check directory structure.");

	console.add("Filesystem started.");

	fileSystem.addPath(logFile, "data/data");
	fileSystem.addPath(logFile, "data/scripts");

	logFile.write("About to load font.");

	fontOne.load(logFile, 16, "data/Modenine.ttf");
	fontOne.setColor(255, 255, 255, 255);

#ifdef MY_DEBUG//=true
	logFile.write("Running DEBUG version.");
#endif

	if (!paraScriptInstance.init(logFile, reinterpret_cast<asSFuncPtr &>(scr_Output)))
		sys_shutdownWithError("Error: Could not start Scripting engine.");

	console.add("Scripting started.");
	sys_scriptInitScriptFunctions();
	sys_scriptInitFunctions();
	sys_scriptInitVariables();
	io_getScriptFileNames("scripts");
	paraScriptInstance.loadAndCompile(logFile);
	paraScriptInstance.cacheFunctions(logFile);


}
