#include <gui/guiRender.h>
#include <gui/guiLanguage.h>
#include "../../hdr/io/configFile.h"
#include "../../hdr/system/startup.h"

CSimpleIniA iniFile;
std::string configFileName = "";

//--------------------------------------------------------------------------------------------------------------------
//
// Save the in memory config file to disk
void io_saveConfigValues()
//--------------------------------------------------------------------------------------------------------------------
{
	auto returnCode = iniFile.Save(configFileName);
	if (returnCode < 0)
		sys_shutdownWithError(sys_getString("Unable to save config file [ %s ]", configFileName.c_str()));
}

//--------------------------------------------------------------------------------------------------------------------
//
// Update a value to be saved into the config file - INT version
void io_updateConfigValue(std::string keyName, int newValue)
//--------------------------------------------------------------------------------------------------------------------
{
	auto returnCode = iniFile.SetValue("Main", keyName.c_str(), sys_getString("%i", newValue).c_str());
	if (returnCode < 0)
		sys_shutdownWithError(sys_getString("Unable to update config file value [ %s ]", keyName.c_str()));
}

//--------------------------------------------------------------------------------------------------------------------
//
// Update a value to be saved into the config file - STRING version
void io_updateConfigValue(std::string keyName, std::string newValue)
//--------------------------------------------------------------------------------------------------------------------
{
	auto returnCode = iniFile.SetValue("Main", keyName.c_str(), newValue.c_str());
	if (returnCode < 0)
		sys_shutdownWithError(sys_getString("Unable to update config file value [ %s ]", keyName.c_str()));
}


//--------------------------------------------------------------------------------------------------------------------
//
// Read values from config ini file
void io_readConfigValues(const std::string& fileName)
//--------------------------------------------------------------------------------------------------------------------
{
	configFileName = fileName;

	auto returnCode = iniFile.LoadFile(fileName.c_str());
	if (returnCode < 0)
	{
		sys_shutdownWithError("Unable to open config file.");
	}

	auto returnValue = iniFile.GetValue ("Main", "consoleFontFilename", "default");
	if (strcmp(returnValue, "default") == 0)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "consoleFontFilename"));
	consoleFontFilename = returnValue;

	returnValue = iniFile.GetValue ("Main", "guiFontFileName", "default");
	if (strcmp(returnValue, "default") == 0)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "guiFontFileName"));
	guiFontFileName = returnValue;

	guiFontSize = (int)iniFile.GetLongValue ("Main", "guiFontSize", 0);
	if (guiFontSize == 0)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "guiFontSize"));

	consoleVirtualWidth = (int)iniFile.GetLongValue ("Main", "consoleVirtualWidth", 0);
	if (consoleVirtualWidth == 0)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "consoleVirtualWidth"));

	consoleVirtualHeight = (int)iniFile.GetLongValue("Main", "consoleVirtualHeight", 0);
	if (consoleVirtualHeight == 0)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "consoleVirtualHeight"));

	consoleNumColumns = (int)iniFile.GetLongValue("Main", "consoleNumColumns", 0);
	if (consoleNumColumns == 0)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "consoleNumColumns"));

	consoleFontSize = (int)iniFile.GetLongValue("Main", "consoleFontSize", 0);
	if (consoleFontSize == 0)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "consoleFontSize"));

	logicalWinWidth = (int)iniFile.GetLongValue("Main", "logicalWinWidth", 0);
	if (logicalWinWidth == 0)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "logicalWinWidth"));

	logicalWinHeight = (int)iniFile.GetLongValue("Main", "logicalWinHeight", 0);
	if (logicalWinHeight == 0)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "logicalWinHeight"));

	windowWidth = (int)iniFile.GetLongValue("Main", "windowWidth", 0);
	if (windowWidth == 0)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "windowWidth"));

	windowHeight = (int)iniFile.GetLongValue("Main", "windowHeight", 0);
	if (windowHeight == 0)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "windowHeight"));

	windowFullscreen = (int)iniFile.GetLongValue("Main", "windowFullscreen", -1);
	if (windowFullscreen == -1)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "windowFullscreen"));

	windowFullscreenDesktop = (int)iniFile.GetLongValue("Main", "windowFullscreenDesktop", -1);
	if (windowFullscreenDesktop == -1)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "windowFullscreenDesktop"));

	windowBorderless = (int)iniFile.GetLongValue("Main", "windowBorderless", -1);
	if (windowBorderless == -1)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "windowBorderless"));

	windowInputGrabbed = (int)iniFile.GetLongValue("Main", "windowInputGrabbed", -1);
	if (windowInputGrabbed == -1)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "windowInputGrabbed"));

	windowInputFocus = (int)iniFile.GetLongValue("Main", "windowInputFocus", -1);
	if (windowInputFocus == -1)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "windowInputFocus"));

	windowAllowHighDPI = (int)iniFile.GetLongValue("Main", "windowAllowHighDPI", -1);
	if (windowAllowHighDPI == -1)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "windowAllowHighDPI"));

	whichRenderer = (int)iniFile.GetLongValue("Main", "whichRenderer", -1);
	if (whichRenderer == -1)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "whichRenderer"));

	presentVSync = (int)iniFile.GetLongValue("Main", "presentVSync", -1);
	if (presentVSync == -1)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "presentVSync"));

	renderScaleQuality = (int)iniFile.GetLongValue("Main", "renderScaleQuality", -1);
	if (renderScaleQuality == -1)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "renderScaleQuality"));

	volumeLevel = (int)iniFile.GetLongValue("Main", "volumeLevel", -1);
	if (volumeLevel == -1)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "volumeLevel"));

	maxNumChannels = (int)iniFile.GetLongValue("Main", "maxNumChannels", -1);
	if (volumeLevel == -1)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "maxNumChannels"));

	currentLanguage = (int)iniFile.GetLongValue("Main", "currentLanguage", -1);
	if (currentLanguage == -1)
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "currentLanguage"));
}

