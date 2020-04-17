#include "../../hdr/system/frameUpdate.h"

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
	}
}