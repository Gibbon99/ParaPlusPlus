
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
	PARA_Surface *tempSurface;

	if (fileName.empty ())
	{
		funcOutput (-1, int_getString ("Texture load - missing filename."));
		return false;
	}

	tempTexture.fileName = fileName;
	tempTexture.keyName  = fileName.erase (fileName.find_last_of ("."), std::string::npos);
	tempSurface = SDL_LoadBMP_RW (funcLoad (tempTexture.fileName), 0);
	if (nullptr == tempSurface)
	{
		funcOutput (-1, int_getString ("Unable to load texture file [ %s ] - [ %s ]", tempTexture.fileName.c_str (), SDL_GetError ()));
		return false;
	}

	tempTexture.texture = SDL_CreateTextureFromSurface (renderer.renderer, tempSurface);
	if (nullptr == tempTexture.texture)
	{
		funcOutput (-1, int_getString ("Failed to create texture [ %s ] - [ %s ]", tempTexture.fileName.c_str (), SDL_GetError ()));
		return false;
	}
	tempTexture.loaded = true;
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
void paraTexture::render(std::string keyName)
//----------------------------------------------------------------------------------------------------------------------
{
	if (SDL_RenderCopy(renderer.renderer, texture[keyName].texture, nullptr, nullptr) < 0)
	{
		funcOutput(-1, int_getString("Unable to render texture [ %s ] - [ %s ]", keyName.c_str(), SDL_GetError()));
		return;
	}
}