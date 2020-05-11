#include <gui/guiScrollbox.h>
#include <system/startup.h>
#include "gui/guiRender.h"
#include "system/frameRender.h"
#include "io/console.h"
#include "gui/guiSideview.h"
#include "game/hud.h"

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
	finalPoint -= frameWidth / 2;

	switch (currentMode)
	{
		case MODE_CONSOLE_EDIT:
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
					((((hiresVirtualHeight - (hiresVirtualHeight / 2) - databaseSprite.getFrameHeight())) / 2) + (textures.at("hudNew").getHeight()) / 2), 2.0);

			gui_renderGUI ();
			break;

		case MODE_GUI_DECKVIEW:
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
	}

	if (doScreenEffect)
		textures.at("screen").render();

	gam_renderHud ();
	sys_completeFrame ();
}

