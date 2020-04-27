#include "../../hdr/system/scriptConfig.h"

//----------------------------------------------------------------------------------------------------------------------
//
// This is how we call a script from the Host program : Name in Script : Name to call from host
void sys_scriptInitScriptFunctions ()
//----------------------------------------------------------------------------------------------------------------------
{
	paraScriptInstance.addScriptFunction ("void as_useNewRenderer(int &in newRenderer)", "as_useNewRenderer");

	paraScriptInstance.addScriptFunction ("void as_testFunction()", "as_testFunction");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup all the global variables to be shared between host and scripts
void sys_scriptInitVariables ()
//----------------------------------------------------------------------------------------------------------------------
{
	paraScriptInstance.addHostVariable("int quitProgram", &quitLoop);
}

void sys_scriptPrintInt (std::string inStr, int inInt)
{
	std::cout << "String : " << inStr << "Int : " << inInt << endl;
}
/*
// Function implementation with generic script interface
void PrintString_Generic(asIScriptGeneric *gen)
{
	string *str = (string*)gen->GetArgAddress(0);
	cout << *str;
}
*/
//----------------------------------------------------------------------------------------------------------------------
//
// Setup all the globals functions that the scripts can call for action in the host
void sys_scriptInitFunctions ()
//----------------------------------------------------------------------------------------------------------------------
{
//	paraScriptInstance.scriptEngine->RegisterObjectMethod("paraRenderer", "void useNewRenderer(int)", asMETHOD(paraRenderer, useNewRenderer), asCALL_THISCALL);

	paraScriptInstance.scriptEngine->RegisterObjectType ("paraRenderer", 0, asOBJ_REF);
	paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraRenderer", asBEHAVE_ADDREF, "void f()", asMETHOD(paraRenderer, AddRef), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraRenderer", asBEHAVE_RELEASE, "void f()", asMETHOD(paraRenderer, ReleaseRef), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraRenderer", "void getRendererInfo()", asMETHOD(paraRenderer, getRendererInfo), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraRenderer", "void d_getAllRenderers()", asMETHOD(paraRenderer, d_getAllRenderers), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterGlobalProperty ("paraRenderer as_renderer", &renderer);

	paraScriptInstance.addHostFunction ("void sys_printConInt(string &in, int param)", (functionPtr) &sys_scriptPrintInt);
	paraScriptInstance.addHostFunction ("void sys_addEvent(int eventType, int eventAction, int eventDelay, string &in)", (functionPtr) &sys_addEvent);
}
