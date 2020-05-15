#include <game/shipDecks.h>
#include <system/startup.h>
#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include <game/player.h>
#include <system/util.h>
#include "gui/guiDeckView.h"


//-----------------------------------------------------------------------------
//
// Draw the player indicator on the deckview map
void gui_renderPlayerLocation (float smallTleSize)
//-----------------------------------------------------------------------------
{
	// TODO - get scaling of position right

	int tileLocationX, tileLocationY;

	tileLocationX = playerDroid.previousWorldPosInPixels.x * smallTleSize;
	tileLocationY = playerDroid.previousWorldPosInPixels.y * smallTleSize;

//	tileLocationX *= smallTleSize;
//	tileLocationY *= smallTleSize;

//	tileLocationY += textures.at ("hudNew").getHeight ();
//	tileLocationY += smallTleSize / 2;
//	tileLocationX += smallTleSize / 2;

//	if (tim_getIconState())
	filledCircleRGBA (renderer.renderer, tileLocationX, tileLocationY, 12, 255, 255, 255, 255);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Show the entire current deck from the terminal
void gui_renderTerminalDeck ()
//----------------------------------------------------------------------------------------------------------------------
{
	double      tileScale;
	std::string levelNameCache;
	Uint32      format;
	int         width;
	int         height;
	SDL_Rect    destRect;
	SDL_Rect    sourceRect;
	double      deckViewTextPosX;
	double      deckViewTextPosY;

	SDL_QueryTexture (gam_getPlayfieldTexture (), &format, nullptr, &width, &height);
	if (width > hiresVirtualWidth)
	{
		tileScale = static_cast<double>(hiresVirtualWidth) / static_cast<double>(width);
	}
	else
		tileScale = 1.0;

	double finalWidth  = static_cast<double>(width) * tileScale;
	double finalHeight = static_cast<double>(height) * tileScale;

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

	gui_renderPlayerLocation (tileScale);
}