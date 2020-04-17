#include "../../hdr/system/startup.h"
#include "../../hdr/system/scriptEngine.h"
#include "../../hdr/system/scriptConfig.h"
#include "../../hdr/io/fileSystem.h"
#include "../../hdr/classes/configFile.h"

#define APP_NAME    "Para++"

// The window we'll be rendering to
PARA_Window *window = nullptr;

// Renderer associated with the window
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
	if (nullptr == window)
		sys_shutdownWithError("Attempting to use invalid window.");

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
		if (SDL_CreateWindowAndRenderer(windowWidth, windowHeight, SDL_WINDOW_RESIZABLE, &window, &renderer))
			sys_shutdownWithError(sys_getString("Window could not be created. [ %s ]", SDL_GetError()));
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

	io_readConfigValues("data/config.ini");

	sys_createScreen();
	console.add("Window system started.");

	sys_getRendererInfo();

	if (!fileSystem.init(logFile, "data", "data"))
		sys_shutdownWithError("Error. Could not start filesystem. Check directory structure.");

	console.add("Filesystem started.");

	fileSystem.addPath(logFile, "data/data");
	fileSystem.addPath(logFile, "data/scripts");

	logFile.write("About to load font.");

	consoleFont.load(logFile, 16, "data/console.ttf");
	consoleFont.setColor(255, 255, 255, 255);

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
