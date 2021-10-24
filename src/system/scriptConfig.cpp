#include <cassert>
#include "game/tiles.h"
#include "game/shipDecks.h"
#include "game/healing.h"
#include "game/player.h"
#include "game/lineOfSight.h"
#include "game/score.h"
#include "game/transferRender.h"
#include "game/transfer.h"
#include "game/transferDroidAI.h"
#include "game/transferGame.h"
#include "game/game.h"
#include "game/alertLevel.h"
#include "game/audio.h"
#include "game/database.h"
#include "game/hud.h"
#include "game/texture.h"
#include "game/doors.h"
#include "gui/guiLostScreen.h"
#include "gui/guiHighScore.h"
#include "gui/guiSideview.h"
#include "gui/guiLanguage.h"
#include "io/keyboard.h"
#include "system/scriptConfig.h"
#include "system/startup.h"
#include "system/cpPhysics.h"
#include "system/util.h"
#include "classes/paraBullet.h"

//----------------------------------------------------------------------------------------------------------------------
//
// This is how we call a script from the Host program : Name in Script : Name to call from host
void sys_scriptInitScriptFunctions()
//----------------------------------------------------------------------------------------------------------------------
{
	paraScriptInstance.addScriptFunction ("void as_useNewRenderer(int &in newRenderer)", "as_useNewRenderer");
	paraScriptInstance.addScriptFunction ("void as_getAudioSpecs()", "as_getAudioSpecs");
	paraScriptInstance.addScriptFunction ("void as_loadAudioResources()", "as_loadAudioResources");
	paraScriptInstance.addScriptFunction ("void as_loadTextureResources()", "as_loadTextureResources");
	paraScriptInstance.addScriptFunction ("void as_loadAllDecks()", "as_loadAllDecks");

	paraScriptInstance.addScriptFunction ("int as_getVolume()", "as_getVolume");
	paraScriptInstance.addScriptFunction ("void as_setVolume(int newVolume)", "as_setVolume");

	paraScriptInstance.addScriptFunction ("void as_createGUI()", "as_createGUI");
	paraScriptInstance.addScriptFunction ("void as_handleMainMenu()", "as_handleMainMenu");
	paraScriptInstance.addScriptFunction ("void as_handleOptionsMenu()", "as_handleOptionsMenu");

	paraScriptInstance.addScriptFunction ("void as_handleVideoOptions()", "as_handleVideoOptions");
	paraScriptInstance.addScriptFunction ("void as_handleAudioMenu()", "as_handleAudioMenu");
	paraScriptInstance.addScriptFunction ("void as_handleControlsMenu()", "as_handleControlsMenu");
	paraScriptInstance.addScriptFunction ("void as_handleGameMenu()", "as_handleGameMenu");

	paraScriptInstance.addScriptFunction ("void as_handleDeckviewScreen()", "as_handleDeckviewScreen");
	paraScriptInstance.addScriptFunction ("void as_handleShipviewScreen()", "as_handleShipviewScreen");
	paraScriptInstance.addScriptFunction ("void as_handleTerminalMenu()", "as_handleTerminalMenu");
	paraScriptInstance.addScriptFunction ("void as_handleDatabaseMenu()", "as_handleDatabaseMenu");

	paraScriptInstance.addScriptFunction ("void as_handleTransferScreens()", "as_handleTransferScreens");

	paraScriptInstance.addScriptFunction ("void as_handleTutorialElements ()", "as_handleTutorialElements");

	paraScriptInstance.addScriptFunction ("void as_handleDialogBoxes ()", "as_handleDialogBoxes");

	paraScriptInstance.addScriptFunction ("void as_handleHighscoreDisplay()", "as_handleHighscoreDisplay");
	paraScriptInstance.addScriptFunction ("void as_refreshHighscoreLabels()", "as_refreshHighscoreLabels");
	paraScriptInstance.addScriptFunction ("void as_refreshControlLabels()", "as_refreshControlLabels");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup all the global variables to be shared between host and scripts
void sys_scriptInitVariables()
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
	paraScriptInstance.addHostVariable ("double baseGameSpeed", &baseGameSpeed);
	paraScriptInstance.addHostVariable ("double hudTextPosX", &hudTextPosX);
	paraScriptInstance.addHostVariable ("double hudTextPosY", &hudTextPosY);
	paraScriptInstance.addHostVariable ("double hudScorePosX", &hudScorePosX);
	paraScriptInstance.addHostVariable ("double hudScorePosY", &hudScorePosY);
	paraScriptInstance.addHostVariable ("bool doScreenEffect", &doScreenEffect);
	paraScriptInstance.addHostVariable ("string tileFilename", &tileFilename);
	paraScriptInstance.addHostVariable ("string tileStyle", &tileStyle);
	paraScriptInstance.addHostVariable ("string tileColor", &tileColor);
	paraScriptInstance.addHostVariable ("double healingAnimSpeed", &healingAnimSpeed);
	paraScriptInstance.addHostVariable ("double pixelsPerMeter", &pixelsPerMeter);
	paraScriptInstance.addHostVariable ("double playerFriction", &playerFriction);
	paraScriptInstance.addHostVariable ("double gravity", &gravity);
	paraScriptInstance.addHostVariable ("double doorAnimSpeed", &doorAnimSpeed);
	paraScriptInstance.addHostVariable ("float collisionCount", &collisionCount);
	paraScriptInstance.addHostVariable ("int collisionLimit", &collisionLimit);
	paraScriptInstance.addHostVariable ("float bulletDensity", &bulletDensity);
	paraScriptInstance.addHostVariable ("float bulletFriction", &bulletFriction);
	paraScriptInstance.addHostVariable ("float bulletAnimationSpeed", &bulletAnimationSpeed);
	paraScriptInstance.addHostVariable ("float numDisrupterFrames", &numDisrupterFrames);
	paraScriptInstance.addHostVariable ("float disrupterFadeAmount", &disrupterFadeAmount);
	paraScriptInstance.addHostVariable ("float bulletMoveSpeed", &bulletMoveSpeed);
	paraScriptInstance.addHostVariable ("int visibleFadeValue", &visibleFadeValue);
	paraScriptInstance.addHostVariable ("int redAlertLevel", &redAlertLevel);
	paraScriptInstance.addHostVariable ("int yellowAlertLevel", &yellowAlertLevel);
	paraScriptInstance.addHostVariable ("float updateScoreDelay", &updateScoreDelay);
	paraScriptInstance.addHostVariable ("float distanceForDoorSoundMax", &distanceForDoorSoundMax);

	paraScriptInstance.addHostVariable ("int numberTransferRows", &numberTransferRows);
	paraScriptInstance.addHostVariable ("int transferBitmapHeight", &transferBitmapHeight);
	paraScriptInstance.addHostVariable ("int transferBitmapWidth", &transferBitmapWidth);

	paraScriptInstance.addHostVariable ("float transferRowHeight", &transferRowHeight);
	paraScriptInstance.addHostVariable ("float transferRowStartY", &transferRowStartY);
	paraScriptInstance.addHostVariable ("float transferRowCellWidth", &transferRowCellWidth);

	paraScriptInstance.addHostVariable ("float transferBackgroundStartX", &transferBackgroundStartX);
	paraScriptInstance.addHostVariable ("float transferBackgroundStartY", &transferBackgroundStartY);
	paraScriptInstance.addHostVariable ("float transferBackgroundWidth", &transferBackgroundWidth);
	paraScriptInstance.addHostVariable ("float transferBackgroundHeight", &transferBackgroundHeight);
	paraScriptInstance.addHostVariable ("int transferBorderThickness", &transferBorderThickness);
	paraScriptInstance.addHostVariable ("float transferStatusTabWidth", &transferStatusTabWidth);
	paraScriptInstance.addHostVariable ("float transferStatusTabHeight", &transferStatusTabHeight);
	paraScriptInstance.addHostVariable ("float transferSidebarGap", &transferSidebarGap);
	paraScriptInstance.addHostVariable ("float transferSidebarWidth", &transferSidebarWidth);
	paraScriptInstance.addHostVariable ("float transferLineThickness", &transferLineThickness);
	paraScriptInstance.addHostVariable ("int chooseSideTimeOut", &chooseSideTimeOut);
	paraScriptInstance.addHostVariable ("float chooseSideDelayTime", &chooseSideDelayTime);
	paraScriptInstance.addHostVariable ("float chooseRowDelayTime", &chooseRowDelayTime);
	paraScriptInstance.addHostVariable ("float activeRowCounter", &activeRowCounter);
	paraScriptInstance.addHostVariable ("int transferTimeOut", &transferTimeOut);
	paraScriptInstance.addHostVariable ("float transferDelayTime", &transferDelayTime);
	paraScriptInstance.addHostVariable ("int powerdownLevelScore", &powerdownLevelScore);
	paraScriptInstance.addHostVariable ("float healingDelayCounter", &healingDelayCounter);
	paraScriptInstance.addHostVariable ("int healingAmountPerTick", &healingAmountPerTick);
	paraScriptInstance.addHostVariable ("float explosionAnimationSpeed", &explosionAnimationSpeed);
	paraScriptInstance.addHostVariable ("int explosionDamage", &explosionDamage);
	paraScriptInstance.addHostVariable ("int alertLevelVolume", &alertLevelVolume);
	paraScriptInstance.addHostVariable ("float staticAnimationDelay", &staticAnimationDelay);
	paraScriptInstance.addHostVariable ("int lostScreenShowTime", &lostScreenShowTime);
//	paraScriptInstance.addHostVariable ("float tableSpacingY", &tableSpacingY);
	paraScriptInstance.addHostVariable ("float influenceTimelimit", &influenceTimelimit);
	paraScriptInstance.addHostVariable ("float influenceTimelimtDelay", &influenceTimelimtDelay);
	paraScriptInstance.addHostVariable ("float influenceTimeLeftWarning", &influenceTimeLeftWarning);
	paraScriptInstance.addHostVariable ("int transferResultDelay", &transferResultDelay);

	paraScriptInstance.addHostVariable ("int maxNumBumps", &maxNumBumps);
	paraScriptInstance.addHostVariable ("float bounceCounterDelay", &bounceCounterDelay);
}

