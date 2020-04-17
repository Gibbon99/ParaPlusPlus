#include "../../hdr/system/frameRender.h"
#include "../../hdr/system/startup.h"
#include "../../hdr/io/console.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Render a frame once
void sys_renderFrame (double interpolation)
//----------------------------------------------------------------------------------------------------------------------
{
	SDL_SetRenderDrawColor(sys_getRenderer(), 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(sys_getRenderer());

	con_renderConsole();

	SDL_RenderPresent(sys_getRenderer());
}
