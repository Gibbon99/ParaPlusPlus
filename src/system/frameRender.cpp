#include <gui/guiLostScreen.h>
#include <gui/guiHighScore.h>
#include <system/gameEvents.h>
#include <system/util.h>
#include "gui/guiScrollbox.h"
#include "system/startup.h"
#include "game/shipDecks.h"
#include "game/healing.h"
#include "game/player.h"
#include "gui/guiDeckView.h"
#include "game/pathFind.h"
#include "game/droids.h"
#include "game/bullet.h"
#include "game/particles.h"
#include "game/lightMaps.h"
#include "game/transferRender.h"
#include "gui/guiRender.h"
#include "system/frameRender.h"
#include "io/console.h"
#include "gui/guiSideview.h"
#include "game/hud.h"
#include "game/doors.h"

unsigned long g_debugDroidCount = 0;

//----------------------------------------------------------------------------------------------------------------------
//
// Prepare the frame for rendering
void sys_prepareFrame ()
//----------------------------------------------------------------------------------------------------------------------
{
	renderer.prepareFrame ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Complete a frame and present to the screen
void sys_completeFrame ()
//----------------------------------------------------------------------------------------------------------------------
{
	renderer.presentFrame ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a frame once
void sys_renderFrame (double interpolation)
//----------------------------------------------------------------------------------------------------------------------
{
	sys_prepareFrame ();

	if (renderer.currentFadeState == FADE_STATE_OFF)
	{
		sys_completeFrame ();
		return;
	}

	int halfScreen = hiresVirtualWidth / 2;
	int frameWidth = databaseSprite.getFrameWidth ();
	int halfPoint  = (halfScreen - frameWidth) / 2;
	int finalPoint = halfPoint + halfScreen;

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
			gui_renderGUI ();
			break;

		case MODE_GUI_TERMINAL:
			gui_renderGUI ();
			break;

		case MODE_GUI_TUT_TRANSFER_START:
		case MODE_GUI_TUT_TRANSFER_GAME:
		case MODE_GUI_TUT_LIFTS:
		case MODE_GUI_TUT_TERMINALS:
		case MODE_GUI_TUT_HEALING:
		case MODE_GUI_TUT_TIPS:
			gui_renderGUI ();
			break;

		case MODE_GUI_DATABASE:
			gui_renderScrollbox ("databaseScreen.scrollbox", interpolation);
			databaseSprite.setTintColor (255, 255, 255);
			databaseSprite.render (finalPoint, ((((hiresVirtualHeight - (hiresVirtualHeight / 2) - databaseSprite.getFrameHeight ())) / 2) + (textures.at ("hudNew").getHeight ())), 2.0, static_cast<Uint8>(255));
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

		case MODE_GUI_HIGHSCORE_SCREEN:
			gui_renderHighScoreScreen();
			break;

		case MODE_TRANSFER_SCREEN_ONE:
			databaseSprite.render (
					(hiresVirtualWidth - databaseSprite.getFrameWidth ()) / 2,
					(((hiresVirtualWidth - databaseSprite.getFrameHeight ()) / 2) + textures.at ("hudNew").getHeight ()) - databaseSprite.getFrameHeight (), 2.0, static_cast<Uint8>(64));
			gui_renderGUI ();
			break;

		case MODE_TRANSFER_SCREEN_TWO:
			databaseSprite.render (
					(hiresVirtualWidth - databaseSprite.getFrameWidth ()) / 2,
					(((hiresVirtualWidth - databaseSprite.getFrameHeight ()) / 2) + textures.at ("hudNew").getHeight ()) - databaseSprite.getFrameHeight (), 2.0, static_cast<Uint8>(64));
			gui_renderGUI ();
			break;

		case MODE_TRANSFER_CHOOSE_SIDE:
			trn_renderTransferGame ();
			break;

		case MODE_TRANSFER_GAME:
			trn_renderTransferGame();
			break;

		case MODE_TRANSFER_DEADLOCK:
			trn_renderTransferGame();
			break;

		case MODE_GAME_OVER:
			gam_renderVisibleScreen (interpolation);
			playerDroid.sprite.render (gameWinWidth / 2, gameWinHeight / 2, 1.0, static_cast<Uint8>(255));

//			gam_renderDroids (gam_getCurrentDeckName ());
			gam_renderParticles ();
			break;

		case MODE_GAME:
			gam_renderVisibleScreen (interpolation);
			gam_renderHealingFrames ();
			gam_renderDoorFrames ();

			if (playerDroid.inTransferMode)
				playerDroid.sprite.setTintColor (0, 0, 255);

			else if (playerDroid.lowInfluenceTimeleft)
				playerDroid.sprite.setTintColor (static_cast<Uint8>(playerDroid.influenceFade), static_cast<Uint8>(playerDroid.influenceFade), 255);

			else
				playerDroid.sprite.setTintColor (255, 255, 255);

			playerDroid.sprite.render (gameWinWidth / 2, gameWinHeight / 2, 1.0, static_cast<Uint8>(255));

			gam_renderDroids (gam_getCurrentDeckName ());

			gam_renderLightmaps ();
			gam_renderBullets ();
			gam_renderParticles ();

//			gam_showWayPoints (gam_getCurrentDeckName());
			if (d_showInfluenceMap)
				gam_debugInfluenceMap ();

			if (d_showPhysics)
				sys_getPhysicsWorld ()->DebugDraw ();

			if (d_showNodeArrays)
				gam_AStarDebugNodes (0);

			if (d_showAStarPath)
				gam_AStarDebugWayPoints (0);

			if (gui.getCurrentDialogbox() != NO_DIALOG_BOX)
				gui_renderActiveDialogbox ();

			break;

		case MODE_END_LOST_SCREEN:
			gui_renderLostScreen();
			break;
	}

	if (doScreenEffect)
		textures.at ("screen").render ();

	if ((currentMode != MODE_CONSOLE_EDIT) && (currentMode != MODE_CONSOLE_INIT))
		gam_renderHud ();

	fontClass.use ("guiFont");
	fontClass.render (renderer.renderer, 1, 550, 200, 200, 200, 255, sys_getString ("Droids : %i Think : %i FPS : %i intoNextFrame : %f ", g_debugDroidCount, thinkFPSPrint, fpsPrint, percentIntoNextFrame));

	sys_completeFrame ();
}

