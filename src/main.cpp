#include "../hdr/main.h"
#include "../hdr/system/startup.h"
#include "../hdr/system/frameUpdate.h"
#include "../hdr/system/frameRender.h"
#include "system/gameEvents.h"

paraScript     paraScriptInstance;
paraLogFile    logFile;
paraFileSystem fileSystem;
paraFont       fontClass;
paraConsole    console (1, 255, 255, 255, 255);
paraRenderer   renderer;
paraAudio      audio;
paraGui        gui;

paraSprite databaseSprite;

std::map<std::string, paraTexture> textures;

bool   quitLoop             = false;
Uint32 maxNumUpdateLoops    = 0;
Uint32 fps                  = 0;
Uint32 fpsPrint             = 0;
Uint32 thinkFPS             = 0;
Uint32 thinkFPSPrint        = 0;
double percentIntoNextFrame = 0.0f;
int    currentMode;
double pixelsPerMeter;          // From script
Uint32 currentTime          = 0;

//
// Game levers
//
double baseGameSpeed;

//----------------------------------------------------------------------------------------------------------------------
//
// Return the current game time
Uint32 sys_getCurrentTime()
//----------------------------------------------------------------------------------------------------------------------
{
	return currentTime;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Timer callback to track the Update and Render FPS
Uint32 fpsCallback (Uint32 interval, void *param)
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
int main (int argc, char *argv[])
//----------------------------------------------------------------------------------------------------------------------
{
	double msPerUpdate  = 1000.0f / TICKS_PER_SECOND;
	double timeLag      = 0.0f;
	Uint32 previousTime = PARA_GetTicks ();

	sys_startSystems ();

	// TODO - use this for blinking player on deckview
	SDL_TimerID myTimer = SDL_AddTimer (1000, fpsCallback, nullptr);

	while (!quitLoop)
	{
		previousTime         = currentTime;
		currentTime          = PARA_GetTicks ();
		maxNumUpdateLoops    = 0;

		timeLag += (currentTime - previousTime);

		while (timeLag >= msPerUpdate && maxNumUpdateLoops < MAX_FRAMESKIP)
		{
			sys_gameTickRun ();
			timeLag -= msPerUpdate;
			thinkFPS++;
			maxNumUpdateLoops++;
		}
		percentIntoNextFrame = timeLag / msPerUpdate;
		percentIntoNextFrame = percentIntoNextFrame > 1.0 ? 1.0 : percentIntoNextFrame;
		percentIntoNextFrame = percentIntoNextFrame < 0.0 ? 0.0 : percentIntoNextFrame;

		sys_renderFrame (percentIntoNextFrame);
		fps++;
	}

	sys_shutdown ();

	return 0;
}