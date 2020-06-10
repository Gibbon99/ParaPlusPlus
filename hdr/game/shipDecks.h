#pragma once

#include <unordered_map>
#include <box2d/b2_collision.h>
#include <classes/paraLift.h>
#include "main.h"
#include "droids.h"

#define MAP_VERSION                116

/*
class paraVec2d
{
public:
	// Default constructor does nothing (for performance).
	paraVec2d () = default;

	// Construct using coordinates.
	paraVec2d (double xIn, double yIn) : x (xIn), y (yIn)
	{
	}

	// Set this vector to some specified coordinates.
	void Set (double x_, double y_)
	{
		x = x_;
		y = y_;
	}

	// Subtract this vector from another.
	paraVec2d operator- (paraVec2d &v)
	{
		x -= v.x;
		y -= v.y;
	}

	// Add a vector to this vector.
	void operator+= (const paraVec2d &v)
	{
		x += v.x;
		y += v.y;
	}

	// Subtract a vector to this vector.
	void operator-= (const paraVec2d &v)
	{
		x -= v.x;
		y -= v.y;
	}

	// Multiply this vector by a scalar.
	void operator*= (double a)
	{
		x *= a;
		y *= a;
	}

	// Set to new value
	paraVec2d &operator= (const paraVec2d &v)
	{
		x = v.x;
		y = v.y;
	}

	double x;
	double y;

private:

};

struct paraVec2i
{
	int x, y;
};
*/
//-----------------------------------------------------------------------------
//
// Loaded from disk per level
//
//-----------------------------------------------------------------------------
struct _lineSegment
{
	b2Vec2 start;
	b2Vec2 finish;
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
	b2Vec2 worldPosInPixels;    // Position in world coords
	b2Vec2 worldPosInMeters;      // Position on the map to update image
};

struct _deckStruct
{
	int                        mapVersion;
	int                        numLineSegments;
	int                        numWaypoints;
	int                        numDroids;
	int                        numLifts;
	b2Vec2                     levelDimensions;
	std::vector<_lineSegment>  lineSegments; // Need to be floats for load ??
	std::vector<b2Vec2>        wayPoints;
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
	std::vector<droidClass>    droid;

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

// Populate the shipdeck structure from a file in memory
void gam_loadShipDeck (const std::string &fileName);

// Return the current deck name
std::string gam_getCurrentDeckName ();

// Change to a new deck
void gam_changeToDeck (const std::string &deckName, int whichLift);

void gam_renderVisibleScreen (double interpolation);

// Draw a single tile from the sheet to the playfield texture
void gam_renderSingleTile (int destX, int destY, int tileIndex);

// Get the playfield texture
PARA_Texture *gam_getPlayfieldTexture ();

// Return the levelName from the passed in deckNumber
std::string gam_returnLevelNameFromDeck (int deckNumber);

// Return the deck index number based on current deck name
int gam_getCurrentDeckIndex ();

// Create the influence map to be same size as deck dimensions - call on deck change
void gam_createInfluenceMap ();

// Change player pixel coords into tile coords
void gam_populateInfluenceMap (b2Vec2 playerPositionInPixels);

// Get the value from the influence map
int gam_getInfluenceMapValue (int tileIndex);

// Debug influence map
void gam_debugInfluenceMap ();

// Render the waypoint segments
void gam_showWayPoints (const std::string levelName);

// Create the texture to represent the current deck level
void gam_createDeckTexture (std::string deckName);

// All droids on this deck are dead - set deck to dead
void gam_setDeckIsDead ();

// Check if all the levels are dead - Game Won
void gam_checkAllLevels();