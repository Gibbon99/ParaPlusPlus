
#include "../hdr/main.h"
#include "../hdr/system/startup.h"
#include "../hdr/system/frameUpdate.h"
#include "../hdr/system/frameRender.h"

paraScript     paraScriptInstance;
paraLogFile    logFile;
paraFileSystem fileSystem;
paraFont       consoleFont;
paraConsole    console;

#define TICKS_PER_SECOND 30.0f
#define MAX_FRAMESKIP 5

bool   quitLoop          = false;
Uint32 maxNumUpdateLoops = 0;
Uint32 fps               = 0;
Uint32 fpsPrint          = 0;
Uint32 thinkFPS          = 0;
Uint32 thinkFPSPrint     = 0;

//----------------------------------------------------------------------------------------------------------------------
//
// Timer callback to track the Update and Render FPS
Uint32 fpsCallback(Uint32 interval, void *param)
//----------------------------------------------------------------------------------------------------------------------
{
	thinkFPSPrint = thinkFPS;
	fpsPrint      = fps;
	thinkFPS      = 0;
	fps           = 0;
	return (interval);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Entry point - fixed update time step with unrestricted rendering
int main(int argc, char *argv[])
//----------------------------------------------------------------------------------------------------------------------
{
	double msPerUpdate          = 1000.0f / TICKS_PER_SECOND;
	Uint32 previousTime         = PARA_GetTicks();
	double timeLag              = 0.0f;
	double percentIntoNextFrame = 0.0f;

	sys_startSystems();

	SDL_TimerID myTimer = SDL_AddTimer(1000, fpsCallback, nullptr);

	while (!quitLoop)
	{
		Uint32 currentTime = PARA_GetTicks();
		Uint32 elapsedTime = currentTime - previousTime;
		previousTime             = currentTime;
		timeLag += elapsedTime;r
		maxNumUpdateLoops        = 0;

		while (timeLag >= msPerUpdate && maxNumUpdateLoops < MAX_FRAMESKIP)
		{
			sys_gameTickRun();
			timeLag -= msPerUpdate;
			thinkFPS++;
			maxNumUpdateLoops++;
		}
		percentIntoNextFrame     = timeLag / msPerUpdate;
		if (percentIntoNextFrame > 1.0f)
			percentIntoNextFrame = 1.0f;

		sys_renderFrame(percentIntoNextFrame);

		SDL_BlitSurface(consoleFont.write(logFile, 1, 10,
		                                  sys_getString("intoNextFrame : %f Think : %i FPS : %i", percentIntoNextFrame, thinkFPSPrint,
		                                                fpsPrint)),
		                nullptr, sys_getScreenSurface(), &consoleFont.pos);

		//Update the surface
		SDL_UpdateWindowSurface(sys_getWindow());
		fps++;
	}
	sys_shutdown();

	return 0;
}