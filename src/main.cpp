
#include "../hdr/main.h"
#include "../hdr/system/startup.h"

paraScript     paraScriptInstance;
paraLogFile    logFile;
paraFileSystem fileSystem;
paraFont       fontOne;
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
		SDL_BlitSurface(fontOne.write(logFile, 100, 100, "Test text for Para++"), nullptr, sys_getScreenSurface(), &fontOne.pos);

		int consoleX, consoleY = 0;

		consoleY = 600 - fontOne.lineHeight;

		auto consoleItr = console.consoleText.rbegin();
		for (; consoleItr != console.consoleText.rend(); ++consoleItr)
		{
			SDL_BlitSurface(fontOne.write(logFile, consoleX, consoleY, *consoleItr), nullptr, sys_getScreenSurface(), &fontOne.pos);
			consoleY -= fontOne.lineHeight;
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
