#include "../../hdr/system/startup.h"
#include "../../hdr/system/scriptEngine.h"
#include "../../hdr/system/scriptConfig.h"
#include "../../hdr/io/fileSystem.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Start systems - Logfile, Script, Filesystem ( Physfs ), Event queues
void sys_startSystems()
//----------------------------------------------------------------------------------------------------------------------
{
	if (!logFile.open("paraLogFile.txt"))
		sys_shutdownWithError("Error: Could not start paraLogFile. Check write permissions on folder.");
	logFile.write("Logfile started.");

	if (!fileSystem.init(logFile, "data", "data"))
		sys_shutdownWithError("Error. Could not start filesystem. Check directory structure.");

	fileSystem.addPath(logFile, "data/data");
	fileSystem.addPath(logFile, "data/scripts");

#ifdef MY_DEBUG
	logFile.write("Running DEBUG version.");
#endif

	if (!paraScriptInstance.init(logFile, reinterpret_cast<asSFuncPtr &>(scr_Output)))
		sys_shutdownWithError("Error: Could not start Scripting engine.");

	sys_scriptInitScriptFunctions();
	sys_scriptInitFunctions();
	sys_scriptInitVariables();
	io_getScriptFileNames("scripts");
	paraScriptInstance.loadAndCompile(logFile);
	paraScriptInstance.cacheFunctions(logFile);
}
