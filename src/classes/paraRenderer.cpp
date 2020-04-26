#include <vector>
#include <SDL_video.h>
#include <SDL_render.h>
#include <SDL_hints.h>
#include "../../hdr/classes/paraRenderer.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Constructor
paraRenderer::paraRenderer ()
//----------------------------------------------------------------------------------------------------------------------
{
	shutdownFunc = nullptr;
	windowHeight = 0;
	windowWidth  = 0;

	getRendererInfo ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set a function to call when a fatal error occurs and we shutdown
void paraRenderer::setShutdownFunction (functionPtrStr outputFunction)
//----------------------------------------------------------------------------------------------------------------------
{
	paraRenderer::shutdownFunc = outputFunction;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set a function to call when a fatal error occurs and we shutdown
void paraRenderer::setConOutFunction (functionPtrOut outputFunction)
//----------------------------------------------------------------------------------------------------------------------
{
	paraRenderer::consoleOutFunc = outputFunction;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Pass in string and parameters to format and return a string
// https://stackoverflow.com/questions/19009094/c-variable-arguments-with-stdstring-only
std::string paraRenderer::int_getString (std::string format, ...)
//----------------------------------------------------------------------------------------------------------------------
{
	const char *const zcFormat = format.c_str ();

	// initialize use of the variable argument array
	va_list vaArgs;
	va_start(vaArgs, format);

	// reliably acquire the size from a copy of the variable argument array
	// and a functionally reliable call to mock the formatting
	va_list vaCopy;
	va_copy(vaCopy, vaArgs);
	const int iLen = std::vsnprintf (nullptr, 0, zcFormat, vaCopy);
	va_end(vaCopy);

	// return a formatted string without risking memory mismanagement and without assuming any compiler
	// or platform specific behavior
	std::vector<char> zc (iLen + 1);
	std::vsnprintf (zc.data (), zc.size (), zcFormat, vaArgs);
	va_end(vaArgs);

	return std::string (zc.data ());
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the flags to be used with the renderer
Uint32 paraRenderer::createRendererFlags (int rendererIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	Uint32 newFlags = 0;

	if ((rendererIndex < 0) || (rendererIndex > (int) (rendererInfo.size () - 1)))
		shutdownFunc ("Error. Using invalid renderindex to create renderer flags.");

	if (int_useVSync)
		newFlags = SDL_RENDERER_PRESENTVSYNC;

	targetTextureAvailable = (rendererInfo[rendererIndex].rendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) != 0;

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
// Create a OS window and create the renderer for that window
void paraRenderer::create (int newWinWidth, int newWinHeight, int winFlags, int rendererIndex, bool useVSync, const std::string &windowTitle)
//----------------------------------------------------------------------------------------------------------------------
{
	int rendererFlags = 0;

	if (nullptr != window)
	{
		SDL_DestroyRenderer (renderer);
		SDL_DestroyWindow (window);
	}

	int_useVSync  = useVSync;
	whichRenderer = rendererIndex;
	verifyRenderer ();
	rendererFlags = createRendererFlags (whichRenderer);

	window = SDL_CreateWindow (windowTitle.c_str (), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, newWinWidth, newWinHeight, winFlags);
	if (nullptr == window)
		shutdownFunc (int_getString ("Window could not be created. [ %s ]", SDL_GetError ()));

	renderer = SDL_CreateRenderer (window, whichRenderer, rendererFlags);
	if (nullptr == renderer)
		shutdownFunc (int_getString ("Renderer could not be created. [ %s ]", SDL_GetError ()));

	windowWidth   = newWinWidth;
	windowHeight  = newWinHeight;
	cacheWinFlags = winFlags;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set a new renderer
void paraRenderer::useNewRenderer (int newRendererIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	if ((newRendererIndex < 0) || (newRendererIndex > rendererInfo.size () - 1))
	{
		consoleOutFunc (-1, int_getString ("Invalid renderer index [ %i ]", newRendererIndex));
		return;
	}

	create (windowWidth, windowHeight, cacheWinFlags, newRendererIndex, int_useVSync, cacheTitle);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the information about the available renderers
void paraRenderer::getRendererInfo ()
//----------------------------------------------------------------------------------------------------------------------
{
	int            numRenderDrivers;
	__rendererInfo tempRendererInfo;

	SDL_RendererInfo renderDriverInfo;

	numRenderDrivers = SDL_GetNumRenderDrivers ();
	if (numRenderDrivers <= 0)
		shutdownFunc ("Error. No renderer drivers available.");

	for (int i = 0; i != numRenderDrivers; i++)
	{
		if (SDL_GetRenderDriverInfo (i, &renderDriverInfo) < 0)
		{
			shutdownFunc (int_getString ("Error. Unable to get render driver info [ %s ]", SDL_GetError ()));
		}
		tempRendererInfo.rendererInfo            = renderDriverInfo;
		tempRendererInfo.rendererName            = renderDriverInfo.name;
		tempRendererInfo.softwareFallback        = (renderDriverInfo.flags & SDL_RENDERER_SOFTWARE) ? true : false;
		tempRendererInfo.hardwareAccelerated     = (renderDriverInfo.flags & SDL_RENDERER_ACCELERATED) ? true : false;
		tempRendererInfo.supportsVSync           = (renderDriverInfo.flags & SDL_RENDERER_PRESENTVSYNC) ? true : false;
		tempRendererInfo.supportsRenderToTexture = (renderDriverInfo.flags & SDL_RENDERER_TARGETTEXTURE) ? true : false;

		rendererInfo.push_back (tempRendererInfo);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Verify that the renderer pulled from the config file is valid and can be used
void paraRenderer::verifyRenderer ()
//----------------------------------------------------------------------------------------------------------------------
{
	if ((whichRenderer < 0) || (whichRenderer > (int) (rendererInfo.size () - 1)))
	{
		whichRenderer = 0;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Debug - show the current renderer to the console
void paraRenderer::d_getCurrentBackingTexture ()
//----------------------------------------------------------------------------------------------------------------------
{
	consoleOutFunc (-1, int_getString ("[ %s ]", rendererInfo[whichRenderer].rendererName.c_str ()));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Display all the created backing textures information to the console
void paraRenderer::d_getAllBackingTextures ()
//----------------------------------------------------------------------------------------------------------------------
{
	for (const auto &textureItr : backingTextures)
	{
		consoleOutFunc (-1, int_getString ("[ %s ] - Logical [ %i x %i ]", textureItr.first.c_str (), textureItr.second.logicalWidth, textureItr.second.logicalHeight));
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get information on the current renderer
void paraRenderer::d_getCurrentRenderer ()
//----------------------------------------------------------------------------------------------------------------------
{
	if (rendererInfo.empty ())
	{
		consoleOutFunc (-1, "Renderer information has not been populated.");
		return;
	}

	if (whichRenderer < (int) (rendererInfo.size ()))
	{
		consoleOutFunc (-1, int_getString ("Renderer name [ %s ]", rendererInfo[whichRenderer].rendererName.c_str ()));
		consoleOutFunc (-1, int_getString (" - Hardware accelerated [ %s ]", rendererInfo[whichRenderer].hardwareAccelerated ? "Yes" : "No"));
		consoleOutFunc (-1, int_getString (" - Render to texture [ %s ]", rendererInfo[whichRenderer].supportsRenderToTexture ? "Yes" : "No"));
		consoleOutFunc (-1, int_getString (" - Render to VSync [ %s ]", rendererInfo[whichRenderer].supportsVSync ? "Yes" : "No"));
		consoleOutFunc (-1, int_getString (" - Software fallback [ %s ]", rendererInfo[whichRenderer].softwareFallback ? "Yes" : "No"));
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Display all the available renderers
void paraRenderer::d_getAllRenderers ()
//----------------------------------------------------------------------------------------------------------------------
{
	int currentCount = 0;

	if (rendererInfo.empty ())
	{
		consoleOutFunc (-1, "Renderer information has not been populated.");
		return;
	}

	for (auto rendererItr : rendererInfo)
	{
		consoleOutFunc (-1, int_getString ("Index [ %i ] Renderer name [ %s ]", currentCount, rendererInfo[currentCount].rendererName.c_str ()));
		consoleOutFunc (-1, int_getString (" - Hardware accelerated [ %s ]", rendererInfo[currentCount].hardwareAccelerated ? "Yes" : "No"));
		consoleOutFunc (-1, int_getString (" - Render to texture [ %s ]", rendererInfo[currentCount].supportsRenderToTexture ? "Yes" : "No"));
		consoleOutFunc (-1, int_getString (" - Render to VSync [ %s ]", rendererInfo[currentCount].supportsVSync ? "Yes" : "No"));
		consoleOutFunc (-1, int_getString (" - Software fallback [ %s ]", rendererInfo[currentCount].softwareFallback ? "Yes" : "No"));
		currentCount++;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the string name of the currently active backing texture
std::string paraRenderer::getCurrentBackingTexture ()
//----------------------------------------------------------------------------------------------------------------------
{
	return activeBackingTexture;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Make a new backing texture active
void paraRenderer::setCurrentBackingTexture (const std::string &newActiveTexture)
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
	shutdownFunc (int_getString ("Unable to set new backing texture. Not found [ %s ]", newActiveTexture.c_str ()));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the render target texture handle
PARA_Texture *paraRenderer::getRenderTarget (const std::string &textureName)
//----------------------------------------------------------------------------------------------------------------------
{
//	SDL_assert_release(renderTargetTexture != nullptr);
	if (backingTextures.empty ())
		shutdownFunc (int_getString ("No backing textures exist."));

	for (const auto &textureItr : backingTextures)
	{
		if (textureName == textureItr.first)
		{
			return textureItr.second.backingTexture;
		}
	}
	shutdownFunc (int_getString ("Backing texture [ %s ] does not exist.", textureName.c_str ()));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create a render target texture
void paraRenderer::createRenderTargetTexture (const std::string &textureName, int logicalWidth, int logicalHeight, int setRenderScaleQuality)
//----------------------------------------------------------------------------------------------------------------------
{
	std::string      hintValue;
	__backingTexture tempBackingTexture{};

	if (!targetTextureAvailable)
		return;

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
			SDL_DestroyTexture (textureItr.second.backingTexture);
			backingTextures.erase (textureName);
		}
	}

	tempBackingTexture.logicalHeight  = logicalHeight;
	tempBackingTexture.logicalWidth   = logicalWidth;
	tempBackingTexture.backingTexture = nullptr;
	//
	// Influence how the scaling is done when rendering the target texture to screen
	hintValue = std::to_string (setRenderScaleQuality);
	SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, hintValue.c_str ());

	tempBackingTexture.backingTexture = SDL_CreateTexture (renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, logicalWidth, logicalHeight);
	if (nullptr == tempBackingTexture.backingTexture)
		shutdownFunc (int_getString ("Unable to create render target texture [ %s ]", SDL_GetError ()));

	backingTextures.insert (std::pair<std::string, __backingTexture> (textureName, tempBackingTexture));

	//
	// Adjust virtual scaling
	SDL_RenderSetLogicalSize (renderer, logicalWidth, logicalHeight);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Reset target texture to screen, and present current texture if required
void paraRenderer::presentFrame ()
//----------------------------------------------------------------------------------------------------------------------
{
	if (targetTextureAvailable)
	{
		SDL_SetRenderTarget (renderer, nullptr);
		SDL_RenderCopy (renderer, getRenderTarget (getCurrentBackingTexture ()), nullptr, nullptr);
	}

	SDL_RenderPresent (renderer);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set a new render target if required, and clear the screen
void paraRenderer::prepareFrame ()
//----------------------------------------------------------------------------------------------------------------------
{
	static int errorCount = 0;

	if (targetTextureAvailable)
	{
		if (SDL_SetRenderTarget (paraRenderer::renderer, getRenderTarget (getCurrentBackingTexture ())) < 0)
		{
			errorCount++;
			if (errorCount > 5)
				shutdownFunc ("Exceeded error count for set render target. Check logfile for details.");
		}
	}

	SDL_SetRenderDrawColor (paraRenderer::renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear (paraRenderer::renderer);
}