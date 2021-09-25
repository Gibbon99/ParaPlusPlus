#ifndef PARA_PARAASTAR_H
#define PARA_PARAASTAR_H

#include <thread>
#include <b2_math.h>
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
	ERROR
};

class paraAStar
{

public:

	struct _pathNode2
	{
		b2Vec2 tileLocation   = {0, 0};
		int    parent         = -1;
		int    g_movementCost = 0;
		int    h_estMoveCost  = 0;
		int    f_score        = 0;
	};

	struct _pathNode3
	{
		b2Vec2 tileLocation   = {0, 0};
		int    parent         = -1;
		int    g_movementCost = 0;
		int    h_estMoveCost  = 0;
		int    f_score        = 0;
	};

	struct _pathNode4
	{
		b2Vec2 tileLocation   = {0, 0};
		int    parent         = -1;
		int    g_movementCost = 0;
		int    h_estMoveCost  = 0;
		int    f_score        = 0;
	};

	struct _pathNode5
	{
		b2Vec2 tileLocation   = {0, 0};
		int    parent         = -1;
		int    g_movementCost = 0;
		int    h_estMoveCost  = 0;
		int    f_score        = 0;
	};

	paraAStar ();

	~paraAStar ();

	std::thread::id startThread ();

	void switchTravelDirection ();

	void stopUsingPath ();

	int requestNewPath (b2Vec2 start, b2Vec2 destination);

	inline void addTileToOpenNode (b2Vec2 whichTile, int moveCost, int parent);

	int findLowestCostNode ();

	void moveNodeToClosedList (int whichNodeIndex);

//	void searchThread ();
	int searchThread ();

	bool isNodeInClosedList (b2Vec2 whichNode);

	bool isNodeInOpenList (b2Vec2 whichNode);

	bool generateNewNode (int whichDirection);

	bool stillRunning ();

	bool areWaypointsReady ();

	b2Vec2 getWaypoint ();

	ASTAR_DIRECTION getAStarDirection ();

	int getPathStatus ();

//
// Helper routines
//
	int findDistance (b2Vec2 fromTile, b2Vec2 toTile);

	bool isTileSolid (int tileIndex);

//
// Prepare found path for use
//
	void extractPath ();

	void convertToCoords ();

	void compressWaypoints ();

//
// Debug routines
//
	void debugDraw (b2Vec2 lineStart, b2Vec2 lineFinish);

	void debugWayPoints ();

	void debugNodes ();

	int getID ();

	void setID (int newID);

	int getWayPointsIndex ();

private:

	int             ID                       = -1;
	int             currentNodePtrClosedList = -1;
	int             pathStatus               = -1;
	int             wayPointsIndex           = {};
	int             currentNodeIndex         = {};
	bool            wayPointsReady           = false;
	bool            isRunning                = false;
	b2Vec2          startTile                = {0, 0};
	b2Vec2          destTile                 = {0, 0};
	ASTAR_DIRECTION aStarDirection           = ASTAR_DIRECTION::DOWN;

	std::vector<_pathNode3> openNodes {};
	std::vector<_pathNode4> closedNodes {};
	std::vector<_pathNode5> foundPath {};
	std::vector<b2Vec2>     wayPoints {};        // In world ( pixel ) coordinates
	std::thread             *threadID;
};

#endif //PARA_PARAASTAR_H
