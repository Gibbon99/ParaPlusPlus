#include <utility>
#include "main.h"
#include "system/util.h"
#include "io/logFile.h"
#include "classes/paraTexture.h"

void paraTexture::AddRef()
{

}

void paraTexture::ReleaseRef()
{

}

//----------------------------------------------------------------------------------------------------------------------
//
// Deconstructor for a texture - free memory
paraTexture::~paraTexture()
//----------------------------------------------------------------------------------------------------------------------
{
	if (nullptr != surface)
	{
//		SDL_FreeSurface (surface);
//		surface = nullptr;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Constructor
paraTexture::paraTexture(textureFunctionPtrStr outputFunction, textureFunctionPtrLoad loadFunction)
//----------------------------------------------------------------------------------------------------------------------
{
	paraTexture::funcOutput = outputFunction;
	paraTexture::funcLoad   = loadFunction;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return a pointer to the texture
PARA_Texture *paraTexture::getTexture()
//----------------------------------------------------------------------------------------------------------------------
{
	return texture;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return a pointer to the surface
PARA_Surface *paraTexture::getSurface()
//----------------------------------------------------------------------------------------------------------------------
{
	return surface;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Pass in string and parameters to format and return a string
// https://stackoverflow.com/questions/19009094/c-variable-arguments-with-stdstring-only
std::string paraTexture::int_getString(std::string format, ...)
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
// Set the filename for a texture
void paraTexture::setFileName(std::string newFilename)
//----------------------------------------------------------------------------------------------------------------------
{
	fileName = newFilename;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Destroy this texture
void paraTexture::destroy()
//----------------------------------------------------------------------------------------------------------------------
{
	SDL_DestroyTexture (texture);
	SDL_FreeSurface (surface);
	texture = nullptr;
	surface = nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Check if a texture has been loaded properly
bool paraTexture::isLoaded()
//----------------------------------------------------------------------------------------------------------------------
{
	return loaded;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Load a bitmap from a memory pointer - create a texture from the surface
bool paraTexture::load(std::string newFileName, std::string newKeyName)
//----------------------------------------------------------------------------------------------------------------------
{
	if (newFileName.empty ())
	{
		funcOutput (-1, int_getString ("Texture load - missing filename."));
		return false;
	}

	loaded = false;

	if (nullptr != surface)
		SDL_FreeSurface (surface);

	fileName = std::move (newFileName);
	surface  = SDL_LoadBMP_RW (funcLoad (fileName), 0);     // TODO Free surface - memory leak - make constructor and free SDL_FreeSurface(surface)
	if (nullptr == surface)
	{
		funcOutput (-1, int_getString ("Unable to load texture file [ %s ] - [ %s ]", fileName.c_str (), SDL_GetError ()));
		return false;
	}

	texture = SDL_CreateTextureFromSurface (renderer.renderer, surface);
	if (nullptr == texture)
	{
		funcOutput (-1, int_getString ("Failed to create texture [ %s ] - [ %s ]", fileName.c_str (), SDL_GetError ()));
		return false;
	}

	keyName = std::move (newKeyName);
	if (SDL_QueryTexture (texture, nullptr, nullptr, &width, &height) < 0)
	{
		funcOutput (-1, int_getString ("Failed to query next texture [ %s ] - [ %s ]", fileName.c_str (), SDL_GetError ()));
		return false;
	}

	funcOutput (-1, int_getString ("Loaded [ %s ] with key [ %s ]", fileName.c_str (), keyName.c_str ()));
#ifdef MY_DEBUG
	log_addEvent (sys_getString ("[ %s ] Loaded file [ %s ]", __func__, fileName.c_str ()));
#endif
	sys_freeMemory (fileName);

	loaded = true;
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the height of a texture
int paraTexture::getHeight() const
//----------------------------------------------------------------------------------------------------------------------
{
	return height;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the width of a texture
int paraTexture::getWidth() const
//----------------------------------------------------------------------------------------------------------------------
{
	return width;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a texture to the current backing target - or screen if backing texture is not used
void paraTexture::render()
//----------------------------------------------------------------------------------------------------------------------
{
	if (!loaded)
		return;

	try
	{
		if (keyName == "screen")
			SDL_SetTextureBlendMode (static_cast<PARA_Texture *>(texture), SDL_BLENDMODE_MOD); //SDL_BLENDMODE_BLEND);
		else
			SDL_SetTextureBlendMode (static_cast<PARA_Texture *>(texture), SDL_BLENDMODE_BLEND);
	}
	catch (std::out_of_range &outOfRange)
	{
	}

	if (SDL_RenderCopy (renderer.renderer, texture, nullptr, nullptr) < 0)
	{
		funcOutput (-1, int_getString ("Unable to render texture [ %s ] - [ %s ]", keyName.c_str (), SDL_GetError ()));
		return;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a texture to the current backing target - or screen if backing texture is not used
//
// Pass in destination coords - coords are in pixel space
void paraTexture::render(SDL_Rect *destination)
//----------------------------------------------------------------------------------------------------------------------
{
	if (SDL_RenderCopy (renderer.renderer, texture, nullptr, destination) < 0)
	{
		funcOutput (-1, int_getString ("Unable to render texture [ %s ] - [ %s ]", keyName.c_str (), SDL_GetError ()));
		return;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the boolean value from single bit array of the image pixels
char paraTexture::pixelColor(int posX, int posY)
//----------------------------------------------------------------------------------------------------------------------
{
	if ((posY * surface->w + posX) > static_cast<int>(collisionMap.size ()))
	{
		sys_shutdownWithError ("Attempted out of bound access on collision map.");
	}
	return collisionMap[posY * surface->w + posX];
}
