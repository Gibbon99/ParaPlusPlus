#include <gui/guiScrollbox.h>
#include <system/startup.h>
#include <game/shipDecks.h>
#include <game/healing.h>
#include <game/player.h>
#include <gui/guiDeckView.h>
#include <game/pathFind.h>
#include <game/droids.h>
#include "gui/guiRender.h"
#include "system/frameRender.h"
#include "io/console.h"
#include "gui/guiSideview.h"
#include "game/hud.h"
#include "game/doors.h"

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
	int frameWidth = databaseSprite.getFrameWidth();
	int halfPoint = (halfScreen - frameWidth) / 2;
	int finalPoint = halfPoint + halfScreen;

	switch (currentMode)
	{
		case MODE_CONSOLE_EDIT:
		case MODE_CONSOLE_INIT:
			con_renderConsole ();
			break;

		case MODE_SHOW_SPLASH:
			textures.at("splash").render();
			break;

		case MODE_GUI_MAINMENU:
			gui_renderGUI ();
			break;

		case MODE_GUI_TERMINAL:
			gui_renderGUI ();
			break;

		case MODE_GUI_DATABASE:
			gui_renderScrollbox ("databaseScreen.scrollbox", interpolation);
			databaseSprite.render (finalPoint,
					((((hiresVirtualHeight - (hiresVirtualHeight / 2) - databaseSprite.getFrameHeight())) / 2) + (textures.at("hudNew").getHeight())), 2.0);

			gui_renderGUI ();
			break;

		case MODE_GUI_DECKVIEW:
			gam_renderHealingFrames (gam_getCurrentDeckName());
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

		case MODE_GAME:
			gam_renderVisibleScreen(interpolation);
			gam_renderHealingFrames (gam_getCurrentDeckName());
			gam_renderDoorFrames ();

			playerDroid.sprite.render (gameWinWidth / 2, gameWinHeight / 2,  1.0);

			testCircle.sprite.render(testCircle.worldPosInPixels.x, testCircle.worldPosInPixels.y, 0.5);

			renderTestDroid();

			if (d_showPhysics)
				sys_getPhysicsWorld ()->DebugDraw();

			gam_AStarDebugWayPoints (testDroid.aStarIndex);
			break;

	}

	if (doScreenEffect)
		textures.at("screen").render();

	if ((currentMode != MODE_CONSOLE_EDIT) && (currentMode != MODE_CONSOLE_INIT))
		gam_renderHud ();

	sys_completeFrame ();
}

