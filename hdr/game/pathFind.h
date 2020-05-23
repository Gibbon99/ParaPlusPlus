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
	bool   walkable;
	b2Vec2 tileLocation;
	int    parent;
	int    g_movementCost;
	int    h_estMoveCost;
	int    f_score;
};

struct _nodeList
{
	int                    whichDroid;        // Who owns this path
	std::string            whichLevel;        // Which level is this droid on
	bool                   pathReady;
	bool                   wayPointsReady;
	bool                   inUse;
	b2Vec2                 startTile;
	b2Vec2                 destTile;
	int                    currentNodePtrClosedList;
	std::vector<_pathNode> openNodes;
	std::vector<_pathNode> closedNodes;
	std::vector<_pathNode> foundPath;
	std::vector<b2Vec2>    wayPoints;
};

extern std::vector<_nodeList> path;
extern int numAStarPaths;
extern bool d_showNodeArrays;
extern bool d_showAStarPath;

// Start a new path
int gam_requestNewPath (b2Vec2 start, b2Vec2 destination, int whichDroid, std::string whichLevel);

// Show aStar waypoint lines
void gam_AStarDebugWayPoints (int whichPath);

// A thread to run the aStar searching function
void gam_AStarSearchThread (int whichPathArg);

// Show the open and closed nodes
void gam_AStarDebugNodes(int whichPath);

// See if the path is ready to use
bool gam_isAStarReady(int whichPath);