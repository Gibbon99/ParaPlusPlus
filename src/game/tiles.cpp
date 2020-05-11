#include <game/texture.h>
#include "game/tiles.h"

std::string tileFilename;
std::string tileStyle;
std::string tileColor;

//----------------------------------------------------------------------------------------------------------------------
//
// Set the passed in tile style
void gam_setTileType ()
//----------------------------------------------------------------------------------------------------------------------
{
	tileFilename = tileStyle + "_" + tileColor + ".bmp";
}

//----------------------------------------------------------------------------------------------------------------------
//
// Load the tile set
void gam_loadTileSet()
//----------------------------------------------------------------------------------------------------------------------
{
	gam_loadTexture(tileFilename, "tiles");
}