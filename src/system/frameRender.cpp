#include "../../hdr/system/frameRender.h"
#include "../../hdr/system/startup.h"
#include "../../hdr/io/console.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Prepare the frame for rendering
void sys_prepareFrame()
//----------------------------------------------------------------------------------------------------------------------
{
	renderer.prepareFrame ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Complete a frame and present to the screen
void sys_completeFrame()
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
	sys_prepareFrame();

	switch (currentMode)
	{
		case MODE_CONSOLE_EDIT:
			con_renderConsole();
			break;

		case MODE_SHOW_SPLASH:
			texture.render("splash");
			break;
	}

	sys_completeFrame();
}
