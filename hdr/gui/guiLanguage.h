#pragma once

#include "main.h"

extern int currentLanguage;

// Return a language string based on the key
std::string gui_getLanguageString(const std::string& key);

// Return a string based on the key
std::string gui_getString(const std::string &key);

// Add a key and string to the map
void gui_addKeyAndText(std::string &key, std::string &textString);

// Reset the language array
void gui_resetLanguage();