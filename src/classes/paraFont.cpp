#include <vector>
#include <cstdarg>
#include "../../hdr/classes/paraFont.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Set a function to call when displaying any output
void paraFont::setOutputFunction(funcPtrIntStr outputFunction)
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
// Set the new font to use
void paraFont::use(std::string keyName)
//----------------------------------------------------------------------------------------------------------------------
{
	auto fontItr = fonts.find(keyName);
	if (fontItr == fonts.end())
	{
		funcOutput(-1, int_getString("Unable to find font [ %s ]", keyName.c_str()));
		return;
	}

	currentFont = keyName;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Start the TTF if required, load a font file with a size
bool paraFont::load (int fontSize, std::string keyName, std::string fileName)
//----------------------------------------------------------------------------------------------------------------------
{
	__PARA_FONT     tempFont{};

	if (!TTF_WasInit())
	{
		if (-1 == TTF_Init())
		{
			paraFont::fontSystemAvailable = false;
			funcOutput(-1, int_getString("Unable to start font system [ %s ]", TTF_GetError()));
			return false;
		}
		SDL_version compile_version, *link_version;

		TTF_VERSION(&compile_version)
		funcOutput(-1, int_getString("Compiled with SDL_ttf version: %d.%d.%d",
		                         compile_version.major,
		                         compile_version.minor,
		                         compile_version.patch));

		link_version = const_cast<SDL_version *>(TTF_Linked_Version());
		funcOutput(-1, int_getString("Running with SDL_ttf version: %d.%d.%d",
		                         link_version->major,
		                         link_version->minor,
		                         link_version->patch));
	}

	tempFont.handle = TTF_OpenFont(fileName.c_str(), fontSize);
	if (nullptr == tempFont.handle)
	{
		funcOutput(-1, int_getString("Unable to open font [ %s ] - [ %s ]", fileName.c_str(), TTF_GetError()));
		tempFont.available = false;
		return false;
	}
	tempFont.available  = true;
	tempFont.lineHeight = TTF_FontLineSkip(tempFont.handle);

	fonts.insert(std::pair<std::string, __PARA_FONT>(keyName, tempFont));

	funcOutput(-1, int_getString("Loaded font file [ %s ]", fileName.c_str()));

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
	if (fonts[currentFont].available)
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
PARA_Surface *paraFont::write(double X, double Y, std::string fontText)
//----------------------------------------------------------------------------------------------------------------------
{
	if (nullptr != paraFont::surface)
	{
		SDL_FreeSurface(paraFont::surface);
	}

	paraFont::surface = TTF_RenderText_Blended(fonts[currentFont].handle, fontText.c_str(), paraFont::color);
	if (nullptr == paraFont::surface)
	{
		funcOutput(-1, int_getString("Unable to render font to surface [ %s ]", TTF_GetError()));
		return nullptr;
	}
	paraFont::pos.x = static_cast<int>(X);
	paraFont::pos.y = static_cast<int>(Y);
	paraFont::pos.w = paraFont::surface->w;
	paraFont::pos.h = paraFont::surface->h;

	return paraFont::surface;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the height of the currently set font
int paraFont::height()
//----------------------------------------------------------------------------------------------------------------------
{
	return fonts[currentFont].lineHeight;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the length of a string when rendered in the current font
int paraFont::width(std::string fontText)
//----------------------------------------------------------------------------------------------------------------------
{
	int textWidth;
	int textHeight;
	int returnCode;

	returnCode = TTF_SizeText(fonts[currentFont].handle, fontText.c_str(), &textWidth, &textHeight);
	if (returnCode < 0)
	{
		funcOutput(-1, int_getString("Unable to get text width. Font [ %s ]", currentFont.c_str()));
		return -1;
	}

	return textWidth;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a string of text
void paraFont::render(SDL_Renderer *whichRenderer, double posX, double posY, int r, int g, int b, int a, std::string text)
//----------------------------------------------------------------------------------------------------------------------
{
	SDL_Surface     *tempSurface;
	SDL_Texture     *tempTexture;
	SDL_Rect        tempRect;

	setColor (r, g, b, a);
	tempSurface = write (posX, posY, text);
	if (nullptr == tempSurface)
	{
		funcOutput (-1, int_getString ("%s", "Unable to create temp surface when rendering text [ %s ].", text.c_str()));
		return;
	}
	tempTexture = SDL_CreateTextureFromSurface (whichRenderer, tempSurface);
	if (nullptr == tempTexture)
	{
		funcOutput (-1, int_getString ("%s", "Unable to create temp texture when rendering console."));
		return;
	}
	SDL_RenderCopy (whichRenderer, tempTexture, nullptr, &pos);
	SDL_DestroyTexture (tempTexture);
}
