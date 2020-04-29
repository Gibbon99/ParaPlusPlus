#include "../classes/SimpleIni.h"

#ifndef PARA_CONFIGFILE_H
#define PARA_CONFIGFILE_H

// Read values from config ini file
void io_readConfigValues(const std::string& fileName);

// Update a value to be saved into the config file - STRING version
void io_updateConfigValue(std::string keyName, std::string newValue);

// Update a value to be saved into the config file - INT version
void io_updateConfigValue(std::string keyName, int newValue);

// Save the in memory config file to disk
void io_saveConfigValues();

#endif //PARA_CONFIGFILE_H
