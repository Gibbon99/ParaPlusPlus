#pragma once

#include <game/shipDecks.h>
#include "main.h"

#define NODE_UP             0x0
#define NODE_DOWN           0x1
#define NODE_LEFT           0x2
#define NODE_RIGHT          0x4

#define initialNumReserveNodes 10

#define PATH_TOO_SHORT -1
#define NO_DROID -1


struct _pathNode
{
	bool   walkable = false;
	b2Vec2 tileLocation = {0,0};
	int    parent = -1;
	int    g_movementCost = 0;
	int    h_estMoveCost = 0;
	int    f_score = 0;
};

struct _nodeList
{
	int                    whichDroid = -1;        // Who owns this path
	std::string            whichLevel = "";        // Which level is this droid on
	bool                   pathReady = false;
	bool                   wayPointsReady = false;
	bool                   inUse = false;
	bool                   isValid = false;
	b2Vec2                 startTile = {0,0};
	b2Vec2                 destTile = {0, 0};
	int                    currentNodePtrClosedList = -1;;
	std::vector<_pathNode> openNodes{};
	std::vector<_pathNode> closedNodes{};
	std::vector<_pathNode> foundPath{};
	std::vector<b2Vec2>    wayPoints{};
};

extern std::vector<_nodeList> path;
extern int numAStarPaths;
extern bool d_showNodeArrays;
extern bool d_showAStarPath;

// Start a new path
int gam_requestNewPath (b2Vec2 start, b2Vec2 destination, int newWhichDroid, std::string whichLevel);

// Show aStar waypoint lines
void gam_AStarDebugWayPoints (int whichPath);

// A thread to run the aStar searching function
void gam_AStarSearchThread (int whichPathArg);

// Show the open and closed nodes
void gam_AStarDebugNodes(int whichPath);

// See if the path is ready to use
bool gam_isAStarReady(int whichPath);

// Clear coord memory and flag ready for reuse
void gam_AStarRemovePath (int whichPath);

// Is a tile considered 'solid' or not - no bounds checking version
bool gam_AStarIsTileSolid (int tileIndex);

// Remove a path based on droid using it
void gam_removeWhichDroidPath(int newWhichDroid);

// See if the path is a valid one
bool gam_isAStarValid(int whichPath);

// Draw all current aStar paths
void gam_AStarDebugAllPaths();

// Reset all path entries
void gam_resetAllPaths ();