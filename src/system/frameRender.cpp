#include "../../hdr/system/frameRender.h"
#include "../../hdr/system/startup.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Render a frame once
void sys_renderFrame (double interpolation)
//----------------------------------------------------------------------------------------------------------------------
{
	SDL_RenderClear(sys_getRenderer());

	SDL_FillRect(sys_getScreenSurface(), NULL, SDL_MapRGB(sys_getScreenSurface()->format, 0, 0, 0));


	console.prepare(600, consoleFont.lineHeight);
	for (; console.consoleItr != console.consoleText.rend();++console.consoleItr)
	{
		SDL_BlitSurface(consoleFont.write(logFile, console.posX, console.posY, *console.consoleItr), nullptr, sys_getScreenSurface(), &consoleFont.pos);
		console.posY -= consoleFont.lineHeight;
	}
}
