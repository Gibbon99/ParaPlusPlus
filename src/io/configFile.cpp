#include "../../hdr/classes/configFile.h"
#include "../../hdr/system/shutdown.h"

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

	if (!iniFile.GetValueToInt("Main", "virtualWinWidth", (int &)virtualWinWidth, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "virtualWinWidth"));

	if (!iniFile.GetValueToInt("Main", "virtualWinHeight", (int &)virtualWinHeight, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "virtualWinHeight"));

	if (!iniFile.GetValueToInt("Main", "windowWidth", (int &)windowWidth, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "windowWidth"));

	if (!iniFile.GetValueToInt("Main", "windowHeight", (int &)windowHeight, 0))
		sys_shutdownWithError(sys_getString("Unable to locate value [ %s ] in config file.", "windowHeight"));

	console.add(sys_getString("virtualWinWidth [ %i ]", virtualWinWidth));
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