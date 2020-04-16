
#include "../hdr/main.h"
#include "../hdr/system/startup.h"

paraScript     paraScriptInstance;
paraLogFile    logFile;
paraFileSystem fileSystem;
paraFont       consoleFont;
paraConsole    console;

bool      quitLoop = false;
SDL_Event evt;

//----------------------------------------------------------------------------------------------------------------------
//
// Entry point
int main(int argc, char *argv[])
//----------------------------------------------------------------------------------------------------------------------
{
	sys_startSystems();

	while (!quitLoop)
	{
		console.prepare(600, consoleFont.lineHeight);
		for (; console.consoleItr != console.consoleText.rend(); ++console.consoleItr)
		{
			SDL_BlitSurface(consoleFont.write(logFile, console.posX, console.posY, *console.consoleItr), nullptr, sys_getScreenSurface(), &consoleFont.pos);
			console.posY -= consoleFont.lineHeight;
		}

		//Update the surface
		SDL_UpdateWindowSurface(sys_getWindow());

		//Handle events on queue
		while( SDL_PollEvent( &evt ) != 0 )
		{
			//User requests quit
			if( evt.type == SDL_QUIT )
			{
				quitLoop = true;
			}
		}
	}

	//Destroy window
	SDL_DestroyWindow(sys_getWindow());

	//Quit SDL subsystems
	SDL_Quit();

	sys_shutdown();

	return 0;
}
