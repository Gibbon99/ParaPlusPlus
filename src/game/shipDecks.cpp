#include "sdl2_gfx/SDL2_gfxPrimitives.h"
#include "classes/paraBullet.h"
#include "io/console.h"
#include "system/util.h"
#include "system/gameEvents.h"
#include "system/startup.h"
#include "game/healing.h"
#include "game/lifts.h"
#include "game/player.h"
#include "game/lightMaps.h"
#include "game/tiles.h"
#include "game/texture.h"
#include "game/audio.h"
#include "game/score.h"
#include "game/shipDecks.h"
#include "game/doors.h"
#include "game/terminal.h"
#include "game/alertLevel.h"

int                                                    tileSize;
int                                                    numTileAcrossInTexture = 8;
int                                                    numTilesDownInTexture  = 8;
int                                                    powerdownLevelScore;
cpVect                                                 drawOffset;
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
int gam_getInfluenceMapValue(int tileIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	if (tileIndex > static_cast<int>(influenceMap.size ()))
		sys_shutdownWithError ("tileIndex greater than influenceMap size.");

	return influenceMap[tileIndex];
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set influence map values
void gam_setInfluenceValues(cpVect startPos, int value, int size)
//----------------------------------------------------------------------------------------------------------------------
{
	cpVect topRight;
	int    finalSize;

	finalSize = size * 2;

	influenceMap[(unsigned int) startPos.y * (unsigned int) (shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.x) + (unsigned int) startPos.x] = value;

	topRight.x = startPos.x + (float) size;
	topRight.y = startPos.y + (float) size;

	for (auto i = 0; i != finalSize; i++)
	{
		topRight.y--;
		influenceMap[(unsigned int) topRight.y * (unsigned int) (shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.x) + (unsigned int) topRight.x] = value;
	}

	for (auto i = 0; i != finalSize; i++)
	{
		topRight.x--;
		influenceMap[(unsigned int) topRight.y * (unsigned int) (shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.x) + (unsigned int) topRight.x] = value;
	}

	for (auto i = 0; i != finalSize; i++)
	{
		topRight.y++;
		influenceMap[(unsigned int) topRight.y * (unsigned int) (shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.x) + (unsigned int) topRight.x] = value;
	}

	for (auto i = 0; i != finalSize; i++)
	{
		topRight.x++;
		influenceMap[(unsigned int) topRight.y * (unsigned int) (shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.x) + (unsigned int) topRight.x] = value;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Change player pixel coords into tile coords
void gam_populateInfluenceMap(cpVect playerPositionInPixels)
//----------------------------------------------------------------------------------------------------------------------
{
	cpVect newPosition;

	std::fill (influenceMap.begin (), influenceMap.end (), 0);

	newPosition.x = (playerPositionInPixels.x) / static_cast<float>(tileSize);
	newPosition.y = (playerPositionInPixels.y) / static_cast<float>(tileSize);

	gam_setInfluenceValues (newPosition, 15, 2);
	gam_setInfluenceValues (newPosition, 25, 1);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the influence map to be same size as deck dimensions - call on deck change
void gam_createInfluenceMap()
//----------------------------------------------------------------------------------------------------------------------
{
	//
	// Create the influence map and set to 0 values
	influenceMap.resize (static_cast<int>(shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.x) * static_cast<int>(shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.y));
	std::fill (influenceMap.begin (), influenceMap.end (), 0);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Debug influence map
void gam_debugInfluenceMap()
//----------------------------------------------------------------------------------------------------------------------
{
	int    influenceValue;
	cpVect drawPosition;

	for (auto countY = 0; countY != static_cast<int>(shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.y); countY++)
	{
		for (auto countX = 0; countX != static_cast<int>(shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.x); countX++)
		{
			influenceValue = influenceMap[(countY * static_cast<unsigned int>(shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.x)) + countX];
			if (influenceValue > 0)
			{
				drawPosition.x = static_cast<float>(countX * tileSize);
				drawPosition.y = static_cast<float>(countY * tileSize);

				drawPosition = sys_worldToScreen (drawPosition, tileSize);

				boxRGBA (renderer.renderer, static_cast<short int>(drawPosition.x), static_cast<short int>(drawPosition.y), static_cast<short int>(drawPosition.x + static_cast<float>(tileSize)), static_cast<short int>(drawPosition.y +
				                                                                                                                                                                                                          static_cast<float>(tileSize)),
				         30 + ((influenceValue / 10) * 40), 0, 0, 60);
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the playfield texture
PARA_Texture *gam_getPlayfieldTexture()
//----------------------------------------------------------------------------------------------------------------------
{
	return playfieldTexture;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the deck index number based on current deck name
int gam_getCurrentDeckIndex()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto deckItr: shipdecks)
	{
		if (deckItr.first == currentDeckName)
			return deckItr.second.deckNumber;
	}

	sys_shutdownWithError ("Unable to find current deck index.");
	return -1;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the current deck name
std::string gam_getCurrentDeckName()
//----------------------------------------------------------------------------------------------------------------------
{
	return currentDeckName;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Convert current tile information into padded array
// Suitable for display on current screen size
void gam_addPaddingToLevel(const std::string fileName)
//----------------------------------------------------------------------------------------------------------------------
{
	std::vector<int> tempLevel;
	cpVect           tempDimensions {};
	int              countY, countX, whichTile;
	int              destX, destY;
	std::string      levelName;

	drawOffset.x = static_cast<float>(gameWinWidth / tileSize);
	drawOffset.y = static_cast<float>(gameWinHeight / tileSize);

	destY = static_cast<int>(drawOffset.y / 2);

	tempDimensions.x = shipdecks.at (fileName).levelDimensions.x;
	tempDimensions.y = shipdecks.at (fileName).levelDimensions.y;

	tempLevel.reserve (static_cast<unsigned int>((shipdecks.at (fileName).levelDimensions.x + drawOffset.x) * (shipdecks.at (fileName).levelDimensions.y + drawOffset.y)));

	tempLevel.assign (static_cast<unsigned int>((shipdecks.at (fileName).levelDimensions.x + drawOffset.x) * (shipdecks.at (fileName).levelDimensions.y + drawOffset.y)), 0);

	for (countY = 0; countY != static_cast<int>(shipdecks.at (fileName).levelDimensions.y); countY++)
	{
		destX = static_cast<int>(drawOffset.x) / 2;

		for (countX = 0; countX != static_cast<int>(shipdecks.at (fileName).levelDimensions.x); countX++)
		{
			whichTile = shipdecks.at (fileName).tiles[static_cast<unsigned int>((countY * static_cast<int>(shipdecks.at (fileName).levelDimensions.x)) + countX)];
			tempLevel[static_cast<unsigned int>((destY * static_cast<int>((tempDimensions.x + drawOffset.x))) + destX)] = whichTile;
			destX++;
		}
		destY++;
	}

	tempDimensions.x += drawOffset.x;
	tempDimensions.y += drawOffset.y;

	shipdecks.at (fileName).levelDimensions.x = tempDimensions.x;
	shipdecks.at (fileName).levelDimensions.y = tempDimensions.y;

	shipdecks.at (fileName).tiles.clear ();

	shipdecks.at (fileName).tiles.reserve (static_cast<unsigned int>(tempDimensions.x * tempDimensions.y));

	shipdecks.at (fileName).tiles.assign (static_cast<unsigned int>(tempDimensions.x * tempDimensions.y), 0);

	for (int i = 0; i != tempDimensions.x * tempDimensions.y; i++)
	{
		shipdecks.at (fileName).tiles[i] = tempLevel[i];
	}
	tempLevel.clear ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Populate the shipdeck structure from a file in memory
void gam_loadShipDeck(const std::string &fileName)
//----------------------------------------------------------------------------------------------------------------------
{
	SDL_RWops    *fp;
	int          checkVersion;
	_deckStruct  tempLevel;
	_lineSegment tempSegment {};
	cpVect       tempWaypoint {};
	int          tempDroidType;
	int          tempTile;
	int          fileSize;
	char         *memoryBuffer;
	float        tempFloat;

	try
	{
		tileSize = textures.at ("tiles").getWidth () / numTileAcrossInTexture;
	}
	catch (std::out_of_range &outOfRange)
	{
		//
		// Tiles texture is not loaded yet, resubmit the load request with a time delay
		sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_DECK, 1, fileName);
		return;
	}

	drawOffset.x = static_cast<float>(gameWinWidth) / 2;     // Padding to make tilePosX always positive
	drawOffset.y = static_cast<float>(gameWinHeight) / 2;    // Padding to make tilePosY always positive

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

		cpVect tempVec2 {};

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
void gam_renderSingleTile(int destX, int destY, int tileIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	static int      previousTileIndex = -1;
	static SDL_Rect sourceRect;
	SDL_Rect        destRect;

	if (previousTileIndex != tileIndex)
	{
		previousTileIndex = tileIndex;

		sourceRect.x = static_cast<int>((tileIndex % numTileAcrossInTexture) * tileSize);
		sourceRect.y = (tileIndex / numTilesDownInTexture) * tileSize;
		sourceRect.w = tileSize;
		sourceRect.h = tileSize; // - 1;
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
void gam_createDeckTexture(const std::string &deckName)
//----------------------------------------------------------------------------------------------------------------------
{
	int tileIndex;

	if (playfieldTexture != nullptr)
		SDL_DestroyTexture (playfieldTexture);

	playfieldTexture = SDL_CreateTexture (renderer.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, static_cast<int>(shipdecks.at (deckName).levelDimensions.x * tileSize), static_cast<int>(
			shipdecks.at (deckName).levelDimensions.y * tileSize));
	if (nullptr == playfieldTexture)
		sys_shutdownWithError (sys_getString ("Unable to create playfield texture [ %s ].", SDL_GetError ()));

	SDL_SetRenderTarget (renderer.renderer, playfieldTexture);
	SDL_SetRenderDrawColor (renderer.renderer, 0x00, 0x00, 0x00, SDL_ALPHA_TRANSPARENT);
	SDL_RenderClear (renderer.renderer);

	for (auto countY = 0; countY != static_cast<int>(shipdecks.at (deckName).levelDimensions.y); countY++)
	{
		for (auto countX = 0; countX != static_cast<int>(shipdecks.at (deckName).levelDimensions.x); countX++)
		{
			tileIndex = shipdecks.at (deckName).tiles[(countY * static_cast<int>(shipdecks.at (deckName).levelDimensions.x)) + countX];
			if (tileIndex > 0)
				gam_renderSingleTile (countX * tileSize, countY * tileSize, tileIndex);
		}
	}
	SDL_SetRenderTarget (renderer.renderer, nullptr);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the levelName from the passed in deckNumber
std::string gam_returnLevelNameFromDeck(int deckNumber)
//----------------------------------------------------------------------------------------------------------------------
{
//
// Using   for (auto levelItr : shipdecks)
// Results in massive FPS drop
//
	for (auto const &levelItr: shipdecks)
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
void gam_changeToDeck(string deckName, int whichLift)
//----------------------------------------------------------------------------------------------------------------------
{
	std::string tempFilename;
	std::string tempTiles;

	tempTiles = "tiles";

	if (deckName.empty ())
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
	sys_setupDroidPhysics (deckName);
	sys_setupSolidWalls (deckName);
	gam_doorTriggerSetup ();
	gam_findTerminalPositions (deckName);
	gam_findLiftPositions (deckName);
	gam_findHealingTilesPhysics ();

	// Bullets
	gam_initBulletArray ();

	playerDroid.ai2.setPreviousPosInPixels ({-1, -1});
	playerDroid.setWorldPosInPixels (gam_getLiftWorldPosition (whichLift));
	sys_setPlayerPhysicsPosition (playerDroid.getWorldPosInPixels ());

	// AI
	gam_createInfluenceMap ();

	// Sounds
	gam_startAlertLevelSound (gam_getCurrentAlertLevel ());

	// Player
	gam_clearPlayerTrail ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render the current visible area to the backing texture
void gam_renderVisibleScreen(double interpolation)
//----------------------------------------------------------------------------------------------------------------------
{
	SDL_Rect        sourceRect;
	cpVect          renderPosition {};
	static SDL_Rect renderPositionBackground {};

	//
	// Only get texture sizes the first time
	if (renderPositionBackground.w == 0)
	{
		renderPositionBackground.x = 0;
		renderPositionBackground.y = 0;
		renderPositionBackground.w = textures.at ("universe").getWidth ();
		renderPositionBackground.h = textures.at ("universe").getHeight ();
	}

	viewportRect.x = static_cast<int>(playerDroid.getWorldPosInPixels ().x) - (gameWinWidth / 2);
	viewportRect.y = static_cast<int>(playerDroid.getWorldPosInPixels ().y) - (gameWinHeight / 2);
	viewportRect.w = gameWinWidth;
	viewportRect.h = gameWinHeight;

	renderPosition.x = playerDroid.getWorldPosInPixels ().x - playerDroid.ai2.getPreviousWorldPosInPixels ().x;
	renderPosition.y = playerDroid.getWorldPosInPixels ().y - playerDroid.ai2.getPreviousWorldPosInPixels ().y;

	renderPosition = cpvmult (renderPosition, interpolation);
	renderPosition.x += playerDroid.ai2.getPreviousWorldPosInPixels ().x;
	renderPosition.y += playerDroid.ai2.getPreviousWorldPosInPixels ().y;

	sourceRect.x = static_cast<int>(renderPosition.x) - (gameWinWidth / 2);
	sourceRect.y = static_cast<int>(renderPosition.y) - (gameWinHeight / 2);
	sourceRect.w = gameWinWidth;
	sourceRect.h = gameWinHeight;

	renderer.setCurrentBackingTexture (GAME_BACKING_TEXTURE);

	textures.at ("universe").render (&renderPositionBackground);

	backgroundStarfield.render ();
	//
	// Don't blit over parts of texture with transparent alpha set
	SDL_SetTextureBlendMode (playfieldTexture, SDL_BLENDMODE_BLEND);
	//
	// Rendercopy Float still uses Integer values for source...
	SDL_RenderCopyF (renderer.renderer, playfieldTexture, &sourceRect, nullptr);
}


//-------------------------------------------------------------------------------------------------------------------------
//
// Render the waypoint segments
void gam_showWayPoints(const std::string levelName)
//-------------------------------------------------------------------------------------------------------------------------
{
	int          indexCount {};
	cpVect       lineStart {};
	cpVect       lineFinish {};
	_lineSegment tempLine {};
	cpVect       wallStartDraw, wallFinishDraw {};

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

		thickLineRGBA (renderer.renderer, static_cast<short int>(wallStartDraw.x), static_cast<short int>(wallStartDraw.y), static_cast<short int>(wallFinishDraw.x), static_cast<short int>(wallFinishDraw.y), 3, 0, 0, 200, 55);

		roundedRectangleRGBA (renderer.renderer,
		                      static_cast<short int>(wallStartDraw.x) - 8, static_cast<short int>(wallStartDraw.y) - 8, static_cast<short int>(wallStartDraw.x) + 8, static_cast<short int>(wallStartDraw.y) + 8, 2, 0, 200, 0, 55);

		roundedRectangleRGBA (renderer.renderer,
		                      static_cast<short int>(wallFinishDraw.x) - 4, static_cast<short int>(wallFinishDraw.y) - 4, static_cast<short int>(wallFinishDraw.x) + 4, static_cast<short int>(wallFinishDraw.y) + 4, 2, 0, 200, 0, 55);

		indexCount++;
	}
}

//-------------------------------------------------------------------------------------------------------------------------
//
// All droids on this deck are dead - set deck to dead
void gam_setDeckIsDead()
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
		gam_addEvent (EVENT_ACTION_GAME_CHECK_DECK_CLEAR, 20, "");    // Check all decks
	}
}

//-------------------------------------------------------------------------------------------------------------------------
//
// Check if all the levels are dead - Game Won
void gam_checkAllLevels()
//-------------------------------------------------------------------------------------------------------------------------
{
	for (const auto &deckItr: shipdecks)
	{
		if (!deckItr.second.deckIsDead)
			return;     // One still got droids on it
	}
	//
	// All the decks are clear
	gam_addEvent (EVENT_ACTION_GAME_WON, 20, "");
}