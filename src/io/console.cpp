#include "../../hdr/io/console.h"
#include "../../hdr/system/startup.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Render the console to the screen
void con_renderConsole()
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_Surface *tempSurface  = nullptr;
	PARA_Texture *tempTexture  = nullptr;

	console.prepare(logicalWinHeight, consoleFont.lineHeight);
	for (; console.consoleItr != console.consoleText.rend(); ++console.consoleItr)
	{
		tempSurface = consoleFont.write(logFile, console.posX, console.posY, *console.consoleItr);  // Surface is freed within console class
		tempTexture = SDL_CreateTextureFromSurface(sys_getRenderer(), tempSurface);

		SDL_RenderCopy(sys_getRenderer(), tempTexture, nullptr, &consoleFont.pos);

		SDL_DestroyTexture(tempTexture);

		console.posY -= consoleFont.lineHeight;
	}

	tempSurface = consoleFont.write(logFile, 1, 10,
	                                sys_getString("intoNextFrame : %f Think : %i FPS : %i", percentIntoNextFrame, thinkFPSPrint, fpsPrint));
	tempTexture = SDL_CreateTextureFromSurface(sys_getRenderer(), tempSurface);
	SDL_RenderCopy(sys_getRenderer(), tempTexture, nullptr, &consoleFont.pos);
	SDL_DestroyTexture(tempTexture);
}