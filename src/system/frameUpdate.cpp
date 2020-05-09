#include <io/fileWatch.h>
#include <io/keyboard.h>
#include <io/joystick.h>
#include <io/mouse.h>
#include <gui/guiSideview.h>
#include <gui/guiScrollbox.h>
#include "../../hdr/system/frameUpdate.h"

SDL_Event evt;

//----------------------------------------------------------------------------------------------------------------------
//
// Handle system events and populate the keyboard state array
void sys_processSystemEvents ()
//----------------------------------------------------------------------------------------------------------------------
{
	while (SDL_PollEvent (&evt) != 0)
	{
		switch (evt.type)
		{
			case SDL_RENDER_DEVICE_RESET:
			case SDL_RENDER_TARGETS_RESET:
				// Reload all textures
				break;

			case SDL_WINDOWEVENT_FOCUS_LOST:
				break;

			case SDL_WINDOWEVENT_FOCUS_GAINED:
				break;

			case SDL_WINDOWEVENT_SHOWN:
				break;

			case SDL_WINDOWEVENT_CLOSE:
			case SDL_QUIT:
				quitLoop = true;
				break;

			case SDL_JOYAXISMOTION:
				io_joyMovement(evt.jaxis.which, evt.jaxis.axis, evt.jaxis.value);
				break;

			case SDL_MOUSEMOTION:
				io_mouseMovement(evt.motion.x, evt.motion.y, evt.motion.state);
				break;

			case SDL_MOUSEWHEEL:
				if (evt.wheel.y > 0)    // Scroll Up
					io_mouseWheel(KEY_UP);

				if (evt.wheel.y < 0)    // Scroll down
					io_mouseWheel(KEY_DOWN);
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (evt.button.button == SDL_BUTTON_LEFT)
					io_mouseButtonDown();
				break;

			case SDL_KEYDOWN:
				if (currentMode == MODE_CONSOLE_EDIT)
				{
					if (evt.key.keysym.sym == SDLK_ESCAPE)
						quitLoop = true;

					if (evt.key.keysym.sym == SDLK_BACKSPACE)
						sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_DELETE_CHAR, 0, "");

					if (evt.key.keysym.sym == SDLK_RETURN)
						sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, 0, "");

					if (evt.key.keysym.sym == SDLK_UP)
						console.userBufferNext ();

					if (evt.key.keysym.sym == SDLK_DOWN)
						console.userBufferPrevious ();

					if (evt.key.keysym.sym == SDLK_TAB)
						console.tabCompletion ();

					if (evt.key.keysym.sym == SDLK_PAGEUP)
						console.changeScrollBackOffset (-1);

					if (evt.key.keysym.sym == SDLK_PAGEDOWN)
						console.changeScrollBackOffset (1);
				}


				if (evt.key.keysym.sym == SDLK_F1)
					sys_setNewMode (MODE_SHOW_SPLASH, true);

				if (evt.key.keysym.sym == SDLK_F2)
					sys_setNewMode (MODE_GUI, true);

				if (evt.key.keysym.sym == SDLK_F3)
					sys_setNewMode (MODE_CONSOLE_EDIT, true);

				if (evt.key.keysym.sym == SDLK_F4)
					sys_setNewMode(MODE_SIDEWVIEW, true);

				if (evt.key.keysym.sym == SDLK_F5)
					sys_setNewMode(MODE_BRIEFING, true);

				break;

			case SDL_TEXTINPUT:
				if (currentMode == MODE_CONSOLE_EDIT)
					sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_CHAR, 0, evt.text.text);
				break;
		}
	}
	io_processKeyboardState();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Run a frame once
void sys_gameTickRun ()
//----------------------------------------------------------------------------------------------------------------------
{
	gam_processGameEventQueue ();
	if (renderer.currentFadeState != FADE_STATE_NONE)
		renderer.updateFade ();

	io_checkFileWatcher ();

	sys_processSystemEvents ();

	switch (currentMode)
	{
		case MODE_SIDEWVIEW:
			gui_animateStarfield();
			break;

		case MODE_BRIEFING:
			gui_scrollScrollBox ("introScreen.scrollbox");
			break;
	}
}