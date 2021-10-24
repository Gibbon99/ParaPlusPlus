#include "gui/guiLanguage.h"
#include "game/transfer.h"
#include "game/transferGame.h"
#include "game/hud.h"
#include "game/score.h"
#include "system/startup.h"
#include "system/util.h"

std::string hudText;
//std::string hudScore;
double      hudTextPosX;
double      hudTextPosY;
double      hudScorePosX;
double      hudScorePosY;

//----------------------------------------------------------------------------------------------------------------------
//
// Set the text for the HUD - gets text from language file - pass in key for language lookup
void gam_setHudText(const std::string &newText)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (currentMode)
	{
		case MODE_TRANSFER_CHOOSE_SIDE:
			hudText = trn_getCountdown ();
			break;

		case MODE_TRANSFER_GAME:
			hudText = trn_getTransferCountdown ();
			break;

		default:
			hudText = gui_getString (newText);
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render the HUD to the screen
void gam_renderHud()
//----------------------------------------------------------------------------------------------------------------------
{
	SDL_Rect destination;

	destination.x = 0;
	destination.y = 0;

	switch (currentMode)
	{
		case MODE_GAME:
		case MODE_GAME_OVER:
			destination.h = textures.at ("hudNew").getHeight () * (static_cast<double>(gameWinHeight) / hiresVirtualHeight);
			destination.w = gameWinWidth;
			break;

		default:
			try
			{
				destination.h = textures.at ("hudNew").getHeight ();
				destination.w = textures.at ("hudNew").getWidth ();
			}
			catch (std::out_of_range &outOfRange)
			{
				return;
			}
			break;
	}

	textures.at ("hudNew").render (&destination);

	fontClass.use ("guiFont");
	fontClass.render (renderer.renderer, hudTextPosX, hudTextPosY, 0, 0, 0, 255, hudText);

	fontClass.render (renderer.renderer, hudScorePosX, hudScorePosY, 0, 0, 0, 255, sys_getString ("%06i", gam_getPrintableScore ()));
}