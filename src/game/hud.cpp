
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
	destination.h = texture.height("hudNew");
	destination.w = texture.width("hudNew");

	texture.render ("hudNew", &destination);

	fontClass.use("guiFont");
	fontClass.render(renderer.renderer, hudTextPosX, hudTextPosY, 0, 0, 0, 255, hudText);
}