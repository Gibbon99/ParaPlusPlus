#pragma once

#include "../../hdr/main.h"

// Pass in string and parameters to format and return a string
// https://stackoverflow.com/questions/19009094/c-variable-arguments-with-stdstring-only
std::string sys_getString (std::string format, ...);

// Allocate memory and return pointer - record the size as well
char *sys_malloc (int memorySize, std::string keyName);

// Free a memory slot by name
void sys_freeMemory (std::string keyName);

// Go through memory map and free allocations
void sys_freeMemory ();

// Get the information about the available renderers
void sys_getRendererInfo();

// Set a new mode
void sys_setNewMode (int newMode, bool doFade);

// Get the operating system we are running on
void sys_getOS();