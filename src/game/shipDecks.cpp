#include <system/startup.h>
#include <io/console.h>
#include <game/healing.h>
#include <game/lifts.h>
#include <game/player.h>
#include <system/util.h>
#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include <game/bullet.h>
#include <system/gameEvents.h>
#include <game/lightMaps.h>
#include <game/tiles.h>
#include <game/texture.h>
#include <game/audio.h>
#include <game/score.h>
#include "game/shipDecks.h"
#include "game/doors.h"
#include "game/terminal.h"
#include "game/game.h"
#include "game/alertLevel.h"

int                                                    tileSize;
int                                                    numTileAcrossInTexture = 8;
int                                                    numTilesDownInTexture  = 8;
int                                                    powerdownLevelScore;
b2Vec2                                                 drawOffset;
std::string                                            currentDeckName;
SDL_Rect                                               viewportRect;
int                                                    currentDeckNumber;
std::vector<int>                                       influenceMap;
bool                                                   d_showInfluenceMap     = false;

PARA_Texture *playfieldTexture;
std::unordered_map<std::string, _deckStruct>           shipdecks;
std::unordered_map<std::string, _deckStruct>::iterator g_shipDeckItr;

//----------------------------------------------------------------------------------------------------------------------
//
// Get the value from the influence map
int gam_getInfluenceMapValue (int tileIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	if (tileIndex > static_cast<int>(influenceMap.size ()))
		sys_shutdownWithError ("tileIndex greater than influenceMap size.");

	return influenceMap[tileIndex];
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set influence map values
void gam_setInfluenceValues (b2Vec2 startPos, int value, int size)
//----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2 topRight;
	int    finalSize;

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
	int    influenceValue;
	b2Vec2 drawPosition;

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

	sys_shutdownWithError("Unable to find current deck index.");
	return -1;
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
	b2Vec2           tempDimensions{};
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
	b2Vec2       tempWaypoint{};
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

		b2Vec2 tempVec2{};

		tempVec2.x = tempWaypoint.x - (tileSize / 2);
		tempVec2.y = tempWaypoint.y - (tileSize / 2);

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

	gam_initDroids (tempLevel.levelName);

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

		sourceRect.x = static_cast<float>((tileIndex % numTileAcrossInTexture)) * tileSize;
		sourceRect.y = (tileIndex / numTilesDownInTexture) * tileSize;
		sourceRect.w = tileSize;
		sourceRect.h = tileSize; // - 1;

//		sourceRect.y += 1;
//		sourceRect.h -= 1;
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
// Using   for (auto levelItr : shipdecks)
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

	return "Compiler happy";
}

