#include "../../hdr/system/startup.h"
#include "../../hdr/system/scriptEngine.h"
#include "../../hdr/system/scriptConfig.h"
#include "../../hdr/io/fileSystem.h"
#include "../../hdr/classes/configFile.h"

#define APP_NAME    "Para++"

// The window we'll be rendering to
PARA_Window *window = nullptr;

// Renderer associated with the window
PARA_Renderer *renderer;

int windowFullscreen        = false;
int windowFullscreenDesktop = false;
int windowBorderless        = false;
int windowInputGrabbed      = true;
int windowInputFocus        = true;
int windowAllowHighDPI      = false;
int  whichRenderer           = 0;
int presentVSync            = true;

std::vector<SDL_RendererInfo> rendererInfo;

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
// Return the current window handle
PARA_Window *sys_getWindow()
//----------------------------------------------------------------------------------------------------------------------
{
	return window;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the information about the available renderers
void sys_getRendererInfo()
//----------------------------------------------------------------------------------------------------------------------
{
	int              numRenderDrivers;
	SDL_RendererInfo renderDriverInfo;

	numRenderDrivers = SDL_GetNumRenderDrivers();
	if (numRenderDrivers <= 0)
		sys_shutdownWithError("Error. No renderer drivers available.");

	for (int i = 0; i != numRenderDrivers; i++)
	{
		if (SDL_GetRenderDriverInfo(i, &renderDriverInfo) < 0)
		{
			sys_shutdownWithError(sys_getString("Error. Unable to get render driver info [ %s ]", SDL_GetError()));
		}
		rendererInfo.push_back(renderDriverInfo);

		console.add(sys_getString("%i. Renderer name [ %s ]", i, renderDriverInfo.name));

		if (renderDriverInfo.flags & SDL_RENDERER_SOFTWARE)
			console.add(sys_getString("          %i. Software fallback", i));
		if (renderDriverInfo.flags & SDL_RENDERER_ACCELERATED)
			console.add(sys_getString("          %i. Uses hardware acceleration", i));
		if (renderDriverInfo.flags & SDL_RENDERER_PRESENTVSYNC)
			console.add(sys_getString("          %i. Uses screen refresh rate to sync", i));
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the flags to be used with the renderer
Uint32 sys_createRendererFlags(int rendererIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	Uint32 newFlags = 0;

	if ((rendererIndex < 0) || (rendererIndex > (int)(rendererInfo.size() - 1)))
		sys_shutdownWithError("Error. Using invalid renderindex to create renderer flags.");

	if (presentVSync)
		newFlags = SDL_RENDERER_PRESENTVSYNC;

	if (rendererInfo[rendererIndex].flags & SDL_RENDERER_SOFTWARE)
	{
		newFlags = newFlags | SDL_RENDERER_SOFTWARE;
		return newFlags;
	}

	if (rendererInfo[rendererIndex].flags & SDL_RENDERER_ACCELERATED)
	{
		newFlags = newFlags | SDL_RENDERER_ACCELERATED;
		return newFlags;
	}

	return newFlags;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the flags to be used to create a window with
Uint32 sys_createWindowFlags()
//----------------------------------------------------------------------------------------------------------------------
{
	Uint32 newFlags = 0;

	if (windowFullscreen)
		newFlags = newFlags | SDL_WINDOW_FULLSCREEN;

	if (windowFullscreen && windowFullscreenDesktop)
		newFlags = newFlags | SDL_WINDOW_FULLSCREEN_DESKTOP;

	if (windowBorderless)
		newFlags = newFlags | SDL_WINDOW_BORDERLESS;

	if (windowInputGrabbed)
		newFlags = newFlags | SDL_WINDOW_INPUT_GRABBED;

	if (windowInputFocus)
		newFlags = newFlags | SDL_WINDOW_INPUT_FOCUS;

	if (windowAllowHighDPI)
		newFlags = newFlags | SDL_WINDOW_ALLOW_HIGHDPI;

	return newFlags;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Start the Windowing system
void sys_createScreen(bool restart, int newWinWidth, int newWinHeight, int winFlags, int rendererIndex, int rendererFlags)
//----------------------------------------------------------------------------------------------------------------------
{
	//
	// Maybe need a mutex or a condition here - although no other thread should be rendering apart from main thread
	//
	if (restart)
	{
		SDL_DestroyRenderer(sys_getRenderer());
		SDL_DestroyWindow(sys_getWindow());
	}

	if (!SDL_WasInit(SDL_INIT_EVERYTHING))
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
			sys_shutdownWithError(sys_getString("SDL could not initialize. [ %s ]", SDL_GetError()));
	}

	window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, newWinWidth, newWinHeight, winFlags);
	if (nullptr == window)
		sys_shutdownWithError(sys_getString("Window could not be created. [ %s ]", SDL_GetError()));

	renderer = SDL_CreateRenderer(window, rendererIndex, rendererFlags);
	if (nullptr == renderer)
		sys_shutdownWithError(sys_getString("Renderer could not be created. [ %s ]", SDL_GetError()));

	windowWidth = newWinWidth;
	windowHeight = newWinHeight;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Verify that the renderer pulled from the config file is valid and can be used
void sys_verifyRenderer()
//----------------------------------------------------------------------------------------------------------------------
{
	if ((whichRenderer < 0) || (whichRenderer > (int) (rendererInfo.size() - 1)))
	{
		logFile.write("Attempted to use an invalid renderer. Resetting to default.");
		whichRenderer = 0;
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

	sys_getRendererInfo();  // Store the renderers available to the system

	sys_verifyRenderer();

	sys_createScreen(false, windowWidth, windowHeight, sys_createWindowFlags(), whichRenderer, sys_createRendererFlags(whichRenderer));
	console.add(sys_getString("Window system started. Renderer [ %s ]", rendererInfo[whichRenderer].name));

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

//----------------------------------------------------------------------------------------------------------------------
//
// Destroy the current window and renderer and create a new one
void sys_createNewScreen(int winWidth, int winHeight, int newRenderer)
//----------------------------------------------------------------------------------------------------------------------
{
	sys_createScreen(true, winWidth, winHeight, sys_createWindowFlags(), newRenderer, sys_createRendererFlags(newRenderer));
	console.add(sys_getString("Window system started. Renderer [ %s ]", rendererInfo[newRenderer].name));
}