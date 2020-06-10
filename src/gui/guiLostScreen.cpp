#include <gui/guiLanguage.h>
#include <game/hud.h>
#include <game/audio.h>
#include "system/util.h"
#include "game/texture.h"
#include "gui/guiLostScreen.h"

#define NUM_STATIC_IMAGES 9

float       droidRenderX;
float       droidRenderY;
float       staticAnimationDelay;
int         lostScreenShowTime;
std::string textOneString;
std::string textTwoString;
std::string staticKeyname = "static_1";
SDL_Rect    destination;

//----------------------------------------------------------------------------------------------------
//
// Load the static images for the end screen
void gui_loadStaticImages ()
//----------------------------------------------------------------------------------------------------
{
	std::string newFilename;
	std::string newKeyname;

	for (auto i = 1; i != NUM_STATIC_IMAGES + 1; i++)
	{
		newFilename = sys_getString ("%s_%i.bmp", "static", i);
		newKeyname  = sys_getString ("%s_%i", "static", i);

		gam_loadTexture (newFilename, newKeyname);
	}
}

//----------------------------------------------------------------------------------------------------
//
// Load the 999 graphic to show over the static
void gui_loadDroidGraphic ()
//----------------------------------------------------------------------------------------------------
{
	std::string newFileName;
	std::string newKeyName;

	newFileName = "db_999.bmp";
	newKeyName  = "db_droid";

	gam_loadTexture (newFileName, newKeyName);

	textOneString = gui_getString ("lostTransmission");
	textTwoString = gui_getString ("lostTerminated");
}

//----------------------------------------------------------------------------------------------------
//
// Pre rendering of the end screen - prepare all resources
void gui_preRenderLostScreen ()
//----------------------------------------------------------------------------------------------------
{
	gui_loadStaticImages ();
	gui_loadDroidGraphic ();

	droidRenderX = (hiresVirtualWidth - (textures.at ("db_droid").getWidth () / 32)) / 2;
	droidRenderY = ((hiresVirtualHeight - textures.at ("hudNew").getHeight ()) - textures.at ("db_droid").getHeight ()) / 2;
	droidRenderY += 100;

	gam_setHudText ("lostGameOver");

	gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, true, 1, 127, "endTransmission1");
	gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 1, 127, "endTransmission2");

	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_END_LOST_SCREEN, lostScreenShowTime, "");

	sys_setNewMode (MODE_END_LOST_SCREEN, false);
}

//----------------------------------------------------------------------------------------------------
//
// Process the lost screen
void gui_processLostScreen ()
//----------------------------------------------------------------------------------------------------
{
	static float lostScreenDelayCounter = 1.0f;
	static float staticAnimationCounter = 1.0f;
	static int   currentStaticFrame     = 1;

	staticAnimationCounter -= 1.0f * staticAnimationDelay;
	if (staticAnimationCounter < 0.0f)
	{
		staticAnimationCounter = 1.0f;
		currentStaticFrame++;
		if (currentStaticFrame > NUM_STATIC_IMAGES)
			currentStaticFrame = 1;

		staticKeyname = sys_getString ("%s_%i", "static", currentStaticFrame);
	}

	destination.x = 0;
	destination.y = 0;
	destination.w = textures.at (staticKeyname).getWidth () * 2;
	destination.h = textures.at (staticKeyname).getHeight () * 2;
}

//----------------------------------------------------------------------------------------------------
//
// Render the lost screen
void gui_renderLostScreen ()
//----------------------------------------------------------------------------------------------------
{
	double textOneX;
	double textOneY;
	double textTwoX;
	double textTwoY;

	fontClass.use ("guiFont");

	textOneX = (hiresVirtualWidth - fontClass.width (textOneString)) / 2;
	textOneY = textures.at ("hudNew").getHeight () + 50;

	textTwoX = (hiresVirtualWidth - fontClass.width (textTwoString)) / 2;
	textTwoY = hiresVirtualHeight - 50;

	textures.at (staticKeyname).render (&destination);

	databaseSprite.setTintColor (255, 255, 255);
	databaseSprite.render (droidRenderX, droidRenderY, 2.0, static_cast<Uint8>(255));

	fontClass.render (renderer.renderer, textOneX, textOneY, 255, 255, 255, 255, sys_getString ("%s", textOneString.c_str ()));
	fontClass.render (renderer.renderer, textTwoX, textTwoY, 255, 255, 255, 255, sys_getString ("%s", textTwoString.c_str ()));
}


