#include "../../hdr/classes/paraFont.h"
#include "../../hdr/classes/paraLogFile.h"
#include "../../hdr/system/util.h"
#include "../../hdr/io/logFile.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Start the TTF if required, load a font file with a size
bool paraFont::load(int fontSize, const std::string &fileName)
//----------------------------------------------------------------------------------------------------------------------
{
	//paraFont::logFileOut = outFile;

	if (!TTF_WasInit())
	{
		if (-1 == TTF_Init())
		{
			paraFont::fontSystemAvailable = false;
			log_addEvent(sys_getString("Unable to start font system [ %s ]", TTF_GetError()));
			return false;
		}
		SDL_version compile_version, *link_version;

		TTF_VERSION(&compile_version);
		log_addEvent(sys_getString("Compiled with SDL_ttf version: %d.%d.%d",
		                            compile_version.major,
		                            compile_version.minor,
		                            compile_version.patch));

		link_version = const_cast<SDL_version *>(TTF_Linked_Version());
		log_addEvent(sys_getString("Running with SDL_ttf version: %d.%d.%d",
		                            link_version->major,
		                            link_version->minor,
		                            link_version->patch));
	}

	paraFont::fontHandle = TTF_OpenFont(fileName.c_str(), fontSize);
	if (nullptr == paraFont::fontHandle)
	{
		log_addEvent(sys_getString("Unable to open font [ %s ] - ", fileName.c_str(), TTF_GetError()));
		paraFont::available = false;
		return false;
	}
	paraFont::available = true;
	paraFont::lineHeight = TTF_FontLineSkip(paraFont::fontHandle);

	log_addEvent(sys_getString("Loaded font file [ %s ]", fileName.c_str()));

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
		log_addEvent(sys_getString("Unable to render font to surface [ %s ]", TTF_GetError()));
		return nullptr;
	}
	paraFont::pos.x = X;
	paraFont::pos.y = Y;
	paraFont::pos.w = paraFont::surface->w;
	paraFont::pos.h = paraFont::surface->h;

	return paraFont::surface;
}