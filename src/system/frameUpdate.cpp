#include "../../hdr/system/frameUpdate.h"
#include "../../hdr/system/startup.h"

SDL_Event evt;

//----------------------------------------------------------------------------------------------------------------------
//
// Run a frame once
void sys_gameTickRun()
//----------------------------------------------------------------------------------------------------------------------
{
	while (SDL_PollEvent(&evt) != 0)
	{
//User requests quit
		switch (evt.type)
		{
		case SDL_QUIT:
			quitLoop = true;
			break;

		case SDL_KEYDOWN:
//			if (evt.key.keysym.sym == SDLK_R)
	//			sys_createNewScreen(1024, 768, 0);

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

			break;

		case SDL_TEXTINPUT:
			sys_addEvent(EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_CHAR, 0, evt.text.text);
			break;
		}
	}
}