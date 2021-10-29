#pragma once

#include <thread>
#include "main.h"
#include "paraAI2.h"

#define NODE_UP             0x0
#define NODE_DOWN           0x1
#define NODE_LEFT           0x2
#define NODE_RIGHT          0x4

#define ASTAR_STATUS_READY      0x0
#define ASTAR_STATUS_NOT_READY  0x1
#define ASTAR_STATUS_ERROR      0x2
#define ASTAR_STATUS_TOO_SHORT  0x4

enum class ASTAR_DIRECTION
{
	DOWN = 0,
	UP,
	DESTINATION,
	START,
	ASTAR_ERROR
};

class paraAStar
{

public:

	struct _pathNode2
	{
		cpVect tileLocation   = {0, 0};
		int    parent         = -1;
		int    g_movementCost = 0;
		int    h_estMoveCost  = 0;
		int    f_score        = 0;
	};

	struct _pathNode3
	{
		cpVect tileLocation   = {0, 0};
		int    parent         = -1;
		int    g_movementCost = 0;
		int    h_estMoveCost  = 0;
		int    f_score        = 0;
	};

	struct _pathNode4
	{
		cpVect tileLocation   = {0, 0};
		int    parent         = -1;
		int    g_movementCost = 0;
		int    h_estMoveCost  = 0;
		int    f_score        = 0;
	};

	struct _pathNode5
	{
		cpVect tileLocation   = {0, 0};
		int    parent         = -1;
		int    g_movementCost = 0;
		int    h_estMoveCost  = 0;
		int    f_score        = 0;
	};

	paraAStar();

	~paraAStar();

	std::thread::id startThread();

	void switchTravelDirection();

	void stopUsingPath();

	int requestNewPath(cpVect start, cpVect destination);

	inline void addTileToOpenNode(cpVect whichTile, int moveCost, int parent);

	int findLowestCostNode();

	void moveNodeToClosedList(int whichNodeIndex);

//	void searchThread ();
	int searchThread();

	bool isNodeInClosedList(cpVect whichNode);

	bool isNodeInOpenList(cpVect whichNode);

	bool generateNewNode(int whichDirection);

	bool stillRunning();

	bool areWaypointsReady();

	cpVect getWaypoint();

	ASTAR_DIRECTION getAStarDirection();

	int getPathStatus();

//
// Helper routines
//
	int findDistance(cpVect fromTile, cpVect toTile);

	bool isTileSolid(int tileIndex);

//
// Prepare found path for use
//
	void extractPath();

	void convertToCoords();

	void compressWaypoints();

//
// Debug routines
//
	void debugDraw(cpVect lineStart, cpVect lineFinish);

	void debugWayPoints();

	void debugNodes();

	int getID();

	void setID(int newID);

	int getWayPointsIndex();

private:

	int             ID                       = -1;
	int             currentNodePtrClosedList = -1;
	int             pathStatus               = -1;
	int             wayPointsIndex           = {};
	int             currentNodeIndex         = {};
	bool            wayPointsReady           = false;
	bool            isRunning                = false;
	cpVect          startTile                = {0, 0};
	cpVect          destTile                 = {0, 0};
	ASTAR_DIRECTION aStarDirection           = ASTAR_DIRECTION::DOWN;

	std::vector<_pathNode3> openNodes {};
	std::vector<_pathNode4> closedNodes {};
	std::vector<_pathNode5> foundPath {};
	std::vector<cpVect>     wayPoints {};        // In world ( pixel ) coordinates
	std::thread             *threadID;
};