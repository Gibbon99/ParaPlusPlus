#include "system/util.h"
#include "io/logFile.h"
#include "system/scriptEngine.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Print messages from script compiler to console
void scr_Output(const asSMessageInfo *msg, [[maybe_unused]]void *param)
//----------------------------------------------------------------------------------------------------------------------
{
	int        alertColor = 255;
	const char *type      = "ERR ";

	if (msg->type == asMSGTYPE_WARNING)
	{
		type       = "WARN";
		alertColor = 200;
	}
	else if (msg->type == asMSGTYPE_INFORMATION)
	{
		type       = "INFO";
		alertColor = 150;
	}

	log_addEvent (sys_getString ("%s (%d, %d) : %s : %s", msg->section, msg->row, msg->col, type, msg->message));

	console.add (1, alertColor, 0, 0, 255, sys_getString ("%s (%d, %d) : %s : %s", msg->section, msg->row, msg->col, type, msg->message));

	printf ("%s\n", sys_getString ("%s (%d, %d) : %s : %s", msg->section, msg->row, msg->col, type, msg->message).c_str ());
}
