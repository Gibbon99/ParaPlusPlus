#pragma once

#include "main.h"
#include <physfs.h>

struct RWCloser
{
	void operator()(SDL_RWops *p) const
	{
		SDL_RWclose (p);
	}
};

using UniqueRWops = std::unique_ptr<SDL_RWops, RWCloser>;

// Find the names of all the files in the scripts directory and store ready for loading
void io_getScriptFileNames(std::string directoryName);

// Load function callback - this function loads a file and returns a pointer to memory
SDL_RWops *io_loadRawFile(const std::string& fileName);

// Return a file pointer to a block of memory to be used like a file
//SDL_RWops *io_openMemFile(char *memoryPointer, int memorySize);
UniqueRWops io_openMemFile(char *memoryPointer, int memorySize);
