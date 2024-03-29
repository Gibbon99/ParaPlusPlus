#include "game/tiles.h"
#include "system/util.h"
#include "gui/guiLanguage.h"
#include "io/configFile.h"
#include "gui/guiSideview.h"

CSimpleIniA iniFile;
std::string configFileName = "";

//--------------------------------------------------------------------------------------------------------------------
//
// Save the in memory config file to disk
void io_saveConfigValues()
//--------------------------------------------------------------------------------------------------------------------
{
	static int loopCount = 0;

	auto returnCode = iniFile.SaveFile (configFileName.c_str (), false);
	if (returnCode < 0)
	{
		loopCount++;
		sys_shutdownWithError(sys_getString("Unable to save config file [ %s ]", configFileName.c_str()));
		if (loopCount > 3)
			exit(-1);
	}
}

//--------------------------------------------------------------------------------------------------------------------
//
// Update a value to be saved into the config file - INT version
void io_updateConfigValueInt(const std::string &keyName, int newValue)
//--------------------------------------------------------------------------------------------------------------------
{
	auto returnCode = iniFile.SetValue ("Main", keyName.c_str (), sys_getString ("%i", newValue).c_str ());
	if (returnCode < 0)
		sys_shutdownWithError (sys_getString ("Unable to update config file value [ %s ]", keyName.c_str ()));
}

//--------------------------------------------------------------------------------------------------------------------
//
// Update a value to be saved into the config file - STRING version
void io_updateConfigValue(const std::string &keyName, const std::string &newValue)
//--------------------------------------------------------------------------------------------------------------------
{
	auto returnCode = iniFile.SetValue ("Main", keyName.c_str (), newValue.c_str ());
	if (returnCode < 0)
		sys_shutdownWithError (sys_getString ("Unable to update config file value [ %s ]", keyName.c_str ()));
}

