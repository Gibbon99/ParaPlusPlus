#include "../../hdr/io/configFile.h"
#include "../../hdr/system/shutdown.h"
#include "../../hdr/system/startup.h"

//--------------------------------------------------------------------------------------------------------------------
//
// Read values from config ini file
void io_readConfigValues(const std::string& fileName)
//--------------------------------------------------------------------------------------------------------------------
{
	IniFileParser iniFile;

	if (!iniFile.Open(fileName))
	{
		sys_shutdownWithError(sys_getString("Unable to open config file [ %s ]", fileName.c_str()));
	}

	if (!iniFile.GetValuesToString("Main", "consoleFontFilename", (std::string &)consoleFontFilename))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "consoleFontFilename"));

	if (!iniFile.GetValueToInt("Main", "consoleWinWidth", (int &)consoleWinWidth, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "consoleWinWidth"));

	if (!iniFile.GetValueToInt("Main", "consoleWinHeight", (int &)consoleWinHeight, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "consoleWinHeight"));

	if (!iniFile.GetValueToInt("Main", "consoleNumColumns", (int &)consoleNumColumns, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "consoleNumColumns"));

	if (!iniFile.GetValueToInt("Main", "consoleFontSize", (int &)consoleFontSize, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "consoleFontSize"));

	if (!iniFile.GetValueToInt("Main", "logicalWinWidth", (int &)logicalWinWidth, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "logicalWinWidth"));

	if (!iniFile.GetValueToInt("Main", "logicalWinHeight", (int &)logicalWinHeight, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "logicalWinHeight"));

	if (!iniFile.GetValueToInt("Main", "windowWidth", (int &)windowWidth, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "windowWidth"));

	if (!iniFile.GetValueToInt("Main", "windowHeight", (int &)windowHeight, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "windowHeight"));

	if (!iniFile.GetValueToInt("Main", "windowFullscreen", (int &)windowFullscreen, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "windowFullscreen"));

	if (!iniFile.GetValueToInt("Main", "windowFullscreenDesktop", (int &)windowFullscreenDesktop, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "windowFullscreenDesktop"));

	if (!iniFile.GetValueToInt("Main", "windowBorderless", (int &)windowBorderless, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "windowBorderless"));

	if (!iniFile.GetValueToInt("Main", "windowInputGrabbed", (int &)windowInputGrabbed, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "windowInputGrabbed"));

	if (!iniFile.GetValueToInt("Main", "windowInputFocus", (int &)windowInputFocus, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "windowInputFocus"));

	if (!iniFile.GetValueToInt("Main", "windowAllowHighDPI", (int &)windowAllowHighDPI, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "windowAllowHighDPI"));

	if (!iniFile.GetValueToInt("Main", "whichRenderer", (int &)whichRenderer, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "whichRenderer"));

	if (!iniFile.GetValueToInt("Main", "presentVSync", (int &)presentVSync, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "presentVSync"));

	if (!iniFile.GetValueToInt("Main", "renderScaleQuality", (int &)renderScaleQuality, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "renderScaleQuality"));
}

//std::stoi( str )
//There are version for all flavours of numbers:
// long stol(string), float stof(string), double stod(string),... see http://en.cppreference.com/w/cpp/string/basic_string/stol

/*
#include "iniparser.h"
using IniParser = ini::IniParser;
// ...

// first example (savest)
IniParser ipa;
if(ini.readFile("test.ini")) {
// only if file is successful red
// parse value to std::map
auto ini_data = ipa.parse();

std::string val = ini_data["Test"]["value"] // "120"
ini_data["Test"]["value"] = "90" // set value
ini_data
// add new value
ini_data["New"]["counter"] = 20;

//remove value from existing
ini_data["Test"].erase("value");

if(ipa.writeFile("test.ini", ini_data)) {
// do something if writing was successful ...
}
}
*/