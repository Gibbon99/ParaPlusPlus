#include "game/texture.h"
#include "game/tiles.h"

std::string tileFilename {};
std::string tileStyle {};
std::string tileColor {};

//----------------------------------------------------------------------------------------------------------------------
//
// Set the passed in tile style
void gam_setTileType(bool useDarkTiles)
//----------------------------------------------------------------------------------------------------------------------
{
	if (!useDarkTiles)
		tileFilename = tileStyle + "_" + tileColor + ".bmp";
	else
		tileFilename = tileStyle + "_" + "dark" + ".bmp";
}

//----------------------------------------------------------------------------------------------------------------------
//
// Load the tile set
void gam_loadTileSet()
//----------------------------------------------------------------------------------------------------------------------
{
	std::string tileKeyName;

	tileKeyName = "tiles";

	gam_loadTexture (tileFilename, tileKeyName);
}