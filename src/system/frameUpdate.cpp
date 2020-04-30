#include <io/fileWatch.h>
#include "../../hdr/system/frameUpdate.h"
#include "../main.h"
#include "../../hdr/system/gameEvents.h"

SDL_Event evt;

//----------------------------------------------------------------------------------------------------------------------
//
// Run a frame once
void sys_gameTickRun()
//----------------------------------------------------------------------------------------------------------------------
{
	gam_processGameEventQueue ();
	if (renderer.currentFadeState != FADE_STATE_NONE)
		renderer.updateFade();

	io_checkFileWatcher ();

	while (SDL_PollEvent (&evt) != 0)
	{

		switch (evt.type)
		{
			case SDL_QUIT:
				quitLoop = true;
				break;

		case SDL_KEYDOWN:

			if (evt.key.keysym.sym == SDLK_ESCAPE)
				quitLoop = true;		

			if (evt.key.keysym.sym == SDLK_BACKSPACE)
				sys_addEvent(EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_DELETE_CHAR, 0, "");

			if (evt.key.keysym.sym == SDLK_RETURN)
				sys_addEvent(EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, 0, "");

			if (evt.key.keysym.sym == SDLK_UP)
				console.userBufferNext();

			if (evt.key.keysym.sym == SDLK_DOWN)
				console.userBufferPrevious();

			if (evt.key.keysym.sym == SDLK_TAB)
				console.tabCompletion();

			if (evt.key.keysym.sym == SDLK_PAGEUP)
				console.changeScrollBackOffset(-1);

			if (evt.key.keysym.sym == SDLK_PAGEDOWN)
				console.changeScrollBackOffset(1);

			if (evt.key.keysym.sym == SDLK_F1)
				sys_setNewMode (MODE_SHOW_SPLASH, true);

			if (evt.key.keysym.sym == SDLK_F2)
				sys_setNewMode(MODE_GUI, true);

			if (evt.key.keysym.sym == SDLK_F3)
				sys_setNewMode(MODE_CONSOLE_EDIT, true);

			break;

		case SDL_TEXTINPUT:
			sys_addEvent(EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_CHAR, 0, evt.text.text);
			break;
		}
	}
}