//----------------------------------------------------------------------------------------------------------------------
//
// Change to a new deck
void gam_changeToDeck (string deckName, int whichLift)
//----------------------------------------------------------------------------------------------------------------------
{
	std::string tempFilename;
	std::string tempTiles;

	tempTiles = "tiles";

	if (deckName.empty())
		deckName = "Staterooms";

	gam_clearGameEvents ();
	//
	// Clear out droid physics before changing level name
	sys_clearDroidPhysics (currentDeckName);

	currentDeckName   = std::string (deckName);
	currentDeckNumber = gam_getCurrentDeckIndex ();
	g_shipDeckItr     = shipdecks.find (currentDeckName);

	gam_setTileType (g_shipDeckItr->second.deckIsDead);     // Set which tileset to use, normal or dark for dead level
	textures.at ("tiles").destroy ();                     // Free previous texture
	tempFilename = tileFilename;
	gam_loadTexture (tempFilename, tempTiles);        // Needs to be called directly. Putting it in the event queue doesn't happen until after the deck texture is created

	// Textures
	gam_removeAllLightmaps ();
	gam_locateAlertTiles ();
	gam_createDeckTexture (deckName);
	gam_renderAlertTiles ();

	// Physics
	sys_setupEnemyPhysics (deckName);
	sys_setupSolidWalls (deckName);
	gam_doorTriggerSetup ();
	gam_findTerminalPositions (deckName);
	gam_findLiftPositions (deckName);
	gam_findHealingTilesPhysics ();

	// Bullets
	gam_initBulletArray ();

	playerDroid.previousWorldPosInPixels = {-1, -1};
	playerDroid.worldPosInPixels         = gam_getLiftWorldPosition (whichLift);
	sys_setPlayerPhysicsPosition (playerDroid.worldPosInPixels);

	// AI
	gam_createInfluenceMap ();

	// Sounds
	gam_startAlertLevelSound (gam_getCurrentAlertLevel ());
//	gam_checkPlayerHealth();    // See if low energy sound needs to restart
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render the current visible area to the backing texture
void gam_renderVisibleScreen (double interpolation)
//----------------------------------------------------------------------------------------------------------------------
{
	SDL_Rect sourceRect;
	b2Vec2   renderPosition{};

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
//	SDL_RenderSetLogicalSize (renderer.renderer, gameWinWidth, gameWinHeight);
	//
	// Rendercopy Float still uses Integer values for source...
	SDL_RenderCopyF (renderer.renderer, playfieldTexture, &sourceRect, nullptr);
}


//-------------------------------------------------------------------------------------------------------------------------
//
// Render the waypoint segments
void gam_showWayPoints (const std::string levelName)
//-------------------------------------------------------------------------------------------------------------------------
{
	int          indexCount;
	b2Vec2       lineStart;
	b2Vec2       lineFinish;
	_lineSegment tempLine;
	b2Vec2       wallStartDraw, wallFinishDraw;

	indexCount = 0;

	for (auto it: shipdecks.at (levelName).wayPoints)
	{
		tempLine.start.x = shipdecks.at (levelName).wayPoints[indexCount].x;
		tempLine.start.y = shipdecks.at (levelName).wayPoints[indexCount].y;

		if (indexCount + 1 < shipdecks.at (levelName).numWaypoints)
		{
			tempLine.finish.x = shipdecks.at (levelName).wayPoints[indexCount + 1].x;
			tempLine.finish.y = shipdecks.at (levelName).wayPoints[indexCount + 1].y;
		}
		else
		{
			tempLine.finish.x = shipdecks.at (levelName).wayPoints[0].x;
			tempLine.finish.y = shipdecks.at (levelName).wayPoints[0].y;
		}

		lineStart.x = static_cast<float>(tempLine.start.x);
		lineStart.y = static_cast<float>(tempLine.start.y);

		lineFinish.x = static_cast<float>(tempLine.finish.x);
		lineFinish.y = static_cast<float>(tempLine.finish.y);

		wallStartDraw.x  = lineStart.x;
		wallStartDraw.y  = lineStart.y;
		wallFinishDraw.x = lineFinish.x;
		wallFinishDraw.y = lineFinish.y;

		wallStartDraw  = sys_worldToScreen (wallStartDraw, 50);
		wallFinishDraw = sys_worldToScreen (wallFinishDraw, 50);

		thickLineRGBA (renderer.renderer, wallStartDraw.x, wallStartDraw.y, wallFinishDraw.x, wallFinishDraw.y, 3, 0, 0, 200, 55);

		roundedRectangleRGBA (renderer.renderer, wallStartDraw.x - 8, wallStartDraw.y - 8, wallStartDraw.x + 8, wallStartDraw.y + 8, 2, 0, 200, 0, 55);

		roundedRectangleRGBA (renderer.renderer, wallFinishDraw.x - 4, wallFinishDraw.y - 4, wallFinishDraw.x + 4, wallFinishDraw.y + 4, 2, 0, 200, 0, 55);

		indexCount++;
	}
}

//-------------------------------------------------------------------------------------------------------------------------
//
// All droids on this deck are dead - set deck to dead
void gam_setDeckIsDead ()
//-------------------------------------------------------------------------------------------------------------------------
{
	std::string tempFilename;
	std::string tempTiles;

	tempTiles = "tiles";        // Tileset keyName - needs to be a string to pass to function

	if (!g_shipDeckItr->second.deckIsDead)
	{
		gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "levelShutdown");
		gam_setTileType (true);
		textures.at ("tiles").destroy ();
		tempFilename = tileFilename;
		gam_loadTexture (tempFilename, tempTiles);
		gam_modifyScore (powerdownLevelScore);
		gam_createDeckTexture (gam_getCurrentDeckName ());      // Recreate the deck texture with new tileset
		gam_renderAlertTiles ();
		g_shipDeckItr->second.deckIsDead = true;
		gam_addEvent(EVENT_ACTION_GAME_CHECK_DECK_CLEAR, 20, "");    // Check all decks
	}
}

//-------------------------------------------------------------------------------------------------------------------------
//
// Check if all the levels are dead - Game Won
void gam_checkAllLevels()
//-------------------------------------------------------------------------------------------------------------------------
{
	for (auto deckItr : shipdecks)
	{
		if (!deckItr.second.deckIsDead)
			return;     // One still got droids on it
	}
	//
	// All the decks are clear
	gam_addEvent(EVENT_ACTION_GAME_WON, 20, "");
}