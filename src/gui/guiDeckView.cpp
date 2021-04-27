#include <game/shipDecks.h>
#include <system/startup.h>
#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include <game/player.h>
#include <system/util.h>
#include "gui/guiDeckView.h"

double      tileScaleX;
double      tileScaleY;
SDL_TimerID blinkIconTimerID = 0;
bool        showIcon = true;

//-----------------------------------------------------------------------------
//
// Switch state for blinking player icon
Uint32 gui_blinkPlayerIcon (Uint32 interval, void *param)
//-----------------------------------------------------------------------------
{
	showIcon = !showIcon;

	return interval;
}

//-----------------------------------------------------------------------------
//
// Create and start the timer for player blink
void gui_startBlinkTimer(int blinkRateInMS)
//-----------------------------------------------------------------------------
{
	blinkIconTimerID = SDL_AddTimer (blinkRateInMS, gui_blinkPlayerIcon, nullptr);
	if (0 == blinkIconTimerID)
		return;
}

//-----------------------------------------------------------------------------
//
// Stop the blink timer
void gui_stopBlinkTimer()
//-----------------------------------------------------------------------------
{
	if (blinkIconTimerID != 0)
		SDL_RemoveTimer(blinkIconTimerID);
}

//-----------------------------------------------------------------------------
//
// Draw the player indicator on the deckview map
void gui_renderPlayerLocation ()
//-----------------------------------------------------------------------------
{
	int tileLocationX, tileLocationY;

	if (!showIcon)
		return;

	tileLocationX = playerDroid.previousWorldPosInPixels.x * tileScaleX;
	tileLocationY = playerDroid.previousWorldPosInPixels.y * tileScaleY;

	filledCircleRGBA (renderer.renderer, tileLocationX, tileLocationY, 12, 255, 255, 255, 255);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Show the entire current deck from the terminal
void gui_renderTerminalDeck ()
//----------------------------------------------------------------------------------------------------------------------
{
	Uint32   format;
	int      width;
	int      height;
	double   deckViewTextPosX;
	double   deckViewTextPosY;
	SDL_Rect destRect;
	SDL_Rect sourceRect;
//	std::string levelNameCache;
	Uint8         r, g, b, a;
	SDL_BlendMode tempMode;

	//
	// SDL2_gfx changes the blend mode and draw color
	// as part of its rendering - remember so we can change it back
	//
//	SDL_GetRenderDrawColor (renderer.renderer, &r, &g, &b, &a);
//	SDL_GetRenderDrawBlendMode (renderer.renderer, &tempMode);

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

//	SDL_SetRenderDrawColor (renderer.renderer, r, g, b, a);
//	SDL_SetRenderDrawBlendMode (renderer.renderer, tempMode);
}