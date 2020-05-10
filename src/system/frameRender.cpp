#include <gui/guiScrollbox.h>
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

	switch (currentMode)
	{
		case MODE_CONSOLE_EDIT:
			con_renderConsole ();
			break;

		case MODE_SHOW_SPLASH:
			texture.render ("splash");
			break;

		case MODE_GUI_MAINMENU:
			gam_renderHud ();
			gui_renderGUI ();
			break;

		case MODE_GUI_TERMINAL:
			gam_renderHud ();
			gui_renderGUI ();
			break;

		case MODE_GUI_DATABASE:
			gam_renderHud ();
			gui_renderScrollbox ("databaseScreen.scrollbox", interpolation);
			gui_renderGUI ();
			break;

		case MODE_GUI_DECKVIEW:
			gam_renderHud ();
			gui_renderGUI ();
			break;

		case MODE_GUI_SHIPVIEW:
			gui_renderSideView ();
			gam_renderHud ();
			gui_renderGUI ();
			break;

		case MODE_GUI_LIFTVIEW:
			gui_renderSideView ();
			gam_renderHud ();
			break;

		case MODE_GUI_INTROSCROLL:
			gam_renderHud ();
			gui_renderScrollbox ("introScreen.scrollbox", interpolation);
			break;
	}
	sys_completeFrame ();
}

