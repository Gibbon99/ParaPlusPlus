#pragma once

#include "../main.h"
#include <physfs.h>

// Find the names of all the files in the scripts directory and store ready for loading
void io_getScriptFileNames(const std::string& directoryName);
