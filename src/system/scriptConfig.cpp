#include "../../hdr/system/scriptConfig.h"

bool quitProgram;

//----------------------------------------------------------------------------------------------------------------------
//
// This is how we call a script from the Host program : Name in Script : Name to call from host
void sys_scriptInitScriptFunctions ()
//----------------------------------------------------------------------------------------------------------------------
{
	paraScriptInstance.addScriptFunction (logFile, "void script_loadAllResources()", "script_loadAllResources");
	paraScriptInstance.addScriptFunction (logFile, "void script_initGUI()", "script_initGUI");
	paraScriptInstance.addScriptFunction (logFile, "void as_guiHandleElementAction(string &in objectID)", "as_guiHandleElementAction");
	paraScriptInstance.addScriptFunction (logFile, "void as_guiHandleTerminalAction(string &in objectID)", "as_guiHandleTerminalAction");
	paraScriptInstance.addScriptFunction (logFile, "void as_guiHandleDatabaseAction(string &in objectID)", "as_guiHandleDatabaseAction");
	paraScriptInstance.addScriptFunction (logFile, "void as_guiHandleTransferAction(string &in objectID)", "as_guiHandleTransferAction");
	paraScriptInstance.addScriptFunction (logFile, "void as_guiHandleDialogAction(string &in objectID)", "as_guiHandleDialogAction");
}


//----------------------------------------------------------------------------------------------------------------------
//
// Setup all the global variables to be shared between host and scripts
void sys_scriptInitVariables ()
//----------------------------------------------------------------------------------------------------------------------
{
	paraScriptInstance.addHostVariable (logFile, "int quitProgram", &quitProgram);
}

int sys_scriptPrintInt()
{

	return 0;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup all the globals functions that the scripts can call for action in the host
void sys_scriptInitFunctions ()
//----------------------------------------------------------------------------------------------------------------------
{
	paraScriptInstance.addHostFunction (logFile, "void sys_printConInt(string &in, uint param)", reinterpret_cast<asSFuncPtr &>(sys_scriptPrintInt));

}