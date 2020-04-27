#include <vector>
#include <cstdarg>
#include "../../hdr/classes/paraFont.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Set a function to call when displaying any output
void paraFont::setOutputFunction(functionPtrStr outputFunction)
//----------------------------------------------------------------------------------------------------------------------
{
	paraFont::funcOutput = outputFunction;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Pass in string and parameters to format and return a string
// https://stackoverflow.com/questions/19009094/c-variable-arguments-with-stdstring-only
std::string paraFont::int_getString(std::string format, ...)
//----------------------------------------------------------------------------------------------------------------------
{
	const char *const zcFormat = format.c_str();

	// initialize use of the variable argument array
	va_list vaArgs;
	va_start(vaArgs, format);

	// reliably acquire the size from a copy of the variable argument array
	// and a functionally reliable call to mock the formatting
	va_list vaCopy;
	va_copy(vaCopy, vaArgs);
	const int iLen = std::vsnprintf(nullptr, 0, zcFormat, vaCopy);
	va_end(vaCopy);

	// return a formatted string without risking memory mismanagement  and without assuming any compiler
	// or platform specific behavior
	std::vector<char> zc(iLen + 1);
	std::vsnprintf(zc.data(), zc.size(), zcFormat, vaArgs);
	va_end(vaArgs);

	return std::string(zc.data());
}

//----------------------------------------------------------------------------------------------------------------------
//
// Start the TTF if required, load a font file with a size
bool paraFont::load(int fontSize, const std::string &fileName)
//----------------------------------------------------------------------------------------------------------------------
{
	if (!TTF_WasInit())
	{
		if (-1 == TTF_Init())
		{
			paraFont::fontSystemAvailable = false;
			funcOutput(int_getString("Unable to start font system [ %s ]", TTF_GetError()));
			return false;
		}
		SDL_version compile_version, *link_version;

		TTF_VERSION(&compile_version)
		funcOutput(int_getString("Compiled with SDL_ttf version: %d.%d.%d",
		                         compile_version.major,
		                         compile_version.minor,
		                         compile_version.patch));

		link_version = const_cast<SDL_version *>(TTF_Linked_Version());
		funcOutput(int_getString("Running with SDL_ttf version: %d.%d.%d",
		                         link_version->major,
		                         link_version->minor,
		                         link_version->patch));
	}

	paraFont::fontHandle = TTF_OpenFont(fileName.c_str(), fontSize);
	if (nullptr == paraFont::fontHandle)
	{
		funcOutput(int_getString("Unable to open font [ %s ] - [ %s ]", fileName.c_str(), TTF_GetError()));
		paraFont::available = false;
		return false;
	}
	paraFont::available  = true;
	paraFont::lineHeight = TTF_FontLineSkip(paraFont::fontHandle);

	funcOutput(int_getString("Loaded font file [ %s ]", fileName.c_str()));

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Close the TTF file
void paraFont::close()
//----------------------------------------------------------------------------------------------------------------------
{
	TTF_CloseFont(paraFont::fontHandle);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the color to print this font in
void paraFont::setColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
//----------------------------------------------------------------------------------------------------------------------
{
	if (paraFont::available)
	{
		paraFont::color.r = red;
		paraFont::color.g = green;
		paraFont::color.b = blue;
		paraFont::color.a = alpha;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create a surface to hold the text and return pointer to the surface
PARA_Surface *paraFont::write(float X, float Y, const std::string &fontText)
//----------------------------------------------------------------------------------------------------------------------
{
	if (nullptr != paraFont::surface)
	{
		SDL_FreeSurface(paraFont::surface);
	}

	paraFont::surface = TTF_RenderText_Blended(paraFont::fontHandle, fontText.c_str(), paraFont::color);
	if (nullptr == paraFont::surface)
	{
		funcOutput(int_getString("Unable to render font to surface [ %s ]", TTF_GetError()));
		return nullptr;
	}
	paraFont::pos.x = (int) X;
	paraFont::pos.y = (int) Y;
	paraFont::pos.w = paraFont::surface->w;
	paraFont::pos.h = paraFont::surface->h;

	return paraFont::surface;
}