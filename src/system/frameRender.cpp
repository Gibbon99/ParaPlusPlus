#include <gui/guiLostScreen.h>
#include <system/util.h>
#include <classes/paraStarfield.h>
#include <system/cpPhysicsDebug.h>
#include "gui/guiScrollbox.h"
#include "system/startup.h"
#include "game/shipDecks.h"
#include "game/healing.h"
#include "game/player.h"
#include "gui/guiDeckView.h"
#include "game/droids.h"
#include "classes/paraBullet.h"
#include "game/particles.h"
#include "game/lightMaps.h"
#include "game/transferRender.h"
#include "gui/guiRender.h"
#include "system/frameRender.h"
#include "io/console.h"
#include "gui/guiSideview.h"
#include "game/hud.h"
#include "game/doors.h"

unsigned long g_debugDroidCount          = 0;
bool          d_showPerfStats            = false;
int           d_showPathIndex            = 0;
bool          d_showWaypoints            = false;
int           databaseDroidRenderYOffset = 30;          // From script

Uint8         r, g, b, a;
SDL_BlendMode tempMode;

//----------------------------------------------------------------------------------------------------------------------
//
// Prepare the frame for rendering
void sys_prepareFrame()
//----------------------------------------------------------------------------------------------------------------------
{
	//
	// SDL2_gfx changes the blend mode and draw color
	// as part of its rendering - remember, so we can change it back
	//
	SDL_GetRenderDrawColor (renderer.renderer, &r, &g, &b, &a);
	SDL_GetRenderDrawBlendMode (renderer.renderer, &tempMode);

	renderer.prepareFrame ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Complete a frame and present to the screen
void sys_completeFrame()
//----------------------------------------------------------------------------------------------------------------------
{
	SDL_SetRenderDrawColor (renderer.renderer, r, g, b, a);
	SDL_SetRenderDrawBlendMode (renderer.renderer, tempMode);

	renderer.presentFrame ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a frame once
void sys_renderFrame(double interpolation)
//----------------------------------------------------------------------------------------------------------------------
{
	sys_prepareFrame ();

	if (renderer.currentFadeState == FADE_STATE_OFF)
	{
		sys_completeFrame ();
		return;
	}

	int  halfScreen = hiresVirtualWidth / 2;
	int  frameWidth = databaseSprite.getFrameWidth ();
	int  halfPoint  = (halfScreen - frameWidth) / 2;
	int  finalPoint = halfPoint + halfScreen;

	int droidPosX{0};
	int droidPosY{0};

	switch (currentMode)
	{
		case MODE_CONSOLE_EDIT:
		case MODE_CONSOLE_INIT:
			con_renderConsole ();
			break;

		case MODE_SHOW_SPLASH:
			textures.at ("splash").render ();
			break;

		case MODE_GUI_MAINMENU:
		case MODE_GUI_TERMINAL:
		case MODE_GUI_TUT_TRANSFER_START:
		case MODE_GUI_TUT_TRANSFER_GAME:
		case MODE_GUI_TUT_LIFTS:
		case MODE_GUI_TUT_TERMINALS:
		case MODE_GUI_TUT_HEALING:
		case MODE_GUI_TUT_TIPS:
		case MODE_GUI_HIGHSCORE_DISPLAY:
		case MODE_GUI_HIGHSCORE_ENTRY:
			gui_renderGUI ();
			break;

		case MODE_GUI_DATABASE:
			gui_renderScrollbox ("databaseScreen.scrollbox", interpolation);
			databaseSprite.setTintColor (255, 255, 255);
			databaseSprite.render (finalPoint, ((((hiresVirtualHeight - (hiresVirtualHeight / 2) - databaseSprite.getFrameHeight ())) / 2) + (textures.at ("hudNew").getHeight () + databaseDroidRenderYOffset)), 2.0, static_cast<Uint8>(255));
			gui_renderGUI ();
			break;

		case MODE_GUI_DECKVIEW:
			gam_renderHealingFrames ();
			gui_renderTerminalDeck ();
			gui_renderGUI ();
			break;

		case MODE_GUI_SHIPVIEW:
			gui_renderSideView ();
			gui_renderGUI ();
			break;

		case MODE_GUI_LIFTVIEW:
			gui_renderSideView ();
			break;

		case MODE_GUI_INTROSCROLL:
			gui_renderScrollbox ("introScreen.scrollbox", interpolation);
			break;

		case MODE_GUI_WON_SCREEN:
			gui_renderScrollbox ("wonScreen.scrollbox", interpolation);
			break;

		case MODE_TRANSFER_SCREEN_ONE:
		case MODE_TRANSFER_SCREEN_TWO:
			droidPosX = (hiresVirtualWidth - databaseSprite.getFrameWidth ()) / 2;
			droidPosY = (((hiresVirtualWidth - databaseSprite.getFrameHeight ()) / 2) + textures.at ("hudNew").getHeight ()) - databaseSprite.getFrameHeight ();
			databaseSprite.render (static_cast<float>(droidPosX), static_cast<float>(droidPosY), 2.0, static_cast<Uint8>(64));
			gui_renderGUI ();
			break;

		case MODE_TRANSFER_CHOOSE_SIDE:
			trn_renderTransferGame ();
			break;

		case MODE_TRANSFER_GAME:
			trn_renderTransferGame ();
			break;

		case MODE_TRANSFER_DEADLOCK:
			trn_renderTransferGame ();
			break;

		case MODE_TRANSFER_RESULT:
			trn_renderTransferGame ();
			break;

		case MODE_GAME_OVER:
			gam_renderVisibleScreen (interpolation);
			playerDroid.sprite.render (gameWinWidth / 2, gameWinHeight / 2, 1.0, static_cast<Uint8>(255));

			gam_renderParticles ();
			break;

		case MODE_GAME:
			gam_renderVisibleScreen (interpolation);
			gam_renderHealingFrames ();
			gam_renderDoorFrames ();

			if (playerDroid.getInTransferMode ())
				playerDroid.sprite.setTintColor (0, 0, 255);
			else if (playerDroid.getLowInfluenceTimeFlag ())
				playerDroid.sprite.setTintColor (static_cast<Uint8>(playerDroid.getInfluenceFadeValue ()), static_cast<Uint8>(playerDroid.getInfluenceFadeValue ()), 255);
			else
				playerDroid.sprite.setTintColor (255, 255, 255);

			playerDroid.sprite.render (gameWinWidth / 2, gameWinHeight / 2, 1.0, static_cast<Uint8>(255));

			gam_renderDroids ();

			gam_renderLightmaps ();
			gam_renderBullets ();
			gam_renderParticles ();

//			gam_debugShowPlayerTrail ();

			if (d_showWaypoints)
				gam_showWayPoints (gam_getCurrentDeckName ());

			if (d_showInfluenceMap)
				gam_debugInfluenceMap ();

			if (d_showPhysics)
				d_drawPhysicsDebug ();

//			if (d_showNodeArrays)
//				gam_AStarDebugNodes (d_showPathIndex);

//			if (d_showAStarPath)
//				gam_AStarDebugAllPaths();

			if (gui.getCurrentDialogbox () != NO_DIALOG_BOX)
				gui_renderActiveDialogbox ();

			for (auto itr: shipdecks.at (gam_getCurrentDeckName ()).droid)
			{
//				gam_debugShowTarget (itr);
//				itr.ai2.aStar.debugNodes();
//				itr.ai2.aStar.debugWayPoints();
			}
			break;

		case MODE_END_LOST_SCREEN:
			gui_renderLostScreen ();
			break;
	}

	try
	{
		if (doScreenEffect)
			textures.at ("screen").render ();
	}
	catch (std::out_of_range &outOfRange)
	{
	}

	if ((currentMode != MODE_CONSOLE_EDIT) && (currentMode != MODE_CONSOLE_INIT) && (currentMode != MODE_SHOW_SPLASH))
	{
		gam_renderHud ();

		if (d_showPerfStats)
		{
			fontClass.use ("guiFont");
			fontClass.render (renderer.renderer, 1, 550, 200, 200, 200, 255, sys_getString ("Droids : %i Think : %i FPS : %i intoNextFrame : %f ", g_debugDroidCount, thinkFPSPrint, fpsPrint, percentIntoNextFrame));
		}
	}

	sys_completeFrame ();
}

