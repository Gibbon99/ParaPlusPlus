#include <game/shipDecks.h>
#include <system/startup.h>
#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include <game/player.h>
#include <system/util.h>
#include "gui/guiDeckView.h"

double      tileScaleX;
double      tileScaleY;

//-----------------------------------------------------------------------------
//
// Draw the player indicator on the deckview map
void gui_renderPlayerLocation ()
//-----------------------------------------------------------------------------
{
	int tileLocationX, tileLocationY;

	tileLocationX = playerDroid.previousWorldPosInPixels.x * tileScaleX;
	tileLocationY = playerDroid.previousWorldPosInPixels.y * tileScaleY;

//	if (tim_getIconState())
		filledCircleRGBA (renderer.renderer, tileLocationX, tileLocationY, 12, 255, 255, 255, 255);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Show the entire current deck from the terminal
void gui_renderTerminalDeck ()
//----------------------------------------------------------------------------------------------------------------------
{
	Uint32      format;
	int         width;
	int         height;
	double      deckViewTextPosX;
	double      deckViewTextPosY;
	SDL_Rect    destRect;
	SDL_Rect    sourceRect;
//	std::string levelNameCache;

	SDL_QueryTexture (gam_getPlayfieldTexture (), &format, nullptr, &width, &height);
	if (width > hiresVirtualWidth)
	{
		tileScaleX = static_cast<double>(hiresVirtualWidth) / static_cast<double>(width);
	}
	else
		tileScaleX = 1.0;

	if (height > hiresVirtualHeight)
		tileScaleY = hiresVirtualHeight / static_cast<double>(height);
	else
		tileScaleY = 1.0;

	double finalWidth  = static_cast<double>(width) * tileScaleX;
	double finalHeight = static_cast<double>(height) * tileScaleY;

	destRect.x = static_cast<int>((hiresVirtualWidth - finalWidth)) / 2;
	destRect.y = static_cast<int>((hiresVirtualHeight - finalHeight)) / 2;
	destRect.w = static_cast<int>(finalWidth);
	destRect.h = static_cast<int>(finalHeight);

	sourceRect.x = 0;
	sourceRect.y = 0;
	sourceRect.w = width;
	sourceRect.h = height;

	SDL_RenderCopy (renderer.renderer, gam_getPlayfieldTexture (), &sourceRect, &destRect);

	fontClass.use ("guiFont");

	deckViewTextPosX = 5;
	deckViewTextPosY = renderer.renderHeight () - (fontClass.height ());
	fontClass.render (renderer.renderer, deckViewTextPosX, deckViewTextPosY, 255, 255, 255, 255, sys_getString ("Deck [ %s ]", gam_returnLevelNameFromDeck (currentDeckNumber).c_str ()));

	gui_renderPlayerLocation ();
}