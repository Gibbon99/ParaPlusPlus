#include <system/physics.h>
#include <game/audio.h>
#include <io/fileWatch.h>
#include <io/keyboard.h>
#include <gui/guiLanguage.h>
#include <io/joystick.h>
#include <game/database.h>
#include "../../hdr/system/startup.h"
#include "../../hdr/system/scriptEngine.h"
#include "../../hdr/system/scriptConfig.h"
#include "../../hdr/io/fileSystem.h"
#include "../../hdr/io/configFile.h"
#include "../../hdr/io/logFile.h"
#include "../../hdr/io/console.h"
#include "gui/guiSideview.h"

// Variables needed to start everything
int         logicalWinWidth;
int         logicalWinHeight;
int         windowWidth;
int         windowHeight;
int         hiresVirtualWidth;
int         hiresVirtualHeight;
int         consoleNumColumns;
int         consoleFontSize;
int         windowFullscreen        = false;
int         windowFullscreenDesktop = false;
bool        windowBorderless        = false;
int         windowInputGrabbed      = true;
int         windowInputFocus        = true;
bool        windowAllowHighDPI      = false;
int         whichRenderer           = 0;
bool        presentVSync            = true;
int         renderScaleQuality      = 0;
int         g_volumeLevel           = 0;
int         maxNumChannels          = 0;
int         guiFontSize             = 0;
std::string guiFontFileName;
std::string consoleFontFilename;
int         introFontSize;
std::string introFontFileName;
bool        enableSound;
bool        doScreenEffect;

//----------------------------------------------------------------------------------------------------------------------
//
// Create the flags to be used to create a window with
Uint32 sys_createWindowFlags ()
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
void sys_startSystems ()
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

	io_startFileWatcher ();

	con_initConsole ();
	//
	// Create window and renderer
	renderer.setShutdownFunction (sys_shutdownWithError);
	renderer.setConOutFunction (con_addEvent);
	renderer.create (windowWidth, windowHeight, sys_createWindowFlags (), whichRenderer, presentVSync, APP_NAME);
	//
	// Create target texture for rendering the console onto
	renderer.createRenderTargetTexture (HIRES_BACKING_TEXTURE, hiresVirtualWidth, hiresVirtualHeight, renderScaleQuality);
	renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
	//
	// Start the filesystem
	fileSystem.setOutputFunction (con_addEvent);
	fileSystem.setMallocFunction (sys_malloc);
	if (!fileSystem.init ("data", "data"))
		sys_shutdownWithError ("Error. Could not start filesystem. Check directory structure.");

//	fileSystem.addPath ("data/data");
	fileSystem.addPath ("data/scripts");
	fileSystem.addPath ("data/data.zip");
	//
	// Load and create the fontClass for the console
	fontClass.setOutputFunction (con_addEvent);
	fontClass.load (consoleFontSize, "consoleFont", consoleFontFilename);
	fontClass.use ("consoleFont");
	fontClass.setColor (255, 255, 255, 255);
	//
	// Start the scripting engine
	if (!paraScriptInstance.init (con_addEvent, reinterpret_cast<asFUNCTION_t>(scr_Output)))
		sys_shutdownWithError ("Error: Could not start Scripting engine.");

	sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, ("Scripting started."));
	sys_scriptInitScriptFunctions ();
	sys_scriptInitFunctions ();
	sys_scriptInitVariables ();
	io_getScriptFileNames ("scripts");
	paraScriptInstance.loadAndCompile ();
	paraScriptInstance.cacheFunctions ();

	evt_registerMutex (GAME_MUTEX_NAME);

	sys_setupPhysicsEngine ();

	gam_initAudio ();
	//
	// Textures are done from the same thread as window creation
	paraScriptInstance.run ("as_loadTextureResources", "");

	fontClass.load (guiFontSize, "guiFont", guiFontFileName);
	fontClass.use ("guiFont");

	fontClass.load (28, "guiFont28", guiFontFileName);
	fontClass.use ("guiFont28");

	fontClass.load (guiFontSize, "introFont", introFontFileName);
	fontClass.use ("introFont");

	gui.init (con_addEvent, reinterpret_cast<funcStrIn>(gui_getString), windowWidth, windowHeight, "keybinding.para");
	gui_loadSideViewData ("sideview.dat");
	paraScriptInstance.run ("as_createGUI", "");
//	audio.load("start1", "start1.wav");

	io_initJoystick ();

	fileSystem.getSearchPath ();

	gam_getDBInformation ();

	databaseSprite.create ("db_droid", 32, 0.6);

	SDL_SetTextureBlendMode (textures.at ("screen").getTexture (), SDL_BLENDMODE_MOD);

	//
	// Start in interactive console mode
	sys_setNewMode (MODE_GUI_MAINMENU, false);
}