void sys_scriptPrintInt(std::string inStr, int inInt)
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
void sys_scriptInitFunctions()
//----------------------------------------------------------------------------------------------------------------------
{
	int r;

	//
	// Pass in the renderer class to the script
	r = paraScriptInstance.scriptEngine->RegisterObjectType ("paraRenderer", 0, asOBJ_REF);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraRenderer", asBEHAVE_ADDREF, "void f()", asMETHOD(paraRenderer, AddRef), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraRenderer", asBEHAVE_RELEASE, "void f()", asMETHOD(paraRenderer, ReleaseRef), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraRenderer", "void getRendererInfo()", asMETHOD(paraRenderer, getRendererInfo), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraRenderer", "void d_getAllRenderers()", asMETHOD(paraRenderer, d_getAllRenderers), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraRenderer", "int getNumRenderers()", asMETHOD(paraRenderer, getNumRenderers), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraRenderer", "string getRendererByIndex(int whichRenderer)", asMETHOD(paraRenderer, getRendererByIndex), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraRenderer", "int getCurrentRendererIndex()", asMETHOD(paraRenderer, getCurrentRendererIndex), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterGlobalProperty ("paraRenderer as_renderer", &renderer);
	assert(r >= 0);


	//
	// Pass in the player class to the script
	r = paraScriptInstance.scriptEngine->RegisterObjectType ("paraDroid", 0, asOBJ_REF);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraDroid", asBEHAVE_ADDREF, "void f()", asMETHOD(paraDroid, AddRef), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraDroid", asBEHAVE_RELEASE, "void f()", asMETHOD(paraDroid, ReleaseRef), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraDroid", "int getDroidType()", asMETHOD(paraDroid, getDroidType), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraDroid", "void setCurrentHealth(int newCurrentHealth)", asMETHOD(paraDroid, setCurrentHealth), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterGlobalProperty ("paraDroid as_paraDroid", &playerDroid);
	assert(r >= 0);


	//
	// Pass in the texture class to the script
	r = paraScriptInstance.scriptEngine->RegisterObjectType ("paraTexture", 0, asOBJ_REF);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraTexture", asBEHAVE_ADDREF, "void f()", asMETHOD(paraTexture, AddRef), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraTexture", asBEHAVE_RELEASE, "void f()", asMETHOD(paraTexture, ReleaseRef), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterGlobalProperty ("paraTexture as_texture", &textures);
	assert(r >= 0);
	//
	// Pass in the audio class to the script
	r = paraScriptInstance.scriptEngine->RegisterObjectType ("paraAudio", 0, asOBJ_REF);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraAudio", asBEHAVE_ADDREF, "void f()", asMETHOD(paraAudio, AddRef), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraAudio", asBEHAVE_RELEASE, "void f()", asMETHOD(paraAudio, ReleaseRef), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraAudio", "bool load(string keyName, string fileName)", asMETHOD(paraAudio, load), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraAudio", "int play (string keyName, bool loop, int distance, int pan)", asMETHOD(paraAudio, play), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraAudio", "void deviceInfo()", asMETHOD(paraAudio, deviceInfo), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraAudio", "void setMasterVolume(int volume)", asMETHOD(paraAudio, setMasterVolume), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraAudio", "int getMasterVolume()", asMETHOD(paraAudio, getMasterVolume), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterGlobalProperty ("paraAudio as_audio", &audio);
	assert(r >= 0);
	//
	// Pass in the GUI class to the script
	r = paraScriptInstance.scriptEngine->RegisterObjectType ("paraGui", 0, asOBJ_REF);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraGui", asBEHAVE_ADDREF, "void f()", asMETHOD(paraGui, AddRef), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectBehaviour ("paraGui", asBEHAVE_RELEASE, "void f()", asMETHOD(paraGui, ReleaseRef), asCALL_THISCALL);
	assert(r >= 0);

	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "string getKeyName (int keyIndex)", asMETHOD(paraGui, getKeyName), asCALL_THISCALL);
	assert(r >= 0);

	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void save ()", asMETHOD(paraGui, save), asCALL_THISCALL);
	assert(r >= 0);

	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setLabel (int objectType, string objectID, int gapSize, int newLabelPos, string newLabel)", asMETHOD(paraGui, setLabel), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setAction (int objectType, string objectID, string newAction)", asMETHOD(paraGui, setAction), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setFontName (int objectType, string objectID, string newFontName)", asMETHOD(paraGui, setFontName), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setPosition (int objectType, string objectID, int newRadius, int coordType, double newPosX, double newPosY, double newWidth, double newHeight)", asMETHOD(paraGui, setPosition), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void addToScreen (int objectType, string objectID, string screenID)", asMETHOD(paraGui, addToScreen), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void addToDialogbox (int objectType, string objectID, string screenID)", asMETHOD(paraGui, addToDialogbox), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setColor (int objectType, string objectID, int whichColor, int red, int green, int blue, int alpha)", asMETHOD(paraGui, setColor), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setActive (string objectID)", asMETHOD(paraGui, setActive), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setReady (int objectType, string objectID, bool newState)", asMETHOD(paraGui, setReady), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void create (int objectType, string objectID)", asMETHOD(paraGui, create), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "int getCurrentScreen()", asMETHOD(paraGui, getCurrentScreen), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "int getCurrentDialogbox()", asMETHOD(paraGui, getCurrentDialogbox), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setCurrentScreen (int newScreen)", asMETHOD(paraGui, setCurrentScreen), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setCurrentDialogbox (int newScreen)", asMETHOD(paraGui, setCurrentDialogbox), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "int getIndex (int objectType, string &in objectID)", asMETHOD(paraGui, getIndex), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "int getActiveObjectIndex()", asMETHOD(paraGui, getActiveObjectIndex), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "int getActiveObjectIndexDialogbox()", asMETHOD(paraGui, getActiveObjectIndexDialogbox), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setActiveObject (int whichScreen, int objectType, string objectID)", asMETHOD(paraGui, setActiveObject), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setActiveObjectDialogbox (int whichScreen, int objectType, string objectID)", asMETHOD(paraGui, setActiveObjectDialogbox), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void addNewElement ( string &in, string &in, string &in, int type)", asMETHOD(paraGui, addNewElement), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setSliderValue ( string &in, string &in)", asMETHOD(paraGui, setSliderValue), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setScrollSpeed (int objectType, const string &in, double newScrollSpeed)", asMETHOD(paraGui, setScrollSpeed), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void setTickedStatus (string &in, int whichGroup, bool newValue)", asMETHOD(paraGui, setTickedStatus), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "string getSliderValue(string &in)", asMETHOD(paraGui, getSliderValue), asCALL_THISCALL);
	assert(r >= 0);
	r = paraScriptInstance.scriptEngine->RegisterObjectMethod ("paraGui", "void restartScrollBox(string &in)", asMETHOD(paraGui, restartScrollBox), asCALL_THISCALL);
	assert(r >= 0);

	r = paraScriptInstance.scriptEngine->RegisterGlobalProperty ("paraGui as_paraGui", &gui);
	assert(r >= 0);

	paraScriptInstance.addHostFunction ("void sys_printConInt (string &in, int param)", asFUNCTION(sys_scriptPrintInt));

	paraScriptInstance.addHostFunction ("void sys_setNewMode (int newMode, bool doFade)", asFUNCTION(sys_setNewMode));
	paraScriptInstance.addHostFunction ("void sys_addEvent (int eventType, int eventAction, int eventDelay, string &in)", asFUNCTION(sys_addEvent));
	paraScriptInstance.addHostFunction ("void gam_addAudioEvent (int action, bool loop, int distance, int pan, string &in)", asFUNCTION(gam_addAudioEvent));

	paraScriptInstance.addHostFunction ("void gui_addKeyAndText (string &in, string &in)", asFUNCTION(gui_addKeyAndText));
	paraScriptInstance.addHostFunction ("string gui_getString (string &in)", asFUNCTION(gui_getString));

	paraScriptInstance.addHostFunction ("void as_createSideViewColor  (int index, int red, int green, int blue, int alpha)", asFUNCTION(gui_createSideViewColor));
	paraScriptInstance.addHostFunction ("void io_updateConfigValueInt(string &in, int newValue)", asFUNCTION(io_updateConfigValueInt));
	paraScriptInstance.addHostFunction ("void io_updateConfigValue(string &in, string &in)", asFUNCTION(io_updateConfigValue));

	paraScriptInstance.addHostFunction ("void gam_previousDatabase()", asFUNCTION(gam_previousDatabase));
	paraScriptInstance.addHostFunction ("void gam_nextDatabase()", asFUNCTION(gam_nextDatabase));
	paraScriptInstance.addHostFunction ("void gam_prepareDatabaseScreen(int whichDroidIndex)", asFUNCTION(gam_prepareDatabaseScreen));
	paraScriptInstance.addHostFunction ("void gam_setHudText(string &in)", asFUNCTION(gam_setHudText));
	paraScriptInstance.addHostFunction ("void gam_loadTexture(string &in, string &in)", asFUNCTION(gam_loadTexture));
	paraScriptInstance.addHostFunction ("void gam_setTileType()", asFUNCTION(gam_setTileType));
	paraScriptInstance.addHostFunction ("void gam_loadShipDeck (string &in)", asFUNCTION(gam_loadShipDeck));
	paraScriptInstance.addHostFunction ("void gam_changeToDeck (string &in, int whichLift)", asFUNCTION(gam_changeToDeck));
	paraScriptInstance.addHostFunction ("void trn_setTransferColor (int whichSide, float red, float green, float blue, float alpha)", asFUNCTION(trn_setTransferColor));
	paraScriptInstance.addHostFunction ("void io_initNewKeycodeValue (int whichKey)", asFUNCTION(io_initNewKeycodeValue));

	paraScriptInstance.addHostFunction ("string gui_getHighScoreValueByIndex (int highScoreIndex)", asFUNCTION(gui_getHighScoreValueByIndex));
	paraScriptInstance.addHostFunction ("string gui_getHighScoreNameByIndex (int highScoreIndex)", asFUNCTION(gui_getHighScoreNameByIndex));

	paraScriptInstance.addHostFunction ("void gui_showHighscoreTable ()", asFUNCTION(gui_showHighscoreTable));

	paraScriptInstance.addHostFunction ("void gui_insertNewScore (string &in)", asFUNCTION(gui_insertNewScore));
}