//--------------------------------------------------------------------------------------------------------------------
//
// Read values from config ini file
void io_readConfigValues(const std::string &fileName)
//--------------------------------------------------------------------------------------------------------------------
{
	configFileName = fileName;

	auto returnCode = iniFile.LoadFile (fileName.c_str ());
	if (returnCode < 0)
	{
		sys_shutdownWithError ("Unable to open config file.");
	}

	auto returnValue = iniFile.GetValue ("Main", "consoleFontFilename", "default");
	if (strcmp (returnValue, "default") == 0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "consoleFontFilename"));
	consoleFontFilename = returnValue;

	returnValue = iniFile.GetValue ("Main", "guiFontFileName", "default");
	if (strcmp (returnValue, "default") == 0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "guiFontFileName"));
	guiFontFileName = returnValue;

	guiFontSize = (int) iniFile.GetLongValue ("Main", "guiFontSize", 0);
	if (guiFontSize == 0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "guiFontSize"));

	returnValue = iniFile.GetValue ("Main", "introFontFileName", "default");
	if (strcmp (returnValue, "default") == 0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "introFontFileName"));
	introFontFileName = returnValue;

	introFontSize = (int) iniFile.GetLongValue ("Main", "introFontSize", 0);
	if (guiFontSize == 0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "introFontSize"));

	hiresVirtualWidth = (int) iniFile.GetLongValue ("Main", "hiresVirtualWidth", 0);
	if (hiresVirtualWidth == 0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "hiresVirtualWidth"));

	hiresVirtualHeight = (int) iniFile.GetLongValue ("Main", "hiresVirtualHeight", 0);
	if (hiresVirtualHeight == 0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "hiresVirtualHeight"));

	consoleNumColumns = (int) iniFile.GetLongValue ("Main", "consoleNumColumns", 0);
	if (consoleNumColumns == 0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "consoleNumColumns"));

	consoleFontSize = (int) iniFile.GetLongValue ("Main", "consoleFontSize", 0);
	if (consoleFontSize == 0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "consoleFontSize"));

	gameWinWidth = (int) iniFile.GetLongValue ("Main", "gameWinWidth", 0);
	if (gameWinWidth == 0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "gameWinWidth"));

	gameWinHeight = (int) iniFile.GetLongValue ("Main", "gameWinHeight", 0);
	if (gameWinHeight == 0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "gameWinHeight"));

	windowWidth = (int) iniFile.GetLongValue ("Main", "windowWidth", 0);
	if (windowWidth == 0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "windowWidth"));

	windowHeight = (int) iniFile.GetLongValue ("Main", "windowHeight", 0);
	if (windowHeight == 0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "windowHeight"));

	windowFullscreen = (int) iniFile.GetLongValue ("Main", "windowFullscreen", -1);
	if (windowFullscreen == -1)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "windowFullscreen"));

	windowFullscreenDesktop = (int) iniFile.GetLongValue ("Main", "windowFullscreenDesktop", -1);
	if (windowFullscreenDesktop == -1)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "windowFullscreenDesktop"));

	auto windowBorderlessResult = static_cast<int>(iniFile.GetLongValue ("Main", "windowBorderless", -1));
	if (windowBorderlessResult == -1)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "windowBorderless"));
	windowBorderless = windowBorderlessResult;

	windowInputGrabbed = (int) iniFile.GetLongValue ("Main", "windowInputGrabbed", -1);
	if (windowInputGrabbed == -1)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "windowInputGrabbed"));

	windowInputFocus = (int) iniFile.GetLongValue ("Main", "windowInputFocus", -1);
	if (windowInputFocus == -1)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "windowInputFocus"));

	auto windowAllowHighDPIResult = static_cast<int>(iniFile.GetLongValue ("Main", "windowAllowHighDPI", -1));
	if (windowAllowHighDPIResult == -1)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "windowAllowHighDPI"));
	windowAllowHighDPI = windowAllowHighDPIResult;

	whichRenderer = (int) iniFile.GetLongValue ("Main", "whichRenderer", -1);
	if (whichRenderer == -1)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "whichRenderer"));

	auto presentVSyncResult = static_cast<int>(iniFile.GetLongValue ("Main", "presentVSync", -1));
	if (presentVSyncResult == -1)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "presentVSync"));
	presentVSync = presentVSyncResult;

	renderScaleQuality = (int) iniFile.GetLongValue ("Main", "renderScaleQuality", -1);
	if (renderScaleQuality == -1)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "renderScaleQuality"));

	g_volumeLevel = (int) iniFile.GetLongValue ("Main", "g_volumeLevel", -1);
	if (g_volumeLevel == -1)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "g_volumeLevel"));

	g_speakerType = (int) iniFile.GetLongValue ("Main", "g_speakerType", -1);
	if (g_speakerType == -1)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "g_speakerType"));

	maxNumChannels = (int) iniFile.GetLongValue ("Main", "maxNumChannels", -1);
	if (g_volumeLevel == -1)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "maxNumChannels"));

	currentLanguage = (int) iniFile.GetLongValue ("Main", "currentLanguage", -1);
	if (currentLanguage == -1)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "currentLanguage"));

	sideviewDrawScale = iniFile.GetDoubleValue ("Main", "sideviewDrawScale", -1);
	if (sideviewDrawScale == -1)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "sideviewDrawScale"));

	auto enableSoundResult = static_cast<int>(iniFile.GetLongValue ("Main", "enableSound", -1));
	if (enableSoundResult == -1)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "enableSound"));
	enableSound = enableSoundResult;

	auto doScreenEffectResult = static_cast<int>(iniFile.GetLongValue ("Main", "doScreenEffect", -1));
	if (doScreenEffectResult == -1)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "doScreenEffect"));
	doScreenEffect = doScreenEffectResult;

	returnValue = iniFile.GetValue ("Main", "tileStyle", "default");
	if (strcmp (returnValue, "default") == 0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "tileStyle"));
	tileStyle = returnValue;

	returnValue = iniFile.GetValue ("Main", "tileColor", "default");
	if (strcmp (returnValue, "default") == 0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in config file.", "tileColor"));
	tileColor = returnValue;
}
