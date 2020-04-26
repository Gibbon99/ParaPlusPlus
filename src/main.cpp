#include "../hdr/main.h"
#include "../hdr/system/startup.h"
#include "../hdr/system/frameUpdate.h"
#include "../hdr/system/frameRender.h"

paraScript     paraScriptInstance;
paraLogFile    logFile;
paraFileSystem fileSystem;
paraFont       consoleFont;
paraConsole    console (1, 255, 255, 255, 255);
paraRenderer   renderer;

#define TICKS_PER_SECOND 30.0f
#define MAX_FRAMESKIP 5

bool   quitLoop             = false;
Uint32 maxNumUpdateLoops    = 0;
Uint32 fps                  = 0;
Uint32 fpsPrint             = 0;
Uint32 thinkFPS             = 0;
Uint32 thinkFPSPrint        = 0;
double percentIntoNextFrame = 0.0f;


std::string testVarString = "First string text";
float       testVarFloat  = 3.145;


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
	double msPerUpdate  = 1000.0f / TICKS_PER_SECOND;
	double timeLag      = 0.0f;
	Uint32 previousTime = PARA_GetTicks();
	Uint32 currentTime  = 0;

	sys_startSystems();

	SDL_TimerID myTimer = SDL_AddTimer(1000, fpsCallback, nullptr);

	while (!quitLoop)
	{
		previousTime             = currentTime;
		currentTime              = PARA_GetTicks();
		maxNumUpdateLoops        = 0;

		timeLag += (currentTime - previousTime);

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
		fps++;
	}

	sys_shutdown();

	return 0;
}