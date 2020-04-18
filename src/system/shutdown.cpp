#include "../../hdr/system/shutdown.h"
#include "../../hdr/system/startup.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Close down systems
void sys_closeSystems()
//----------------------------------------------------------------------------------------------------------------------
{
	paraScriptInstance.stop();
	sys_freeMemory ();

	logFile.close();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Shutdown with an error
void sys_shutdownWithError(const std::string& errorMessage)
//----------------------------------------------------------------------------------------------------------------------
{
	if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal Error", errorMessage.c_str(), sys_getWindow()) < 0)
	{
		cout << "Fatal error : " << errorMessage << endl;
	}

	sys_closeSystems();

    exit(-1);
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