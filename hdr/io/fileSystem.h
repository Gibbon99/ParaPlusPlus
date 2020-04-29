#pragma once

#include "../main.h"
#include <physfs.h>

// Find the names of all the files in the scripts directory and store ready for loading
void io_getScriptFileNames(std::string directoryName);

// Load function callback - this function loads a file and returns a pointer to memory
SDL_RWops * io_loadRawFile(std::string fileName);