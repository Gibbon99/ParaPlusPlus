#include "../../hdr/system/scriptEngine.h"

bool scriptReloaded = false;

//----------------------------------------------------------------------------------------------------------------------
//
// Print messages from script compiler to console
void scr_Output(const asSMessageInfo *msg, void *param)
//----------------------------------------------------------------------------------------------------------------------
{
	const char *type = "ERR ";

	if (msg->type == asMSGTYPE_WARNING)
	{
		type = "WARN";
	}
	else if (msg->type == asMSGTYPE_INFORMATION)
	{
		type = "INFO";
	}

	logFile.write(sys_getString("%s (%d, %d) : %s : %s", msg->section, msg->row, msg->col, type, msg->message));

	printf("%s\n",
	       sys_getString("%s (%d, %d) : %s : %s", msg->section, msg->row, msg->col, type, msg->message).c_str());
}

/*
//----------------------------------------------------------------------------------------------------------------------
//
// Restart the script engine
void sys_restartScriptEngine()
//----------------------------------------------------------------------------------------------------------------------
{
	sys_stopScriptEngine();

	scriptFunctions.clear();
	scriptFunctionName.clear();

	scriptReloaded = true;

	sys_initScriptEngine();
}
*/