#include <game/audio.h>
#include <gui/guiLanguage.h>
#include <gui/guiScrollbox.h>
#include <system/startup.h>
#include "../../hdr/system/scriptConfig.h"
#include "gui/guiSideview.h"

//----------------------------------------------------------------------------------------------------------------------
//
// This is how we call a script from the Host program : Name in Script : Name to call from host
void sys_scriptInitScriptFunctions ()
//----------------------------------------------------------------------------------------------------------------------
{
	paraScriptInstance.addScriptFunction ("void as_useNewRenderer(int &in newRenderer)", "as_useNewRenderer");
	paraScriptInstance.addScriptFunction ("void as_getAudioSpecs()", "as_getAudioSpecs");
	paraScriptInstance.addScriptFunction ("void as_loadAudioResources()", "as_loadAudioResources");
	paraScriptInstance.addScriptFunction ("void as_loadTextureResources()", "as_loadTextureResources");

	paraScriptInstance.addScriptFunction ("int as_getVolume()", "as_getVolume");
	paraScriptInstance.addScriptFunction ("void as_setVolume(int &in newVolume)", "as_setVolume");

	paraScriptInstance.addScriptFunction ("void as_createGUI()", "as_createGUI");
	paraScriptInstance.addScriptFunction ("void as_handleMainMenu()", "as_handleMainMenu");
	paraScriptInstance.addScriptFunction ("void as_handleOptionsMenu()", "as_handleOptionsMenu");

	paraScriptInstance.addScriptFunction ("void as_handleVideoMenu()", "as_handleVideoMenu");
	paraScriptInstance.addScriptFunction ("void as_handleAudioMenu()", "as_handleAudioMenu");
	paraScriptInstance.addScriptFunction ("void as_handleControlsMenu()", "as_handleControlsMenu");
	paraScriptInstance.addScriptFunction ("void as_handleGameMenu()", "as_handleGameMenu");

	paraScriptInstance.addScriptFunction ("void as_testFunction()", "as_testFunction");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup all the global variables to be shared between host and scripts
void sys_scriptInitVariables ()
//----------------------------------------------------------------------------------------------------------------------
{
	paraScriptInstance.addHostVariable ("bool quitProgram", &quitLoop);
	paraScriptInstance.addHostVariable ("int currentLanguage", &currentLanguage);
	paraScriptInstance.addHostVariable ("float sideviewDrawScale", &sideviewDrawScale);
	paraScriptInstance.addHostVariable ("int hiresVirtualWidth", &hiresVirtualWidth);
	paraScriptInstance.addHostVariable ("int hiresVirtualHeight", &hiresVirtualHeight);
	paraScriptInstance.addHostVariable ("bool enableSound", &enableSound);
	paraScriptInstance.addHostVariable ("int g_volumeLevel", &g_volumeLevel);
	paraScriptInstance.addHostVariable ("bool presentVSync", &presentVSync);
	paraScriptInstance.addHostVariable ("bool windowBorderless", &windowBorderless);
	paraScriptInstance.addHostVariable ("bool windowAllowHighDPI", &windowAllowHighDPI);
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
	//
	// Pass in the renderer class to the script
	paraScriptInstance.scriptEngine->RegisterObjectType ("paraRenderer", 0, asOBJ_REF);
	paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraRenderer", asBEHAVE_ADDREF, "void f()", asMETHOD(paraRenderer, AddRef), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraRenderer", asBEHAVE_RELEASE, "void f()", asMETHOD(paraRenderer, ReleaseRef), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraRenderer", "void getRendererInfo()", asMETHOD(paraRenderer, getRendererInfo), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraRenderer", "void d_getAllRenderers()", asMETHOD(paraRenderer, d_getAllRenderers), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraRenderer", "int getNumRenderers()", asMETHOD(paraRenderer, getNumRenderers), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraRenderer", "string getRendererByIndex(int whichRenderer)", asMETHOD(paraRenderer, getRendererByIndex), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraRenderer", "int getCurrentRendererIndex()", asMETHOD(paraRenderer, getCurrentRendererIndex), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterGlobalProperty ("paraRenderer as_renderer", &renderer);

	//
	// Pass in the texture class to the script
	paraScriptInstance.scriptEngine->RegisterObjectType ("paraTexture", 0, asOBJ_REF);
	paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraTexture", asBEHAVE_ADDREF, "void f()", asMETHOD(paraTexture, AddRef), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraTexture", asBEHAVE_RELEASE, "void f()", asMETHOD(paraTexture, ReleaseRef), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraTexture", "bool load(string fileName)", asMETHOD(paraTexture, load), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraTexture", "void createMap(string fileName)", asMETHOD(paraTexture, createMap), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterGlobalProperty ("paraTexture as_texture", &texture);
	//
	// Pass in the audio class to the script
	paraScriptInstance.scriptEngine->RegisterObjectType ("paraAudio", 0, asOBJ_REF);
	paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraAudio", asBEHAVE_ADDREF, "void f()", asMETHOD(paraAudio, AddRef), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraAudio", asBEHAVE_RELEASE, "void f()", asMETHOD(paraAudio, ReleaseRef), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraAudio", "bool load(string keyName, string fileName)", asMETHOD(paraAudio, load), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraAudio", "void deviceInfo()", asMETHOD(paraAudio, deviceInfo), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraAudio", "void setMasterVolume(int volume)", asMETHOD(paraAudio, setMasterVolume), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraAudio", "int getMasterVolume()", asMETHOD(paraAudio, getMasterVolume), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterGlobalProperty ("paraAudio as_audio", &audio);
	//
	// Pass in the GUI class to the script
	paraScriptInstance.scriptEngine->RegisterObjectType ("paraGui", 0, asOBJ_REF);
	paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraGui", asBEHAVE_ADDREF, "void f()", asMETHOD(paraGui, AddRef), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraGui", asBEHAVE_RELEASE, "void f()", asMETHOD(paraGui, ReleaseRef), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setLabel (int objectType, string objectID, int gapSize, int newLabelPos, string newLabel)", asMETHOD(paraGui, setLabel), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setAction (int objectType, string objectID, string newAction)", asMETHOD(paraGui, setAction), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setFontName (int objectType, string objectID, string newFontName)", asMETHOD(paraGui, setFontName), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setPosition (int objectType, string objectID, int newRadius, int coordType, double newPosX, double newPosY, double newWidth, double newHeight)", asMETHOD(paraGui, setPosition), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void addToScreen (int objectType, string objectID, string screenID)", asMETHOD(paraGui, addToScreen), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setColor (int objectType, string objectID, int whichColor, int red, int green, int blue, int alpha)", asMETHOD(paraGui, setColor), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setActive (string objectID)", asMETHOD(paraGui, setActive), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setReady (int objectType, string objectID, bool newState)", asMETHOD(paraGui, setReady), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void create (int objectType, string objectID)", asMETHOD(paraGui, create), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "int getCurrentScreen()", asMETHOD(paraGui, getCurrentScreen), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setCurrentScreen (int newScreen)", asMETHOD(paraGui, setCurrentScreen), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "int getIndex (int objectType, string &in objectID)", asMETHOD(paraGui, getIndex), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "int getActiveObjectIndex()", asMETHOD(paraGui, getActiveObjectIndex), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setActiveObject (int whichScreen, int objectType, string objectID)", asMETHOD(paraGui, setActiveObject), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void addNewElement ( string &in, string &in, string &in, int type)", asMETHOD(paraGui, addNewElement), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setSliderValue ( string &in, string &in)", asMETHOD(paraGui, setSliderValue), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setScrollSpeed (string &in, double newScrollSpeed)", asMETHOD(paraGui, setScrollSpeed), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setTickedStatus (string &in, int whichGroup, bool newValue)", asMETHOD(paraGui, setTickedStatus), asCALL_THISCALL);
	paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "string getSliderValue(string &in)", asMETHOD(paraGui, getSliderValue), asCALL_THISCALL);

	paraScriptInstance.scriptEngine->RegisterGlobalProperty ("paraGui as_paraGui", &gui);


	paraScriptInstance.addHostFunction ("void sys_printConInt (string &in, int param)", (functionPtr) &sys_scriptPrintInt);
	paraScriptInstance.addHostFunction ("void sys_addEvent (int eventType, int eventAction, int eventDelay, string &in)", (functionPtr) &sys_addEvent);
	paraScriptInstance.addHostFunction ("void gam_addAudioEvent (int action, bool loop, int distance, int pan, string &in)", (functionPtr) &gam_addAudioEvent);

	paraScriptInstance.addHostFunction ("void gui_addKeyAndText (string &in, string &in)", (functionPtr) &gui_addKeyAndText);
	paraScriptInstance.addHostFunction ("string gui_getString (string &in)", (functionPtr) &gui_getString);

	paraScriptInstance.addHostFunction ("void as_createSideViewColor  (int index, int red, int green, int blue, int alpha)", (functionPtr) &gui_createSideViewColor);
	paraScriptInstance.addHostFunction("void io_updateConfigValueInt(string &in, int newValue)", (functionPtr) &io_updateConfigValueInt);
}
