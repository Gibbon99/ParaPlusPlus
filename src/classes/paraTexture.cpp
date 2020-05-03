#include <main.h>
#include "classes/paraTexture.h"

void paraTexture::AddRef ()
{

}

void paraTexture::ReleaseRef ()
{

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

//-----------------------------------------------------------------------------------------------------------------------
//
// Init the texture callbacks system
int paraTexture::init (textureFunctionPtrStr outputFunction, textureFunctionPtrLoad loadFunction)
//-----------------------------------------------------------------------------------------------------------------------
{
	paraTexture::funcOutput = outputFunction;
	paraTexture::funcLoad   = loadFunction;

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Load a bitmap from a memory pointer - create a texture from the surface
bool paraTexture::load (std::string fileName)
//----------------------------------------------------------------------------------------------------------------------
{
	__texture tempTexture;

	if (fileName.empty ())
	{
		funcOutput (-1, int_getString ("Texture load - missing filename."));
		return false;
	}

	tempTexture.fileName = fileName;
	tempTexture.surface  = SDL_LoadBMP_RW (funcLoad (tempTexture.fileName), 0);
	if (nullptr == tempTexture.surface)
	{
		funcOutput (-1, int_getString ("Unable to load texture file [ %s ] - [ %s ]", tempTexture.fileName.c_str (), SDL_GetError ()));
		return false;
	}

	tempTexture.texture = SDL_CreateTextureFromSurface (renderer.renderer, tempTexture.surface);
	if (nullptr == tempTexture.texture)
	{
		funcOutput (-1, int_getString ("Failed to create texture [ %s ] - [ %s ]", tempTexture.fileName.c_str (), SDL_GetError ()));
		return false;
	}
	tempTexture.loaded  = true;
	tempTexture.keyName = fileName.erase (fileName.find_last_of ("."), std::string::npos);
	if (SDL_QueryTexture (tempTexture.texture, NULL, NULL, &tempTexture.width, &tempTexture.height) < 0)
	{
		funcOutput (-1, int_getString ("Failed to query next texture [ %s ] - [ %s ]", tempTexture.fileName.c_str (), SDL_GetError ()));
		return false;
	}

	texture.insert (std::pair<std::string, __texture> (tempTexture.keyName, tempTexture));
	funcOutput (-1, int_getString ("Loaded [ %s ] with key [ %s ]", tempTexture.fileName.c_str (), tempTexture.keyName.c_str ()));
#ifdef MY_DEBUG
	std::cout << "Loaded file : " << tempTexture.fileName << std::endl;
#endif
	sys_freeMemory (tempTexture.fileName);

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a texture to the current backing target - or screen if backing texture is not used
void paraTexture::render (std::string keyName)
//----------------------------------------------------------------------------------------------------------------------
{
	if (SDL_RenderCopy (renderer.renderer, texture[keyName].texture, nullptr, nullptr) < 0)
	{
		funcOutput (-1, int_getString ("Unable to render texture [ %s ] - [ %s ]", keyName.c_str (), SDL_GetError ()));
		return;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the boolean value from single bit array of the image pixels
char paraTexture::pixelColor (std::string textureName, int posX, int posY)
//----------------------------------------------------------------------------------------------------------------------
{
	return texture[textureName].collisionMap[posY * texture[textureName].surface->w + posX];
}

//----------------------------------------------------------------------------------------------------------------------
//
// Load a collision map
void paraTexture::loadMap (std::string textureName)
//----------------------------------------------------------------------------------------------------------------------
{
	PHYSFS_file   *fileHandle;
	PHYSFS_sint64 returnCode;
	std::string collisionMapFileName;

	collisionMapFileName = textureName + ".map";

	auto fileSize = fileSystem.getFileSize (collisionMapFileName);
	texture[textureName].collisionMap.resize (fileSize);

	fileHandle = PHYSFS_openRead (collisionMapFileName.c_str());
	if (nullptr == fileHandle)
	{
		funcOutput (-1, int_getString("Unable to open file [ %s ] [ %s ]", collisionMapFileName.c_str(), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		return;
	}

	returnCode = PHYSFS_readBytes (fileHandle, &texture[textureName].collisionMap[0], fileSize * sizeof (char));
	if (returnCode < 0)
	{
		funcOutput (-1, int_getString("Unable to read file [ %s ] [ %s ]", collisionMapFileName.c_str(), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		return;
	}

	PHYSFS_close (fileHandle);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create a collision map - used for starfield background
bool paraTexture::createMap (std::string textureName)
//----------------------------------------------------------------------------------------------------------------------
{
	int  counter = 0;
	std::string collisionMapFileName;

	collisionMapFileName = textureName + ".map";

	if (fileSystem.doesFileExist (collisionMapFileName))
	{
		loadMap (texture[textureName].keyName);
		return true;
	}

	texture[textureName].collisionMap.resize (texture[textureName].surface->h * texture[textureName].surface->w);

	texture[textureName].surface = SDL_ConvertSurfaceFormat (texture[textureName].surface, SDL_PIXELFORMAT_ARGB8888, 0);

	//If the surface must be locked
	if (SDL_MUSTLOCK(texture[textureName].surface))
	{
		//Lock the surface
		SDL_LockSurface (texture[textureName].surface);
	}

	Uint32 *pixels    = (Uint32 *) texture[textureName].surface->pixels;
	Uint32 blackColor = SDL_MapRGBA (texture[textureName].surface->format, 0, 0, 0, 255);

	std::cout << "Start map creation" << std::endl;

	for (int      y = 0; y < texture[textureName].surface->h; y++)
	{
		for (int x = 0; x < texture[textureName].surface->w; x++)
		{
			if (blackColor != pixels[y * texture[textureName].surface->w + x])
				texture[texture[textureName].keyName].collisionMap[counter] = 0;
			else
				texture[texture[textureName].keyName].collisionMap[counter] = 1;

			if (counter == (texture[textureName].surface->h * texture[textureName].surface->w) / 2)
				std::cout << "Done 50%" << std::endl;

			counter++;
		}
	}

	//
	// Write out to file to cache for next run
	PHYSFS_file *fileHandle;
	PHYSFS_sint64 returnCode;

	fileHandle = PHYSFS_openWrite (collisionMapFileName.c_str());
	if (fileHandle == nullptr)
	{
		funcOutput (-1, int_getString ("Unable to open file for writing [ %s ] - [ %s ]", collisionMapFileName.c_str(), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		return false;
	}

	returnCode = PHYSFS_writeBytes (fileHandle, &texture[textureName].collisionMap[0], (texture[textureName].collisionMap.size()) * sizeof (char));

	if (returnCode < 0)
	{
		funcOutput (-1, int_getString ("Incomplete file write [ %s ] - [ %s ]", collisionMapFileName.c_str(), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		return false;
	}
	PHYSFS_close (fileHandle);

	//If the surface must be locked
	if (SDL_MUSTLOCK(texture[textureName].surface))
	{
		//Lock the surface
		SDL_UnlockSurface (texture[textureName].surface);
	}
}
