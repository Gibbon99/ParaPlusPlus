#include "../../hdr/system/frameRender.h"
#include "../../hdr/system/startup.h"
#include "../../hdr/io/console.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Prepare the frame for rendering
void sys_prepareFrame()
//----------------------------------------------------------------------------------------------------------------------
{
	static int errorCount = 0;

	if (sys_useRenderTarget())
	{
		if (SDL_SetRenderTarget(sys_getRenderer(), sys_getRenderTarget(sys_getCurrentBackingTexture())) < 0)
		{
			logFile.write(sys_getString("Unable to set render target [ %s ]", SDL_GetError));
			errorCount++;
			if (errorCount > ERROR_COUNT_LIMIT)
				sys_shutdownWithError("Exceeded error count for set render target. Check logfile for details.");
		}
	}

	SDL_SetRenderDrawColor(sys_getRenderer(), 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(sys_getRenderer());
}

//----------------------------------------------------------------------------------------------------------------------
//
// Complete a frame and present to the screen
void sys_completeFrame()
//----------------------------------------------------------------------------------------------------------------------
{
	if (sys_useRenderTarget())
	{
		SDL_SetRenderTarget(sys_getRenderer(), nullptr);
		SDL_RenderCopy(sys_getRenderer(), sys_getRenderTarget(sys_getCurrentBackingTexture()), nullptr, nullptr);
	}

	SDL_RenderPresent(sys_getRenderer());
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a frame once
void sys_renderFrame (double interpolation)
//----------------------------------------------------------------------------------------------------------------------
{
	sys_prepareFrame();

	con_renderConsole();

	sys_completeFrame();
}
