#include <cassert>
#include "../../hdr/system/startup.h"
#include "../../hdr/system/scriptEngine.h"
#include "../../hdr/system/scriptConfig.h"
#include "../../hdr/io/fileSystem.h"
#include "../../hdr/io/configFile.h"
#include "../../hdr/io/logFile.h"
#include "../../hdr/io/console.h"
#include "../../hdr/system/enum.h"

// The window we'll be rendering to
PARA_Window *window = nullptr;

// Renderer associated with the window
PARA_Renderer *renderer = nullptr;

// Target render texture
PARA_Texture *renderTargetTexture   = nullptr;

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

bool renderToTextureAvailable = false;

struct __backingTexture
{
	PARA_Texture *backingTexture;
	int          backingWidth;
	int          backingHeight;
	int			logicalWidth;
	int			logicalHeight;
};

struct __rendererInfo
{
	SDL_RendererInfo rendererInfo;
	std::string      rendererName = "";
	bool             softwareFallback = false;
	bool             hardwareAccelerated = false;
	bool             supportsRenderToTexture = false;
	bool             supportsVSync = false;
};

std::string                             activeBackingTexture;
std::map<std::string, __backingTexture> backingTextures;
std::vector<__rendererInfo>             rendererInfo;

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
// Return the string name of the currently active backing texture
std::string sys_getCurrentBackingTexture()
//----------------------------------------------------------------------------------------------------------------------
{
	return activeBackingTexture;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Display the current backing texture information to the console
void sys_debugBackingTextures()
//----------------------------------------------------------------------------------------------------------------------
{
	for (const auto &textureItr : backingTextures)
	{
		con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString("[ %s ] - Logical [ %i x %i ]", textureItr.first.c_str(), textureItr.second.logicalWidth, textureItr.second.logicalHeight));
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Make a new backing texture active
void sys_setCurrentBackingTexture(const std::string &newActiveTexture)
//----------------------------------------------------------------------------------------------------------------------
{
	for (const auto &textureItr : backingTextures)
	{
		if (newActiveTexture == textureItr.first)
		{
			activeBackingTexture = newActiveTexture;
			return;
		}
	}
	sys_shutdownWithError(sys_getString("Unable to set new backing texture. Not found [ %s ]", newActiveTexture.c_str()));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the render target texture handle
PARA_Texture *sys_getRenderTarget(const std::string &textureName)
//----------------------------------------------------------------------------------------------------------------------
{
//	SDL_assert_release(renderTargetTexture != nullptr);
	if (backingTextures.empty())
		sys_shutdownWithError(sys_getString("No backing textures exist."));

	for (const auto &textureItr : backingTextures)
	{
		if (textureName == textureItr.first)
		{
			return textureItr.second.backingTexture;
		}
	}
	sys_shutdownWithError(sys_getString("Backing texture [ %s ] does not exist.", textureName.c_str()));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the render target texture
void sys_createRenderTargetTexture(const std::string &textureName, int logicalWidth, int logicalHeight)
//----------------------------------------------------------------------------------------------------------------------
{
	std::string      hintValue;
	__backingTexture tempBackingTexture;

//	0 or nearest 	nearest pixel sampling
//	1 or linear 	linear filtering (supported by OpenGL and Direct3D)
//	2 or best	anisotropic filtering (supported by Direct3D)

	//
	// See if it exists already - if so, remove it, assuming it is being resized.
	//
	for (auto &textureItr : backingTextures)
	{
		if (textureItr.first == textureName)
		{
			con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE,
			             sys_getString("Backing texture [ %s ] already exists. Removing.", textureName.c_str()));
			SDL_DestroyTexture(textureItr.second.backingTexture);
			backingTextures.erase(textureName);
		}
	}

	tempBackingTexture.logicalHeight		 = logicalHeight;
	tempBackingTexture.logicalWidth			 = logicalWidth;
//	tempBackingTexture.backingHeight         = targetHeight;
//	tempBackingTexture.backingWidth          = targetWidth;
	tempBackingTexture.backingTexture = nullptr;
	//
	// Influence how the scaling is done when rendering the target texture to screen
	hintValue = std::to_string(renderScaleQuality);
	if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, hintValue.c_str()))
		con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, "Hint SDL_HINT_RENDER_SCALE_QUALITY applied.");
	else
		con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, "Hint SDL_HINT_RENDER_SCALE_QUALITY not applied.");

	tempBackingTexture.backingTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, logicalWidth,
	                                                      logicalHeight);
	if (nullptr == tempBackingTexture.backingTexture)
	{
//		renderToTextureAvailable = false;
		con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString("Unable to create render target texture [ %s ]", SDL_GetError()));
	}

//	renderToTextureAvailable = true;

	backingTextures.insert(std::pair<std::string, __backingTexture>(textureName, tempBackingTexture));

	//
	// Adjust virtual scaling
	SDL_RenderSetLogicalSize(sys_getRenderer(), logicalWidth, logicalHeight);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get information on the current renderer
