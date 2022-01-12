#pragma once

#include <unordered_map>
#include <classes/paraLift.h>
#include "main.h"
#include "droids.h"

#define MAP_VERSION                117

//-----------------------------------------------------------------------------
//
// Loaded from disk per level
//
//-----------------------------------------------------------------------------
struct _lineSegment
{
	cpVect start;
	cpVect finish;
};

//-----------------------------------------------------------------------------
//
// information needed for one lift
//
//-----------------------------------------------------------------------------
struct _liftBasic
{
	int tunnel;
	int posX;
	int posY;
};

//-----------------------------------------------------------------------------
//
// structure to hold information for healing tiles
//
//-----------------------------------------------------------------------------
struct _basicHealing
{
	int    pos;                 // position in array
	int    currentFrame;        // which frame are we on
	double frameDelay;          // animation counter
	cpVect worldPosInPixels;    // Position in world coords
	cpVect worldPosInMeters;      // Position on the map to update image
};

struct _deckStruct
{
	int                        mapVersion;
	int                        numLineSegments;
	int                        numWaypoints;
	int                        numDroids;
	int                        numLifts;
	cpVect                     levelDimensions;
	std::vector<_lineSegment>  lineSegments; // Need to be floats for load ??
	std::vector<cpVect>        wayPoints;
	std::vector<int>           tiles;
	std::vector<int>           droidTypes;
	char                       levelName[20];
	// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	// Above here is loaded from file
	//
	int                        deckNumber;         // Use this to reference by a number
	int                        numEnemiesAlive;
	bool                       deckIsDead = false;
	std::vector<_liftBasic>    lifts;
	paraLift                   liftClass;
	std::vector<_basicHealing> healing;
	std::vector<paraDroid>     droid;

	bool wallPhysicsCreated  = false;
	bool droidPhysicsCreated = false;
};

extern SDL_Rect                                               viewportRect;
extern int                                                    tileSize;
extern std::unordered_map<std::string, _deckStruct>           shipdecks;
extern int                                                    currentDeckNumber;
extern int                                                    powerdownLevelScore;
extern bool                                                   d_showInfluenceMap;
extern std::unordered_map<std::string, _deckStruct>::iterator g_shipDeckItr;

// Load a shipdeck from the new XML format
bool gam_loadXMLShipDesk(const std::string &fileName);

// Return the current deck name
std::string gam_getCurrentDeckName();

// Change to a new deck
void gam_changeToDeck(std::string deckName, int whichLift);

void gam_renderVisibleScreen(double interpolation);

// Draw a single tile from the sheet to the playfield texture
void gam_renderSingleTile(int destX, int destY, int tileIndex);

// Get the playfield texture
PARA_Texture *gam_getPlayfieldTexture();

// Return the levelName from the passed in deckNumber
std::string gam_returnLevelNameFromDeck(int deckNumber);

// Return the deck index number based on current deck name
int gam_getCurrentDeckIndex();

// Create the influence map to be same size as deck dimensions - call on deck change
void gam_createInfluenceMap();

// Change player pixel coords into tile coords
void gam_populateInfluenceMap(cpVect playerPositionInPixels);

// Get the value from the influence map
int gam_getInfluenceMapValue(int tileIndex);

// Debug influence map
void gam_debugInfluenceMap();

// Render the waypoint segments
void gam_showWayPoints(const std::string levelName);

// Create the texture to represent the current deck level
void gam_createDeckTexture(const std::string &deckName);

// All droids on this deck are dead - set deck to dead
void gam_setDeckIsDead();

// Check if all the levels are dead - Game Won
void gam_checkAllLevels();