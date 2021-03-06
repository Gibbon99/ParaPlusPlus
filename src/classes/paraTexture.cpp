#include <main.h>

#include <utility>
#include <system/util.h>
#include "classes/paraTexture.h"

void paraTexture::AddRef ()
{

}

void paraTexture::ReleaseRef ()
{

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
// Pass in string and parameters to format and return a string
// https://stackoverflow.com/questions/19009094/c-variable-arguments-with-stdstring-only
std::string paraTexture::int_getString (std::string format, ...)
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
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	texture = nullptr;
	surface = nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Check if a texture has been loaded properly
bool paraTexture::isLoaded ()
//----------------------------------------------------------------------------------------------------------------------
{
	return loaded;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Load a bitmap from a memory pointer - create a texture from the surface
bool paraTexture::load (std::string newFileName, std::string newKeyName)
//----------------------------------------------------------------------------------------------------------------------
{
	if (newFileName.empty ())
	{
		funcOutput (-1, int_getString ("Texture load - missing filename."));
		return false;
	}

	loaded  = false;

	fileName = std::move(newFileName);
	surface  = SDL_LoadBMP_RW (funcLoad (fileName), 0);
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

	keyName = std::move(newKeyName);
	if (SDL_QueryTexture (texture, nullptr, nullptr, &width, &height) < 0)
	{
		funcOutput (-1, int_getString ("Failed to query next texture [ %s ] - [ %s ]", fileName.c_str (), SDL_GetError ()));
		return false;
	}

	funcOutput (-1, int_getString ("Loaded [ %s ] with key [ %s ]", fileName.c_str (), keyName.c_str ()));
#ifdef MY_DEBUG
	std::cout << "Loaded file : " << fileName << std::endl;
#endif
	sys_freeMemory (fileName);

	loaded  = true;
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the height of a texture
int paraTexture::getHeight ()
//----------------------------------------------------------------------------------------------------------------------
{
	return height;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the width of a texture
int paraTexture::getWidth ()
//----------------------------------------------------------------------------------------------------------------------
{
	return width;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a texture to the current backing target - or screen if backing texture is not used
void paraTexture::render ()
//----------------------------------------------------------------------------------------------------------------------
{
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
void paraTexture::render (SDL_Rect *destination)
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
char paraTexture::pixelColor (int posX, int posY)
//----------------------------------------------------------------------------------------------------------------------
{
	if ((posY * surface->w + posX) > static_cast<int>(collisionMap.size()))
	{
		sys_shutdownWithError("Attempted out of bound access on collision map.");
	}
	return collisionMap[posY * surface->w + posX];
}

//----------------------------------------------------------------------------------------------------------------------
//
// Load a collision map
void paraTexture::loadMap ()
//----------------------------------------------------------------------------------------------------------------------
{
	PHYSFS_file   *fileHandle;
	PHYSFS_sint64 returnCode;
	std::string   collisionMapFileName;

	collisionMapFileName = keyName + ".map";

	auto fileSize = fileSystem.getFileSize (collisionMapFileName);
	collisionMap.resize (fileSize);

	fileHandle = PHYSFS_openRead (collisionMapFileName.c_str ());
	if (nullptr == fileHandle)
	{
		funcOutput (-1, int_getString ("Unable to open file [ %s ] [ %s ]", collisionMapFileName.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		return;
	}

	returnCode = PHYSFS_readBytes (fileHandle, &collisionMap[0], fileSize * sizeof (char));
	if (returnCode < 0)
	{
		funcOutput (-1, int_getString ("Unable to read file [ %s ] [ %s ]", collisionMapFileName.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		return;
	}

	PHYSFS_close (fileHandle);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create a collision map - used for starfield background
bool paraTexture::createMap ()
//----------------------------------------------------------------------------------------------------------------------
{
	int         counter = 0;
	std::string collisionMapFileName;

	collisionMapFileName = keyName + ".map";

	if (fileSystem.doesFileExist (collisionMapFileName))
	{
		loadMap ();
		return true;
	}

	collisionMap.resize (surface->h * surface->w);

	surface = SDL_ConvertSurfaceFormat (surface, SDL_PIXELFORMAT_ARGB8888, 0);

	//If the surface must be locked
	if (SDL_MUSTLOCK(surface))
	{
		//Lock the surface
		SDL_LockSurface (surface);
	}
	Uint32 *pixels    = (Uint32 *) surface->pixels;
	Uint32 blackColor = SDL_MapRGBA (surface->format, 0, 0, 0, 255);

	std::cout << "Start map creation" << std::endl;

	for (int y = 0; y < surface->h; y++)
	{
		for (int x = 0; x < surface->w; x++)
		{
			if (blackColor != pixels[y * surface->w + x])
				collisionMap[counter] = 0;
			else
				collisionMap[counter] = 1;

			if (counter == (surface->h * surface->w) / 2)
				std::cout << "Done 50%" << std::endl;

			counter++;
		}
	}

	//
	// Write out to file to cache for next run
	PHYSFS_file *fileHandle;
	PHYSFS_sint64 returnCode;

	fileHandle = PHYSFS_openWrite (collisionMapFileName.c_str ());
	if (fileHandle == nullptr)
	{
		funcOutput (-1, int_getString ("Unable to open file for writing [ %s ] - [ %s ]", collisionMapFileName.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		return false;
	}

	returnCode = PHYSFS_writeBytes (fileHandle, &collisionMap[0], (collisionMap.size ()) * sizeof (char));

	if (returnCode < 0)
	{
		funcOutput (-1, int_getString ("Incomplete file write [ %s ] - [ %s ]", collisionMapFileName.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		return false;
	}
	PHYSFS_close (fileHandle);

	//If the surface must be locked
	if (SDL_MUSTLOCK(surface))
	{
		//Lock the surface
		SDL_UnlockSurface (surface);
	}

	return true;
}
