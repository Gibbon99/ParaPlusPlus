#include "gui/guiRender.h"
#include "system/frameRender.h"
#include "io/console.h"

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

		case MODE_GUI:
			gui_renderGUI ();
			break;
	}
	sys_completeFrame ();
}
