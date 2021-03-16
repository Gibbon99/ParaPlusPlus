#include "system/util.h"
#include "system/physics.h"
#include "game/particles.h"
#include "gui/guiHighScore.h"
#include "../../hdr/system/shutdown.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Close down systems
void sys_closeSystems()
//----------------------------------------------------------------------------------------------------------------------
{
	io_saveConfigValues();

	gui_writeHighScore();;

	gam_clearEmitters();
	sys_freePhysicsEngine ();

	io_closeJoystick();
	paraScriptInstance.stop();
	sys_freeMemory ();
	logFile.close();

	evt_stopThreads();

	evt_destroyMutexes();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Shutdown with an error
void sys_shutdownWithError(std::string errorMessage)
//----------------------------------------------------------------------------------------------------------------------
{
	if (SDL_ShowSimpleMessageBox (SDL_MESSAGEBOX_ERROR, "Fatal Error", errorMessage.c_str (), renderer.window) < 0)
	{
		cout << "Fatal error : " << errorMessage << endl;
	}

	sys_closeSystems ();

	exit (-1);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Shutdown normally
void sys_shutdown()
//----------------------------------------------------------------------------------------------------------------------
{
	sys_closeSystems();

	exit(0);
}