void sys_debugGetCurrentRenderer()
//----------------------------------------------------------------------------------------------------------------------
{
	int rendererCount = 0;

	if (rendererInfo.empty())
	{
		con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, "Renderer information has not been populated.");
		return;
	}

	if (whichRenderer < (int)(rendererInfo.size()))
	{
		con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE,
		             sys_getString("Renderer name [ %s ]", rendererInfo[whichRenderer].rendererName.c_str()));
		con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString(" - Hardware accelerated [ %s ]", rendererInfo[whichRenderer].hardwareAccelerated ? "Yes" : "No"));
		con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString(" - Render to texture [ %s ]", rendererInfo[whichRenderer].supportsRenderToTexture ? "Yes" : "No"));
		con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString(" - Render to VSync [ %s ]", rendererInfo[whichRenderer].supportsVSync ? "Yes" : "No"));
		con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString(" - Software fallback [ %s ]", rendererInfo[whichRenderer].softwareFallback ? "Yes" : "No"));
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the information about the available renderers
void sys_getRendererInfo()
//----------------------------------------------------------------------------------------------------------------------
{
	int            numRenderDrivers;
	__rendererInfo tempRendererInfo;

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
		tempRendererInfo.rendererInfo            = renderDriverInfo;
		tempRendererInfo.rendererName            = renderDriverInfo.name;
		tempRendererInfo.softwareFallback        = (renderDriverInfo.flags & SDL_RENDERER_SOFTWARE) ? true : false;
		tempRendererInfo.hardwareAccelerated     = (renderDriverInfo.flags & SDL_RENDERER_ACCELERATED) ? true : false;
		tempRendererInfo.supportsVSync           = (renderDriverInfo.flags & SDL_RENDERER_PRESENTVSYNC) ? true : false;
		tempRendererInfo.supportsRenderToTexture = (renderDriverInfo.flags & SDL_RENDERER_TARGETTEXTURE) ? true : false;

		rendererInfo.push_back(tempRendererInfo);
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

	if (rendererInfo[rendererIndex].rendererInfo.flags & SDL_RENDERER_SOFTWARE)
	{
		newFlags = newFlags | SDL_RENDERER_SOFTWARE;
		return newFlags;
	}

	if (rendererInfo[rendererIndex].rendererInfo.flags & SDL_RENDERER_ACCELERATED)
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

	window = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, newWinWidth, newWinHeight, winFlags);
	if (nullptr == window)
		sys_shutdownWithError(sys_getString("Window could not be created. [ %s ]", SDL_GetError()));

	renderer = SDL_CreateRenderer(window, rendererIndex, rendererFlags);
	if (nullptr == renderer)
		sys_shutdownWithError(sys_getString("Renderer could not be created. [ %s ]", SDL_GetError()));

	windowWidth  = newWinWidth;
	windowHeight = newWinHeight;

//	logicalWinHeight = newLogicalWidth;
//	logicalWinHeight = newLogicalHeight;

//	SDL_RenderSetLogicalSize(renderer, newLogicalWidth, newLogicalHeight);
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

	io_readConfigValues("data/config.ini");

	con_initConsole();

	sys_getRendererInfo();  // Store the renderers available to the system

	sys_verifyRenderer();

	sys_createScreen(false, windowWidth, windowHeight, sys_createWindowFlags(), whichRenderer, sys_createRendererFlags(whichRenderer));
	con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE,
	             sys_getString("Window system started. Renderer [ %s ]", rendererInfo[whichRenderer].rendererName.c_str()));

	con_initConsoleBackingTexture();

	fileSystem.setOutputFunction(reinterpret_cast<functionPtrStr>(log_addEvent));
	fileSystem.setMallocFunction(reinterpret_cast<functionPtrMalloc>(sys_malloc));
	if (!fileSystem.init("data", "data"))
		sys_shutdownWithError("Error. Could not start filesystem. Check directory structure.");

	log_addEvent("Filesystem started.");

	fileSystem.addPath("data/data");
	fileSystem.addPath("data/scripts");

	consoleFont.setOutputFunction(reinterpret_cast<functionPtrStr>(log_addEvent));
	consoleFont.load(consoleFontSize, consoleFontFilename);
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

	sys_setNewMode(MODE_CONSOLE_EDIT);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Destroy the current window and renderer and create a new one
void sys_createNewScreen(int winWidth, int winHeight, int newRenderer)
//----------------------------------------------------------------------------------------------------------------------
{
	sys_createScreen(true, winWidth, winHeight, sys_createWindowFlags(), newRenderer, sys_createRendererFlags(newRenderer));
	con_addEvent(0, sys_getString("Window system started. Renderer [ %s ]", rendererInfo[newRenderer].rendererName.c_str()));
}