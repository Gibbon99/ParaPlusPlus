#include <system/cpPhysics.h>
#include <game/audio.h>
#include <io/fileWatch.h>
#include <gui/guiLanguage.h>
#include <io/joystick.h>
#include <game/database.h>
#include <game/tiles.h>
#include <system/util.h>
#include <game/player.h>
#include <classes/paraLightmap.h>
#include <system/scriptEngine.h>
#include <system/scriptConfig.h>
#include <io/fileSystem.h>
#include <io/configFile.h>
#include <io/logFile.h>
#include <gui/guiSideview.h>

// Variables needed to start everything
int         gameWinWidth;
int         gameWinHeight;
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

	io_startFileWatcher ();

	con_initConsole ();
	//
	// Create window and renderer
	renderer.setShutdownFunction (sys_shutdownWithError);
	renderer.setConOutFunction (con_addEvent);
	renderer.create (windowWidth, windowHeight, sys_createWindowFlags (), whichRenderer, presentVSync, APP_NAME);
	//
	// Create target texture for rendering the non game screens onto
	renderer.createRenderTargetTexture (HIRES_BACKING_TEXTURE, hiresVirtualWidth, hiresVirtualHeight, renderScaleQuality);

	renderer.createRenderTargetTexture (GAME_BACKING_TEXTURE, gameWinWidth, gameWinHeight, renderScaleQuality);

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

	evt_registerMutex (GAME_MUTEX_NAME);

	setupBulletLightmapColors ();

	sys_setNewMode (MODE_CONSOLE_INIT, false);

	SDL_Thread *initThread;

	initThread = SDL_CreateThread (sys_startInit, "InitThread", (void *) nullptr);
	if (nullptr == initThread)
		sys_shutdownWithError (sys_getString ("Unable to start init thread - [ %s ]", SDL_GetError ()));

	SDL_Delay (100);

	SDL_DetachThread (initThread);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Start to load and prepare everything else
int sys_startInit([[maybe_unused]]void *ptr)
//----------------------------------------------------------------------------------------------------------------------
{
//
// Start the scripting engine
	if (!paraScriptInstance.init (con_addEvent, reinterpret_cast<asFUNCTION_t>(scr_Output)))
		sys_shutdownWithError ("Error: Could not start Scripting engine.");

	sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, ("Scripting started."));
	sys_scriptInitScriptFunctions ();
	sys_scriptInitFunctions ();
	sys_scriptInitVariables ();
	io_getScriptFileNames ("scripts");
	if (!paraScriptInstance.loadAndCompile ())
	{
		sys_shutdownWithError ("Error: Could not compile scripts.");
	}
	paraScriptInstance.cacheFunctions ();

#if MY_DEBUG
	logFile.write (sys_getString ("[ %s ] Run setupPhysicsEngine.", __func__));
#endif

//	sys_setupPhysicsEngine ();

	sys_createWorldPhysics ();

	gam_initAudio ();

	gam_setTileType (false);
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, tileFilename + "|tiles| ");
//
// Textures are done from the same thread as window creation - put on the Game Event queue
	paraScriptInstance.run ("as_loadTextureResources", "");
	paraScriptInstance.run ("as_loadAllDecks", "");
//
// Load fonts via Game Event thread
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_FONT, 0, guiFontFileName + "|guiFont|" + to_string (guiFontSize));
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_FONT, 0, guiFontFileName + "|guiFont28|" + to_string (28));
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_FONT, 0, introFontFileName + "|introFont|" + to_string (introFontSize));

	SDL_Delay (100);

	gui.init (con_addEvent, reinterpret_cast<funcStrIn>(gui_getString), windowWidth, windowHeight, gameWinWidth, gameWinHeight, "keybinding.para");
	gui_loadSideViewData ("sideview.dat");
	paraScriptInstance.run ("as_createGUI", "");

	io_initJoystick ();

#ifdef MY_DEBUG
	fileSystem.getSearchPath ();
#endif

	gam_getDBInformation ();

	databaseSprite.create ("db_droid", 32, 0.6);

// TODO do as an event with delay
// SDL_SetTextureBlendMode (textures.at ("screen").getTexture (), SDL_BLENDMODE_MOD);

	gam_setupPlayerDroid ();

	createLookupTable ();

	//
	// Setup the starfield - guess the HUD height if the texture isn't loaded yet
	try
	{
		sideviewStarfield.init (renderer, 40, 7, textures.at ("hudNew").getHeight (), windowHeight, windowWidth);
		deckviewStarfield.init (renderer, 40, 7, textures.at ("hudNew").getHeight (), windowHeight, windowWidth);
	}
	catch (std::out_of_range &outOfRange)
	{
		sideviewStarfield.init (renderer, 40, 7, 55, windowHeight, windowWidth);
		deckviewStarfield.init (renderer, 40, 7, 55, windowHeight, windowWidth);
	}

	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_CHANGE_MODE, 100, to_string (MODE_GUI_MAINMENU) + "|" + to_string (true));
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_CHANGE_MODE, 0, to_string (MODE_SHOW_SPLASH) + "|" + to_string (true));

	return 0;
}