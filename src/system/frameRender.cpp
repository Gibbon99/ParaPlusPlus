#include "../../hdr/system/frameRender.h"
#include "../../hdr/system/startup.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Render a frame once
void sys_renderFrame (double interpolation)
//----------------------------------------------------------------------------------------------------------------------
{
	SDL_SetRenderDrawColor(sys_getRenderer(), 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(sys_getRenderer());

	console.prepare(600, consoleFont.lineHeight);
	for (; console.consoleItr != console.consoleText.rend();++console.consoleItr)
	{
		SDL_RenderCopy(sys_getRenderer(), SDL_CreateTextureFromSurface(sys_getRenderer(), consoleFont.write(logFile, console.posX, console.posY, *console.consoleItr)), nullptr, &consoleFont.pos);
		console.posY -= consoleFont.lineHeight;
	}

	SDL_RenderCopy(sys_getRenderer(), SDL_CreateTextureFromSurface(sys_getRenderer(),
			consoleFont.write(logFile, 1, 10, sys_getString("intoNextFrame : %f Think : %i FPS : %i", percentIntoNextFrame, thinkFPSPrint, fpsPrint))),
	                nullptr, &consoleFont.pos);


	SDL_RenderPresent(sys_getRenderer());
}
