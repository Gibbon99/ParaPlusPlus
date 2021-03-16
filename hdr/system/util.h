#pragma once

#include "main.h"
#include "game/shipDecks.h"
#include "system/startup.h"
#include "io/console.h"

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

// Save a SDL_Texture to a file
void sys_saveTexture(SDL_Renderer *ren, SDL_Texture *tex, const char *filename);

// Convert worldPosition coords to screen coords
b2Vec2 sys_worldToScreen (b2Vec2 worldPos, int shapeSize);

// Is an object visible on the screen
bool sys_visibleOnScreen (b2Vec2 worldCoord, int shapeSize);

// Convert coords to pixels from meters
b2Vec2 sys_convertToMeters(b2Vec2 convertFrom);

// Convert coords from meters to pixels
b2Vec2 sys_convertToPixels(b2Vec2 convertFrom);

// Convert from pixels to tileSize coordinates
b2Vec2 sys_convertToTiles(b2Vec2 convertFrom);

// Convert a string to an integer value
int sys_convertToInt(std::string convertFrom);