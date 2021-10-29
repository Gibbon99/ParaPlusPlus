#pragma once

#include "main.h"

extern int currentLanguage;

// Return a string based on the key
std::string gui_getString(const std::string &key);

// Add a key and string to the map
void gui_addKeyAndText(std::string &key, std::string &textString);