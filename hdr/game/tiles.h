#pragma once

#include "main.h"

extern std::string tileFilename;
extern std::string tileStyle;
extern std::string tileColor;

// Set the passed in tile style
void gam_setTileType (bool useDarkTiles);

// Load the tile set
void gam_loadTileSet();