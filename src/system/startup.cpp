#include <cassert>
#include "../../hdr/system/startup.h"
#include "../../hdr/system/scriptEngine.h"
#include "../../hdr/system/scriptConfig.h"
#include "../../hdr/io/fileSystem.h"
#include "../../hdr/io/configFile.h"
#include "../../hdr/io/logFile.h"
#include "../../hdr/io/console.h"

// The window we'll be rendering to
PARA_Window *window = nullptr;

// Renderer associated with the window
PARA_Renderer *renderer = nullptr;

// Target render texture
PARA_Texture    *renderTargetTexture = nullptr;

int logicalWinWidth;
int logicalWinHeight;
int windowWidth;
int windowHeight;
int windowFullscreen        = false;
int windowFullscreenDesktop = false;
int windowBorderless        = false;
int windowInputGrabbed      = true;
int windowInputFocus        = true;
int windowAllowHighDPI      = false;
int whichRenderer           = 0;
int presentVSync            = true;
int renderScaleQuality      = 0;

bool renderToTextureAvailable = false;

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
// Return if render to texture is in use
bool sys_useRenderTarget()
//----------------------------------------------------------------------------------------------------------------------
{
	return renderToTextureAvailable;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the render target texture handle
PARA_Texture *sys_getRenderTarget()
//----------------------------------------------------------------------------------------------------------------------
{
//	SDL_assert_release(renderTargetTexture != nullptr);
	return renderTargetTexture;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the render target texture
void sys_createRenderTargetTexture(int targetWidth, int targetHeight)
//----------------------------------------------------------------------------------------------------------------------
{
	std::string hintValue;

//	0 or nearest 	nearest pixel sampling
//	1 or linear 	linear filtering (supported by OpenGL and Direct3D)
//	2 or best	anisotropic filtering (supported by Direct3D)

	if (nullptr != renderTargetTexture)
	{
		SDL_DestroyTexture(renderTargetTexture);
		renderToTextureAvailable = false;
	}
	//
	// Influence how the scaling is done when rendering the target texture to screen
	hintValue = std::to_string(renderScaleQuality);
	if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, hintValue.c_str()))
		con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, "Hint SDL_HINT_RENDER_SCALE_QUALITY applied.");
	else
		con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, "Hint SDL_HINT_RENDER_SCALE_QUALITY not applied.");

	renderTargetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, targetWidth, targetHeight);
	if (nullptr == renderTargetTexture)
	{
		renderToTextureAvailable = false;
		con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString("Unable to create render target texture [ %s ]", SDL_GetError()));
	}

	renderToTextureAvailable = true;
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

		con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString("%i. Renderer name [ %s ]", i, renderDriverInfo.name));

		if (renderDriverInfo.flags & SDL_RENDERER_SOFTWARE)
			con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString("          %i. Software fallback", i));
		if (renderDriverInfo.flags & SDL_RENDERER_ACCELERATED)
			con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString("          %i. Uses hardware acceleration", i));
		if (renderDriverInfo.flags & SDL_RENDERER_PRESENTVSYNC)
			con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString("          %i. Uses screen refresh rate to sync", i));
		if (renderDriverInfo.flags & SDL_RENDERER_TARGETTEXTURE)
			con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString("          %i. Supports render to texture", i));
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the flags to be used with the renderer
Uint32 sys_createRendererFlags(int rendererIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	Uint32 newFlags = 0;

	if ((rendererIndex < 0) || (rendererIndex > (int) (rendererInfo.size() - 1)))
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
void sys_createScreen(
		bool restart, int newWinWidth, int newWinHeight, int winFlags, int rendererIndex, int rendererFlags, int newLogicalWidth,
		int newLogicalHeight)
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

	window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, newWinWidth, newWinHeight, winFlags);
	if (nullptr == window)
		sys_shutdownWithError(sys_getString("Window could not be created. [ %s ]", SDL_GetError()));

	renderer = SDL_CreateRenderer(window, rendererIndex, rendererFlags);
	if (nullptr == renderer)
		sys_shutdownWithError(sys_getString("Renderer could not be created. [ %s ]", SDL_GetError()));

	windowWidth  = newWinWidth;
	windowHeight = newWinHeight;

	logicalWinHeight = newLogicalWidth;
	logicalWinHeight = newLogicalHeight;

	SDL_RenderSetLogicalSize(renderer, newLogicalWidth, newLogicalHeight);
	//
	// See if render to texture is supported - if so, create a render target texture and set available flag to true
	renderToTextureAvailable = SDL_RenderTargetSupported(renderer);
	if (renderToTextureAvailable)
		sys_createRenderTargetTexture(newLogicalWidth, newLogicalHeight);
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
	if (!SDL_WasInit(SDL_INIT_EVERYTHING))
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
			sys_shutdownWithError(sys_getString("SDL could not initialize. [ %s ]", SDL_GetError()));
	}

	io_initLogFile();

	con_initConsole();

	io_readConfigValues("data/config.ini");

	sys_getRendererInfo();  // Store the renderers available to the system

	sys_verifyRenderer();

	sys_createScreen(false, windowWidth, windowHeight, sys_createWindowFlags(), whichRenderer, sys_createRendererFlags(whichRenderer), logicalWinWidth,
	                 logicalWinHeight);
	con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString("Window system started. Renderer [ %s ]", rendererInfo[whichRenderer].name));

	if (!fileSystem.init("data", "data"))
		sys_shutdownWithError("Error. Could not start filesystem. Check directory structure.");

	log_addEvent("Filesystem started.");

	fileSystem.addPath("data/data");
	fileSystem.addPath("data/scripts");

	log_addEvent("About to load font.");

	consoleFont.load(12, "data/console.ttf");
	consoleFont.setColor(255, 255, 255, 255);

#ifdef MY_DEBUG//=true
	log_addEvent("Running DEBUG version.");
#endif

	if (!paraScriptInstance.init(reinterpret_cast<asSFuncPtr &>(scr_Output)))
		sys_shutdownWithError("Error: Could not start Scripting engine.");

	sys_addEvent(EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, ("Scripting started."));
	sys_scriptInitScriptFunctions();
	sys_scriptInitFunctions();
	sys_scriptInitVariables();
	io_getScriptFileNames("scripts");
	paraScriptInstance.loadAndCompile();
	paraScriptInstance.cacheFunctions();


}

//----------------------------------------------------------------------------------------------------------------------
//
// Destroy the current window and renderer and create a new one
void sys_createNewScreen(int winWidth, int winHeight, int newRenderer)
//----------------------------------------------------------------------------------------------------------------------
{
	sys_createScreen(true, winWidth, winHeight, sys_createWindowFlags(), newRenderer, sys_createRendererFlags(newRenderer), winWidth, winHeight);
	con_addEvent(0, sys_getString("Window system started. Renderer [ %s ]", rendererInfo[newRenderer].name));
}