#include "game/terminal.h"
#include "game/texture.h"
#include "system/util.h"

struct paraMemoryMap
{
	char *pointer;
	int  size;
};

std::map<std::string, paraMemoryMap> memoryMap;

float cosTable[360];
float sinTable[360];

//-----------------------------------------------------------------------------------------------------------
//
// Create the sin/cos tables
void createLookupTable()
//-----------------------------------------------------------------------------------------------------------
{
	for (int i = 0; i != 360; i++)
	{
		cosTable[i] = cos (i);
		sinTable[i] = sin (i);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Look up a value from the COS table
float sys_getCosValue(int angle)
//----------------------------------------------------------------------------------------------------------------------
{
	if ((angle < 0) || (angle > 360))
		return 0.0f;

	return cosTable[angle];
}

//----------------------------------------------------------------------------------------------------------------------
//
// Look up a value from the SIN table
float sys_getSinValue(int angle)
//----------------------------------------------------------------------------------------------------------------------
{
	if ((angle < 0) || (angle > 360))
		return 0.0f;

	return sinTable[angle];
}

//----------------------------------------------------------------------------------------------------------------------
//
// Save a SDL_Texture to a file
void sys_saveTexture(SDL_Renderer *ren, SDL_Texture *tex, const char *filename)
//----------------------------------------------------------------------------------------------------------------------
{
	SDL_Texture *ren_tex;
	SDL_Surface *surf;
	int         st;
	int         w;
	int         h;
	int         format;
	void        *pixels;

	pixels  = nullptr;
	surf    = nullptr;
	ren_tex = nullptr;
	format  = SDL_PIXELFORMAT_RGBA32;

	/* Get information about texture we want to save */
	st = SDL_QueryTexture (tex, NULL, NULL, &w, &h);
	if (st != 0)
	{
		SDL_Log ("Failed querying texture: %s\n", SDL_GetError ());
		goto cleanup;
	}

	ren_tex = SDL_CreateTexture (ren, format, SDL_TEXTUREACCESS_TARGET, w, h);
	if (!ren_tex)
	{
		SDL_Log ("Failed creating render texture: %s\n", SDL_GetError ());
		goto cleanup;
	}

	/*
	 * Initialize our canvas, then copy texture to a target whose pixel data we
	 * can access
	 */
	st = SDL_SetRenderTarget (ren, ren_tex);
	if (st != 0)
	{
		SDL_Log ("Failed setting render target: %s\n", SDL_GetError ());
		goto cleanup;
	}

	SDL_SetRenderDrawColor (ren, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear (ren);

	st = SDL_RenderCopy (ren, tex, NULL, NULL);
	if (st != 0)
	{
		SDL_Log ("Failed copying texture data: %s\n", SDL_GetError ());
		goto cleanup;
	}

	/* Create buffer to hold texture data and load it */
	pixels = malloc (w * h * SDL_BYTESPERPIXEL(format));
	if (!pixels)
	{
		SDL_Log ("Failed allocating memory\n");
		goto cleanup;
	}

	st = SDL_RenderReadPixels (ren, NULL, format, pixels, w * SDL_BYTESPERPIXEL(format));
	if (st != 0)
	{
		SDL_Log ("Failed reading pixel data: %s\n", SDL_GetError ());
		goto cleanup;
	}

	/* Copy pixel data over to surface */
	surf = SDL_CreateRGBSurfaceWithFormatFrom (pixels, w, h, SDL_BITSPERPIXEL(format), w * SDL_BYTESPERPIXEL(format), format);
	if (!surf)
	{
		SDL_Log ("Failed creating new surface: %s\n", SDL_GetError ());
		goto cleanup;
	}

	/* Save result to an image */
	st = SDL_SaveBMP(surf, filename);
	if (st != 0)
	{
		SDL_Log ("Failed saving image: %s\n", SDL_GetError ());
		goto cleanup;
	}

	SDL_Log ("Saved texture as BMP to \"%s\"\n", filename);

	cleanup:
	SDL_FreeSurface (surf);
	free (pixels);
	SDL_DestroyTexture (ren_tex);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set a new mode
void sys_setNewMode(int newMode, bool doFade)
//----------------------------------------------------------------------------------------------------------------------
{
	if (doFade)
	{
		renderer.prepareFade ();
	}

	switch (newMode)
	{
		case MODE_CONSOLE_INIT:
			gui.setRepeatOff (true);
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_PRE_GAME:
			gui.setRepeatOff (false);
			currentMode = newMode;
			break;

		case MODE_GAME:
			gui.setRepeatOff (false);
			currentMode = newMode;
			renderer.setCurrentBackingTexture (GAME_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, gameWinWidth, gameWinHeight);
			break;

		case MODE_CONSOLE_EDIT:
			gui.setRepeatOff (true);
			//
			// Change to new backing texture
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			//
			// Adjust virtual scaling
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			currentMode = newMode;
			break;

		case MODE_SHOW_SPLASH:
			gui.setRepeatOff (true);
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_GUI_MAINMENU:
			gui.setRepeatOff (true);
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);

			audio.stopAllChannels ();
			gui.setCurrentScreen (gui.getIndex (GUI_OBJECT_SCREEN, "mainMenu"));
			gui.setActiveObject (gui.getCurrentScreen (), GUI_OBJECT_BUTTON, "mainMenu.startGameButton");
			break;

		case MODE_GUI_HIGHSCORE_DISPLAY:
			gui.setRepeatOff (true);
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_GUI_HIGHSCORE_ENTRY:
			gui.setRepeatOff (true);
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_GUI_INTROSCROLL:
			gui.setRepeatOff (true);
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_GUI_TUT_TRANSFER_START:
		case MODE_GUI_TUT_TRANSFER_GAME:
		case MODE_GUI_TUT_LIFTS:
		case MODE_GUI_TUT_TERMINALS:
		case MODE_GUI_TUT_HEALING:
		case MODE_GUI_TUT_TIPS:
			gui.setRepeatOff (true);
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_GUI_TERMINAL:
			gui.setRepeatOff (true);
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_GUI_DATABASE:
			gui.setRepeatOff (true);
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_GUI_SHIPVIEW:
			gui.setRepeatOff (true);
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_GUI_DECKVIEW:
			gui.setRepeatOff (true);
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_GUI_LIFTVIEW:
			gui.setRepeatOff (true);
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_PRE_TRANSFER:
			currentMode = newMode;
			break;

		case MODE_TRANSFER_SCREEN_ONE:
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_TRANSFER_PRE_SCREEN_TWO:
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_TRANSFER_SCREEN_TWO:
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_PRE_TRANSFER_CHOOSE_SIDE:
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_TRANSFER_CHOOSE_SIDE:
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_TRANSFER_SELECT_SIDE:
			currentMode = newMode;
			break;

		case MODE_PRE_TRANSFER_GAME:
			currentMode = newMode;
			break;

		case MODE_TRANSFER_GAME:
		case MODE_TRANSFER_RESULT:
			currentMode = newMode;
			break;

		case MODE_TRANSFER_DEADLOCK:
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_END_PRE_LOST_SCREEN:
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_GAME_OVER:
			currentMode = newMode;
//			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
//			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);

			renderer.setCurrentBackingTexture (GAME_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, gameWinWidth, gameWinHeight);
			break;

		case MODE_END_LOST_SCREEN:
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_GUI_WON_SCREEN:
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_GUI_PRE_HIGHSCORE_SCREEN:
			currentMode = newMode;
			renderer.setCurrentBackingTexture (HIRES_BACKING_TEXTURE);
			SDL_RenderSetLogicalSize (renderer.renderer, hiresVirtualWidth, hiresVirtualHeight);
			break;

		case MODE_GUI_KEYCODE_ENTRY:
			currentMode = newMode;
			break;

		default:
			sys_shutdownWithError ("Attempting to set an unknown mode.");
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the operating system we are running on
void sys_getOS()
//----------------------------------------------------------------------------------------------------------------------
{
	con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("[ %s ]", SDL_GetPlatform ()));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Pass in string and parameters to format and return a string
// https://stackoverflow.com/questions/19009094/c-variable-arguments-with-stdstring-only
std::string sys_getString(const std::string &format, ...)
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

	// return a formatted string without risking memory mismanagement  and without assuming any compiler
	// or platform specific behavior
	std::vector<char> zc (iLen + 1);
	std::vsnprintf (zc.data (), zc.size (), zcFormat, vaArgs);
	va_end(vaArgs);

	return std::string (zc.data ());
}

//----------------------------------------------------------------------------------------------------------------------
//
// Allocate memory and return pointer - record the size as well
char *sys_malloc(int memorySize, std::string keyName)
//----------------------------------------------------------------------------------------------------------------------
{
	paraMemoryMap newMemoryMapEntry;

	newMemoryMapEntry.pointer = (char *) malloc (sizeof (char) * memorySize);
	if (newMemoryMapEntry.pointer == nullptr)
	{
		logFile.write (sys_getString ("Memory allocation error for [ %s ]", keyName.c_str ()));
	}

	newMemoryMapEntry.size = memorySize;

	memoryMap.insert (std::pair<std::string, paraMemoryMap> (keyName, newMemoryMapEntry));

#ifdef DEBUG_MEMORY
	logFile.write (sys_getString ("Allocated [ %i ] for [ %s ]", memorySize, keyName.c_str ()));
#endif

	return newMemoryMapEntry.pointer;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Free a memory slot by name
void sys_freeMemory(std::string keyName)
//----------------------------------------------------------------------------------------------------------------------
{
	if (memoryMap.size () == 0)
		return;

	auto memoryItr = memoryMap.find (keyName);

	if (nullptr != memoryItr->second.pointer)
	{
#ifdef DEBUG_MEMORY
		logFile.write (sys_getString ("Free memory [ %i bytes ] - [ %s ]", memoryItr->second.size, memoryItr->first.c_str ()));
#endif
		free (memoryItr->second.pointer);
		memoryMap.erase (memoryItr);
		return;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Go through memory map and free allocations
void sys_freeMemory()
//----------------------------------------------------------------------------------------------------------------------
{
	if (memoryMap.size () == 0)
		return;

	for (auto &memoryItr: memoryMap)
	{
#ifdef DEBUG_MEMORY
		logFile.write (sys_getString ("Free memory [ %i bytes ] - [ %s ]", memoryItr.second.size, memoryItr.first.c_str ()));
#endif
		if (memoryItr.second.pointer != nullptr)
		{
			free (memoryItr.second.pointer);
			memoryItr.second.pointer = nullptr;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Is an object visible on the screen
bool sys_visibleOnScreen(cpVect worldCoord, int shapeSize)
//----------------------------------------------------------------------------------------------------------------------
{
	if (worldCoord.x < viewportRect.x - static_cast<float>(shapeSize))
	{
		return false;
	}
	if (worldCoord.x > viewportRect.x + viewportRect.w + static_cast<float>(shapeSize))
	{
		return false;
	}
	if (worldCoord.y < viewportRect.y - static_cast<float>( shapeSize))
	{
		return false;
	}
	if (worldCoord.y > viewportRect.y + viewportRect.h + static_cast<float>(shapeSize))
	{
		return false;
	}
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Convert worldPosition coords to screen coords
cpVect sys_worldToScreen(cpVect worldPos, int shapeSize)
//----------------------------------------------------------------------------------------------------------------------
{
	cpVect screenCoords {};

	if (sys_visibleOnScreen (worldPos, shapeSize) != 0)
	{
		screenCoords.x = worldPos.x - viewportRect.x;
		screenCoords.y = worldPos.y - viewportRect.y;
	}
	else
	{
		screenCoords.x = worldPos.x - viewportRect.x;
		screenCoords.y = worldPos.y - viewportRect.y;

//		screenCoords.x = -1;
//		screenCoords.y = -1;
	}
	return screenCoords;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Convert from pixels to tileSize coordinates
cpVect sys_convertPixelsToTiles(cpVect convertFrom)
//----------------------------------------------------------------------------------------------------------------------
{
	cpVect returnValue;

	returnValue.x = static_cast<int>(convertFrom.x / tileSize);
	returnValue.y = static_cast<int>(convertFrom.y / tileSize);

	return returnValue;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Convert from tiles to pixels coordinates
cpVect sys_convertTilesToPixels(cpVect convertFrom)
//----------------------------------------------------------------------------------------------------------------------
{
	cpVect returnValue;

	returnValue.x = static_cast<int>(convertFrom.x) * tileSize;
	returnValue.y = static_cast<int>(convertFrom.y) * tileSize;

	return returnValue;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Convert a string to an integer value
int sys_convertToInt(std::string convertFrom)
//----------------------------------------------------------------------------------------------------------------------
{
	return static_cast<int>(strtol (convertFrom.c_str (), nullptr, 10));
}
