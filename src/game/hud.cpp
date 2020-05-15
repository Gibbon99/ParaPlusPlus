#include <system/startup.h>
#include "game/hud.h"

std::string hudText;
std::string hudScore;
double hudTextPosX;
double hudTextPosY;

//----------------------------------------------------------------------------------------------------------------------
//
// Set the text for the HUD
void gam_setHudText(const std::string &newText)
//----------------------------------------------------------------------------------------------------------------------
{
	hudText = newText;
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
	if (currentMode != MODE_GAME)
	{
		try
		{
			destination.h = textures.at ("hudNew").getHeight ();
			destination.w = textures.at ("hudNew").getWidth ();
		}
		catch ( std::out_of_range outOfRange)
			{
			return;
			}
	}
	else
	{
		destination.h = textures.at ("hudNew").getHeight() * (static_cast<double>(gameWinHeight) / hiresVirtualHeight);
		destination.w = gameWinWidth;
	}

	textures.at("hudNew").render(&destination);

	fontClass.use("guiFont");
	fontClass.render(renderer.renderer, hudTextPosX, hudTextPosY, 0, 0, 0, 255, hudText);
}