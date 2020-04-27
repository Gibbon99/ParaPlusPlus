
#include "../../hdr/system/startup.h"
#include "../../hdr/system/scriptEngine.h"
#include "../../hdr/system/scriptConfig.h"
#include "../../hdr/io/fileSystem.h"
#include "../../hdr/io/configFile.h"
#include "../../hdr/io/logFile.h"
#include "../../hdr/io/console.h"

// Variables needed to start everything
int         logicalWinWidth;
int         logicalWinHeight;
int         windowWidth;
int         windowHeight;
int			consoleVirtualWidth;
int			consoleVirtualHeight;
int         consoleNumColumns;
int         consoleFontSize;
int         windowFullscreen        = false;
int         windowFullscreenDesktop = false;
int         windowBorderless        = false;
int         windowInputGrabbed      = true;
int         windowInputFocus        = true;
int         windowAllowHighDPI      = false;
int         whichRenderer           = 0;
int         presentVSync            = true;
int         renderScaleQuality      = 0;
std::string consoleFontFilename;

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
// Start systems - Logfile, Script, Filesystem ( Physfs ), Event queues
void sys_startSystems()
//----------------------------------------------------------------------------------------------------------------------
{
	if (!SDL_WasInit (SDL_INIT_EVERYTHING))
	{
		if (SDL_Init (SDL_INIT_EVERYTHING) < 0)
			sys_shutdownWithError (sys_getString ("SDL could not initialize. [ %s ]", SDL_GetError ()));
	}

	io_initLogFile ();
#ifdef MY_DEBUG
	log_addEvent ("Running DEBUG version.");
#endif
	io_readConfigValues ("data/config.ini");

	con_initConsole ();
	//
	// Create window and renderer
	renderer.setShutdownFunction (reinterpret_cast<functionPtrStr>(sys_shutdownWithError));
	renderer.setConOutFunction (reinterpret_cast<functionPtrOut>(con_addEvent));
	renderer.create (windowWidth, windowHeight, sys_createWindowFlags (), whichRenderer, presentVSync, APP_NAME);
	//
	// Create target texture for rendering the console onto
	renderer.createRenderTargetTexture (CONSOLE_BACKING_TEXTURE, consoleVirtualWidth, consoleVirtualHeight, renderScaleQuality);
	renderer.setCurrentBackingTexture (CONSOLE_BACKING_TEXTURE);
	//
	// Start the filesystem
	fileSystem.setOutputFunction (reinterpret_cast<functionPtrStr>(log_addEvent));
	fileSystem.setMallocFunction (reinterpret_cast<functionPtrMalloc>(sys_malloc));
	if (!fileSystem.init ("data", "data"))
		sys_shutdownWithError ("Error. Could not start filesystem. Check directory structure.");

//	fileSystem.addPath ("data/data");
	fileSystem.addPath ("data/scripts");
	//
	// Load and create the font for the console
	consoleFont.setOutputFunction (reinterpret_cast<functionPtrStr>(log_addEvent));
	consoleFont.load (consoleFontSize, consoleFontFilename);
	consoleFont.setColor (255, 255, 255, 255);
	//
	// Start the scripting engine
	if (!paraScriptInstance.init (reinterpret_cast<asFUNCTION_t>(scr_Output)))
		sys_shutdownWithError ("Error: Could not start Scripting engine.");


	sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, ("Scripting started."));
	sys_scriptInitScriptFunctions ();
	sys_scriptInitFunctions ();
	sys_scriptInitVariables ();
	io_getScriptFileNames ("scripts");
	paraScriptInstance.loadAndCompile ();
	paraScriptInstance.cacheFunctions ();

	evt_registerMutex (GAME_MUTEX_NAME);
	//
	// Start in interactive console mode
	sys_setNewMode (MODE_CONSOLE_EDIT);
}