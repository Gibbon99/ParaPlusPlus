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
		if (evt.type == SDL_QUIT)
		{
			quitLoop = true;
		}

		if (evt.type == SDL_KEYDOWN)
		{
			if (evt.key.keysym.sym == SDLK_SPACE)
			{
				sys_createNewScreen(1024, 768, 0);
			}
		}
	}
}