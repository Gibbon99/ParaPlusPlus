#include <system/startup.h>
#include <io/console.h>
#include <game/healing.h>
#include <game/lifts.h>
#include <game/player.h>
#include <system/util.h>
#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include "game/shipDecks.h"
#include "game/doors.h"
#include "game/terminal.h"

int                                          tileSize;
int                                          numTileAcrossInTexture = 8;
int                                          numTilesDownInTexture  = 8;
paraVec2i                                    drawOffset;
std::unordered_map<std::string, _deckStruct> shipdecks;
PARA_Texture *playfieldTexture;
std::string                                  currentDeckName;
SDL_Rect                                     viewportRect;
int                                          currentDeckNumber;
std::vector<int>                             influenceMap;
bool                                         d_showInfluenceMap = false;

//----------------------------------------------------------------------------------------------------------------------
//
// Get the value from the influence map
int gam_getInfluenceMapValue (int tileIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	if (tileIndex > influenceMap.size ())
		sys_shutdownWithError ("tileIndex greater than influenceMap size.");

	return influenceMap[tileIndex];
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set influence map values
void gam_setInfluenceValues (b2Vec2 startPos, int value, int size)
//----------------------------------------------------------------------------------------------------------------------
{
	paraVec2d topRight;
	int       finalSize;

	finalSize = size * 2;

	influenceMap[startPos.y * (shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.x) + startPos.x] = value;

	topRight.x = startPos.x + size;
	topRight.y = startPos.y + size;

	for (auto i = 0; i != finalSize; i++)
	{
		topRight.y--;
		influenceMap[topRight.y * (shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.x) + topRight.x] = value;
	}

	for (auto i = 0; i != finalSize; i++)
	{
		topRight.x--;
		influenceMap[topRight.y * (shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.x) + topRight.x] = value;
	}

	for (auto i = 0; i != finalSize; i++)
	{
		topRight.y++;
		influenceMap[topRight.y * (shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.x) + topRight.x] = value;
	}

	for (auto i = 0; i != finalSize; i++)
	{
		topRight.x++;
		influenceMap[topRight.y * (shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.x) + topRight.x] = value;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Change player pixel coords into tile coords
void gam_populateInfluenceMap (b2Vec2 playerPositionInPixels)
//----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2 newPosition;

	std::fill (influenceMap.begin (), influenceMap.end (), 0);

	newPosition.x = static_cast<int>(playerPositionInPixels.x) / tileSize;
	newPosition.y = static_cast<int>(playerPositionInPixels.y) / tileSize;

	gam_setInfluenceValues (newPosition, 15, 2);
	gam_setInfluenceValues (newPosition, 25, 1);
//	gam_setInfluenceValues (newPosition, 20, 3);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the influence map to be same size as deck dimensions - call on deck change
void gam_createInfluenceMap ()
//----------------------------------------------------------------------------------------------------------------------
{
	//
	// Create the influence map and set to 0 values
	influenceMap.resize (shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.x * shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.y);
	std::fill (influenceMap.begin (), influenceMap.end (), 0);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Debug influence map
void gam_debugInfluenceMap ()
//----------------------------------------------------------------------------------------------------------------------
{
	int       influenceValue;
	paraVec2d drawPosition;

	for (auto countY = 0; countY != shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.y; countY++)
	{
		for (auto countX = 0; countX != shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.x; countX++)
		{
			influenceValue = influenceMap[(countY * shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.x) + countX];
			if (influenceValue > 0)
			{
				drawPosition.x = countX * tileSize;
				drawPosition.y = countY * tileSize;

				drawPosition = sys_worldToScreen (drawPosition, tileSize);

				boxRGBA (renderer.renderer, drawPosition.x, drawPosition.y, drawPosition.x + tileSize, drawPosition.y + tileSize, 30 + ((influenceValue / 10) * 40), 0, 0, 60);
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the playfield texture
PARA_Texture *gam_getPlayfieldTexture ()
//----------------------------------------------------------------------------------------------------------------------
{
	return playfieldTexture;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the deck index number based on current deck name
int gam_getCurrentDeckIndex ()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto deckItr : shipdecks)
	{
		if (deckItr.first == currentDeckName)
			return deckItr.second.deckNumber;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the current deck name
std::string gam_getCurrentDeckName ()
//----------------------------------------------------------------------------------------------------------------------
{
	return currentDeckName;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Convert current tile information into padded array
// Suitable for display on current screen size
void gam_addPaddingToLevel (const std::string fileName)
//----------------------------------------------------------------------------------------------------------------------
{
	std::vector<int> tempLevel;
	paraVec2d        tempDimensions{};
	int              countY, countX, whichTile;
	int              destX, destY;
	std::string      levelName;

	drawOffset.x = (gameWinWidth / tileSize);
	drawOffset.y = (gameWinHeight / tileSize);

	destY = drawOffset.y / 2;

	tempDimensions.x = shipdecks.at (fileName).levelDimensions.x;
	tempDimensions.y = shipdecks.at (fileName).levelDimensions.y;

	tempLevel.reserve ((shipdecks.at (fileName).levelDimensions.x + drawOffset.x) * (shipdecks.at (fileName).levelDimensions.y + drawOffset.y));

	tempLevel.assign ((shipdecks.at (fileName).levelDimensions.x + drawOffset.x) * (shipdecks.at (fileName).levelDimensions.y + drawOffset.y), 0);

	for (countY = 0; countY != shipdecks.at (fileName).levelDimensions.y; countY++)
	{
		destX = drawOffset.x / 2;

		for (countX = 0; countX != shipdecks.at (fileName).levelDimensions.x; countX++)
		{
			whichTile = shipdecks.at (fileName).tiles[(countY * shipdecks.at (fileName).levelDimensions.x) + countX];
			tempLevel[(destY * (tempDimensions.x + drawOffset.x)) + destX] = whichTile;
			destX++;
		}
		destY++;
	}

	tempDimensions.x += drawOffset.x;
	tempDimensions.y += drawOffset.y;

	shipdecks.at (fileName).levelDimensions.x = tempDimensions.x;
	shipdecks.at (fileName).levelDimensions.y = tempDimensions.y;

	shipdecks.at (fileName).tiles.clear ();

	shipdecks.at (fileName).tiles.reserve (tempDimensions.x * tempDimensions.y);

	shipdecks.at (fileName).tiles.assign (tempDimensions.x * tempDimensions.y, 0);

	for (int i = 0; i != tempDimensions.x * tempDimensions.y; i++)
	{
		shipdecks.at (fileName).tiles[i] = tempLevel[i];
	}
	tempLevel.clear ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Populate the shipdeck structure from a file in memory
void gam_loadShipDeck (const std::string &fileName)
//----------------------------------------------------------------------------------------------------------------------
{
	SDL_RWops    *fp;
	int          checkVersion;
	_deckStruct  tempLevel;
	_lineSegment tempSegment{};
	paraVec2d    tempWaypoint{};
	int          tempDroidType;
	int          tempTile;
	int          fileSize;
	char         *memoryBuffer;
	float        tempFloat;

	try
	{
		tileSize = textures.at ("tiles").getWidth () / numTileAcrossInTexture;
	}
	catch (std::out_of_range outOfRange)
	{
		//
		// Tiles texture is not loaded yet, resubmit the load request with a time delay
		sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_DECK, 1, fileName);
		return;
	}

	drawOffset.x = gameWinWidth / 2;     // Padding to make tilePosX always positive
	drawOffset.y = gameWinHeight / 2;    // Padding to make tilePosY always positive

	fileSize = fileSystem.getFileSize (fileName);
	if (fileSize < 0)
		sys_shutdownWithError (sys_getString ("Fatal error getting level file size [ %s ].", fileName.c_str ()));

	memoryBuffer = sys_malloc (static_cast<int>(sizeof (char) * fileSize), fileName);
	if (nullptr == memoryBuffer)
		sys_shutdownWithError (sys_getString ("Fatal memory allocation error when loading level file."));

	fileSystem.getFileIntoMemory (fileName, memoryBuffer);
	//
	// Open the block of memory and read like a file
	fp = SDL_RWFromConstMem (memoryBuffer, fileSize);
	if (nullptr == fp)
		sys_shutdownWithError (sys_getString ("Mapping memory to file failed for file [ %s ]", fileName.c_str ()));
	//
	// Check this version is ok to use
	SDL_RWread (fp, &checkVersion, sizeof (checkVersion), 1);
	if (checkVersion != MAP_VERSION)
		sys_shutdownWithError (sys_getString ("MAP_VERSION wrong for file. Wanted [ %i ] got from file [ %i ]", MAP_VERSION, checkVersion));
	//
	// Read number variables
	SDL_RWread (fp, (void *) &tempLevel.numLineSegments, sizeof (tempLevel.numLineSegments), 1);
	SDL_RWread (fp, (void *) &tempLevel.numWaypoints, sizeof (tempLevel.numWaypoints), 1);
	SDL_RWread (fp, (void *) &tempLevel.numDroids, sizeof (tempLevel.numDroids), 1);
	SDL_RWread (fp, (void *) &tempLevel.numLifts, sizeof (tempLevel.numLifts), 1);

	SDL_RWread (fp, &tempFloat, sizeof (tempFloat), 1);
	tempLevel.levelDimensions.x = tempFloat;

	SDL_RWread (fp, &tempFloat, sizeof (tempFloat), 1);
	tempLevel.levelDimensions.y = tempFloat;

	//
	// Line segments for physics collisions
	for (int i = 0; i != tempLevel.numLineSegments; i++)
	{
		SDL_RWread (fp, &tempFloat, sizeof (tempFloat), 1);
		tempSegment.start.x = tempFloat;

		SDL_RWread (fp, &tempFloat, sizeof (tempFloat), 1);
		tempSegment.start.y = tempFloat;

		SDL_RWread (fp, &tempFloat, sizeof (tempFloat), 1);
		tempSegment.finish.x = tempFloat;

		SDL_RWread (fp, &tempFloat, sizeof (tempFloat), 1);
		tempSegment.finish.y = tempFloat;

		tempSegment.start.x += drawOffset.x;
		tempSegment.start.y += drawOffset.y;

		tempSegment.finish.x += drawOffset.x;
		tempSegment.finish.y += drawOffset.y;

		tempLevel.lineSegments.push_back (tempSegment);
	}
	//
	// Waypoints for Droid patrol
	for (int i = 0; i != tempLevel.numWaypoints; i++)
	{
		SDL_RWread (fp, &tempFloat, sizeof (tempFloat), 1);
		tempWaypoint.x = tempFloat;

		SDL_RWread (fp, &tempFloat, sizeof (tempFloat), 1);
		tempWaypoint.y = tempFloat;

		tempWaypoint.x += drawOffset.x;
		tempWaypoint.y += drawOffset.y;

		paraVec2d tempVec2{};

		tempVec2.x = tempWaypoint.x;
		tempVec2.y = tempWaypoint.y;

		tempLevel.wayPoints.push_back (tempVec2);
	}

	//
	// Load each droid type on the current level
	for (int i = 0; i != tempLevel.numDroids; i++)
	{
		SDL_RWread (fp, &tempDroidType, sizeof (tempDroidType), 1);
		tempLevel.droidTypes.push_back (tempDroidType);
	}
	//
	// Array holding tile types
	for (int i = 0; i != tempLevel.levelDimensions.x * tempLevel.levelDimensions.y; i++)
	{
		SDL_RWread (fp, &tempTile, sizeof (tempTile), 1);
		tempLevel.tiles.push_back (tempTile);
	}
	SDL_RWread (fp, &tempLevel.levelName, sizeof (tempLevel.levelName), 1);

	//
	// Finished - close the file
	SDL_RWclose (fp);
	sys_freeMemory (fileName);
	//
	// Extract the deck number from the filename
	std::string output     = fileName;
	std::string removePart = "116-newDeck";

	output.erase (0, removePart.size ());
	std::string::size_type idx = output.rfind ('.');

	if (idx != std::string::npos)
	{
		output.erase (idx, 4);
		tempLevel.deckNumber = std::stoi (output, nullptr, 10);
	}
	else
		sys_shutdownWithError (sys_getString ("Unable to parse deck filename for index [ %s ]", fileName.c_str ()));

	// Generate physics masks and categories
	tempLevel.wallPhysicsCreated  = false;
	tempLevel.droidPhysicsCreated = false;

	shipdecks.insert (std::pair<std::string, _deckStruct> (tempLevel.levelName, tempLevel));

	gam_addPaddingToLevel (tempLevel.levelName);

	gam_findHealingTiles (tempLevel.levelName);

	if (shipdecks.size () == 20)
		gam_setupLifts ();

	con_addEvent (-1, sys_getString ("Loaded ship level [ %s ] - index [ %i ]", tempLevel.levelName, tempLevel.deckNumber));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a single tile from the sheet to the playfield texture
void gam_renderSingleTile (int destX, int destY, int tileIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	static int      previousTileIndex = -1;
	static SDL_Rect sourceRect;
	SDL_Rect        destRect;

	if (previousTileIndex != tileIndex)
	{
		previousTileIndex = tileIndex;

		sourceRect.x = (tileIndex % numTileAcrossInTexture) * tileSize;
		sourceRect.y = (tileIndex / numTilesDownInTexture) * tileSize;
		sourceRect.w = tileSize;
		sourceRect.h = tileSize;
	}

	destRect.x = destX;
	destRect.y = destY;
	destRect.w = tileSize;
	destRect.h = tileSize;

	SDL_RenderCopy (renderer.renderer, textures.at ("tiles").getTexture (), &sourceRect, &destRect);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the texture to represent the current deck level
void gam_createDeckTexture (std::string deckName)
//----------------------------------------------------------------------------------------------------------------------
{
	int tileIndex;

	static bool writeFile = false;

	if (playfieldTexture != nullptr)
		SDL_DestroyTexture (playfieldTexture);

	playfieldTexture = SDL_CreateTexture (renderer.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, shipdecks.at (deckName).levelDimensions.x * tileSize, shipdecks.at (deckName).levelDimensions.y * tileSize);
	if (nullptr == playfieldTexture)
		sys_shutdownWithError (sys_getString ("Unable to create playfield texture [ %s ].", SDL_GetError ()));

	SDL_SetRenderTarget (renderer.renderer, playfieldTexture);
	SDL_SetRenderDrawColor (renderer.renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear (renderer.renderer);

	for (auto countY = 0; countY != shipdecks.at (deckName).levelDimensions.y; countY++)
	{
		for (auto countX = 0; countX != shipdecks.at (deckName).levelDimensions.x; countX++)
		{
			tileIndex = shipdecks.at (deckName).tiles[(countY * shipdecks.at (deckName).levelDimensions.x) + countX];
			if (tileIndex > 0)
				gam_renderSingleTile (countX * tileSize, countY * tileSize, tileIndex);
		}
	}
	SDL_SetRenderTarget (renderer.renderer, nullptr);

/*
	if (false == writeFile)
	{
		writeFile = true;
		sys_saveTexture(renderer.renderer, playfieldTexture, "playField.bmp");
	}
*/
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the levelName from the passed in deckNumber
std::string gam_returnLevelNameFromDeck (int deckNumber)
//----------------------------------------------------------------------------------------------------------------------
{
//
// Using   for (auto levelItr : shipLevel)
// Results in massive FPS drop
//
	for (auto const &levelItr : shipdecks)
	{
		if (deckNumber == levelItr.second.deckNumber)
		{
			return levelItr.second.levelName;
		}
	}
	sys_shutdownWithError (sys_getString ("Invalid deck index when searching for deck name [ %i ]", deckNumber));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Change to a new deck
void gam_changeToDeck (const std::string &deckName, int whichLift)
//----------------------------------------------------------------------------------------------------------------------
{
	currentDeckName = deckName;
	gam_createDeckTexture (deckName);

	sys_setupSolidWalls (deckName);
	gam_doorTriggerSetup (deckName);
	gam_findTerminalPositions (deckName);
	gam_findLiftPositions (deckName);

	playerDroid.previousWorldPosInPixels = {-1, -1};
	playerDroid.worldPosInPixels         = gam_getLiftWorldPosition (whichLift, deckName);
	sys_setPlayerPhysicsPosition (playerDroid.worldPosInPixels);

	gam_createInfluenceMap ();

	currentDeckNumber = gam_getCurrentDeckIndex ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render the current visible area to the backing texture
void gam_renderVisibleScreen (double interpolation)
//----------------------------------------------------------------------------------------------------------------------
{
	SDL_Rect  sourceRect;
	paraVec2d renderPosition{};

	viewportRect.x = playerDroid.worldPosInPixels.x - (gameWinWidth / 2);
	viewportRect.y = playerDroid.worldPosInPixels.y - (gameWinHeight / 2);
	viewportRect.w = gameWinWidth;
	viewportRect.h = gameWinHeight;

	renderPosition.x = playerDroid.worldPosInPixels.x - playerDroid.previousWorldPosInPixels.x;
	renderPosition.y = playerDroid.worldPosInPixels.y - playerDroid.previousWorldPosInPixels.y;

	renderPosition *= interpolation;
	renderPosition.x += playerDroid.previousWorldPosInPixels.x;
	renderPosition.y += playerDroid.previousWorldPosInPixels.y;

	sourceRect.x = renderPosition.x - (gameWinWidth / 2);
	sourceRect.y = renderPosition.y - (gameWinHeight / 2);
	sourceRect.w = gameWinWidth;
	sourceRect.h = gameWinHeight;

	renderer.setCurrentBackingTexture (GAME_BACKING_TEXTURE);
	SDL_RenderSetLogicalSize (renderer.renderer, gameWinWidth, gameWinHeight);
	//
	// Rendercopy Float still uses Integer values for source...
	SDL_RenderCopyF (renderer.renderer, playfieldTexture, &sourceRect, nullptr);
}
