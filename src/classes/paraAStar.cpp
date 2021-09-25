#include <thread>
#include <SDL2_gfxPrimitives.h>
#include "game/shipDecks.h"
#include "game/doors.h"
#include "system/util.h"
#include "classes/paraAStar.h"

//#define ASTAR_DEBUG 1
#define ASTAR_DO_PATH_COMPRESS 1

#define ASTAR_THREAD_STARTED        0
#define ASTAR_PATH_TOO_SHORT        -1
#define ASTAR_TILE_IS_SOLID         -2

#define ASTAR_PATH_TOO_SHORT_DISTANCE 2

//-----------------------------------------------------------------------------------------------------------------------
//
// Constructor
paraAStar::paraAStar ()
//-----------------------------------------------------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Deconstructor
paraAStar::~paraAStar ()
//-----------------------------------------------------------------------------------------------------------------------
{
}

//--------------------------------------------------------------------------------------------------------
//
// Return the ID for this aStar and thread
int paraAStar::getID ()
//--------------------------------------------------------------------------------------------------------
{
	return ID;
}

//--------------------------------------------------------------------------------------------------------
//
// Set the ID for this aStar
void paraAStar::setID (int newID)
//--------------------------------------------------------------------------------------------------------
{
	ID = newID;
}

//--------------------------------------------------------------------------------------------------------
//
// Show the open and closed nodes
void paraAStar::debugNodes ()
//--------------------------------------------------------------------------------------------------------
{
	b2Vec2 drawPosition {};

	for (auto foundItr: wayPoints)
	{
		drawPosition = sys_worldToScreen (foundItr, tileSize);

		boxRGBA (renderer.renderer, static_cast<Sint16>(drawPosition.x), static_cast<Sint16>(drawPosition.y), static_cast<Sint16>(drawPosition.x + tileSize), static_cast<Sint16>(drawPosition.y + tileSize),
		         drawPosition.x * 10, 150, 100, 128);
	}
}

//--------------------------------------------------------------------------------------------------------
//
// Draw debug graphic
void paraAStar::debugDraw (b2Vec2 lineStart, b2Vec2 lineFinish)
//--------------------------------------------------------------------------------------------------------
{
	lineStart  = sys_worldToScreen (lineStart, 2);
	lineFinish = sys_worldToScreen (lineFinish, 2);

	thickLineRGBA (renderer.renderer, static_cast<Sint16>(lineStart.x), static_cast<Sint16>(lineStart.y), static_cast<Sint16>(lineFinish.x), static_cast<Sint16>(lineFinish.y), 3, 0, 0, 200, 255);

	roundedRectangleRGBA (renderer.renderer, static_cast<Sint16>(lineStart.x - 8), static_cast<Sint16>(lineStart.y - 8), static_cast<Sint16>(lineStart.x + 8), static_cast<Sint16>(lineStart.y + 8), 2, 0, 200, 0, 255);

	roundedRectangleRGBA (renderer.renderer, static_cast<Sint16>(lineFinish.x - 4), static_cast<Sint16>(lineFinish.y - 4), static_cast<Sint16>(lineFinish.x + 4), static_cast<Sint16>(lineFinish.y + 4), 2, 0, 200, 0, 255);
}

//--------------------------------------------------------------------------------------------------------
//
// Show aStar waypoint lines
void paraAStar::debugWayPoints ()
//--------------------------------------------------------------------------------------------------------
{
	b2Vec2 lineStart {};
	b2Vec2 lineFinish {};
	int    i {0};

	if (wayPoints.size () == 0)
		return;

	if (!wayPointsReady)
		return;

	for (i = 0; i != static_cast<int>(wayPoints.size ()) - 2; i++)
	{

		lineStart.x = wayPoints[i].x;
		lineStart.y = wayPoints[i].y;

		lineFinish.x = wayPoints[i + 1].x;
		lineFinish.y = wayPoints[i + 1].y;

		debugDraw (lineStart, lineFinish);
	}

	lineStart.x = wayPoints[i].x;
	lineStart.y = wayPoints[i].y;

	lineFinish.x = wayPoints[i + 1].x;
	lineFinish.y = wayPoints[i + 1].y;

	debugDraw (lineStart, lineFinish);
}

//--------------------------------------------------------------------------------------------------------
//
// Return if the thread is still running
bool paraAStar::stillRunning ()
//--------------------------------------------------------------------------------------------------------
{
	return isRunning;
}

//--------------------------------------------------------------------------------------------------------
//
// Return if the waypoints are ready to use
bool paraAStar::areWaypointsReady ()
//--------------------------------------------------------------------------------------------------------
{
	std::cout << sys_getString ("Index %i Function : %s", ID, __func__) << std::endl;

	return wayPointsReady;
}

//--------------------------------------------------------------------------------------------------------
//
// Is a tile considered 'solid' or not for creating a path
bool paraAStar::isTileSolid (int tileIndex)
//--------------------------------------------------------------------------------------------------------
{
	if (tileIndex < 0)
		return true;
	//
	// Bounds check
	if (tileIndex > static_cast<int>(shipdecks.at (gam_getCurrentDeckName ()).tiles.size ()))
		return true;

	switch (shipdecks.at (gam_getCurrentDeckName ()).tiles[tileIndex])
	{
		case DOOR_ACROSS_CLOSED:
		case DOOR_ACROSS_OPEN_1:
		case DOOR_ACROSS_OPEN_2:
		case DOOR_ACROSS_CLOSING_1:
		case DOOR_ACROSS_CLOSING_2:

		case DOOR_UP_CLOSED:
		case DOOR_UP_OPEN_1:
		case DOOR_UP_OPEN_2:
		case DOOR_UP_CLOSING_1:
		case DOOR_UP_CLOSING_2:
			return false;
			break;

		case TERMINAL_BOTTOM:
		case TERMINAL_LEFT:
		case TERMINAL_RIGHT:
		case TERMINAL_TOP:
			return true;
			break;
	}

	if (shipdecks.at (gam_getCurrentDeckName ()).tiles[tileIndex] <= NO_PASS_TILE) // This stops the wall tile being considered passable
		return true;

	return false;
}

//--------------------------------------------------------------------------------------------------------
//
// Find the distance from one tile to the next - Manhatten distance
int paraAStar::findDistance (b2Vec2 fromTile, b2Vec2 toTile)
//--------------------------------------------------------------------------------------------------------
{
	int costX {}, costY {};

#ifdef ASTAR_DEBUG
	printf ("Thread [ %i ]: Checking distance between tiles.\n", ID);
#endif

	costX = static_cast<int>(fromTile.x - toTile.x);
	costY = static_cast<int>(fromTile.y - toTile.y);

	return abs (10 * (costX + costY));
}

//--------------------------------------------------------------------------------------------------------
//
// Add a tile to the openNode list
inline void paraAStar::addTileToOpenNode (b2Vec2 whichTile, int moveCost, int parent)
//--------------------------------------------------------------------------------------------------------
{
#ifdef ASTAR_DEBUG
	printf ("Thread [ %i ]: Adding new tile to openNode list.\n", ID);
#endif

	_pathNode3 tempNode {};

	tempNode.parent       = parent;
	tempNode.tileLocation = whichTile;

	if (-1 == parent) // Adding the first node – starting tile
		tempNode.g_movementCost = 0;
	else
		tempNode.g_movementCost = moveCost + closedNodes[currentNodePtrClosedList].g_movementCost;

	tempNode.h_estMoveCost = findDistance (whichTile, destTile);
	tempNode.f_score       = tempNode.g_movementCost + tempNode.h_estMoveCost;

#ifdef ASTAR_DEBUG
	printf ("Thread [ %i ]: Tempnode movement cost [ %i ] estMoveCost [ %i ] f_score [ %i ].\n", ID, tempNode.g_movementCost, tempNode.h_estMoveCost, tempNode.f_score);
#endif

	openNodes.emplace_back (tempNode); //  push_back (tempNode);

#ifdef ASTAR_DEBUG
	printf ("Thread [ %i ]: Opennodes size [ %i ] parent [ %i ] moveCost [ %i ] distanceCost [ %i ].\n", ID, openNodes.size (), parent, tempNode.g_movementCost, tempNode.h_estMoveCost);
#endif
}

//--------------------------------------------------------------------------------------------------------
//
// Find the node with the lowest totalCost
int paraAStar::findLowestCostNode ()
//--------------------------------------------------------------------------------------------------------
{
	int lowestCost {50000};
	int lowestNodeIndexArray {0};

	if (openNodes.empty ())
	{
		printf ("Thread [ %i ]: ERROR: No open nodes to search in paraAStar::findLowestCostNode.\n", ID);
		return -1;
	}

#ifdef ASTAR_DEBUG
	printf ("Thread [ %i ]: Search the open nodes and find the one with the lowest total cost.\n", ID);
#endif

	for (auto itr: openNodes)
	{
		if (itr.f_score < lowestCost)
		{
			lowestCost = itr.f_score;
			lowestNodeIndexArray++;
		}
	}
	return lowestNodeIndexArray - 1;
}

//--------------------------------------------------------------------------------------------------------
//
// Move a node from the openNode list to the closedNode list
void paraAStar::moveNodeToClosedList (int whichNodeIndex)
//--------------------------------------------------------------------------------------------------------
{
	_pathNode4 tempNode {};
//	std::vector<_pathNode2>::iterator itr{};
//	int                               indexCount = 0;

#ifdef ASTAR_DEBUG
	printf ("Thread [ %i ]: Move open node [ %i ] to closedNode list.\n", ID, whichNodeIndex);
#endif

	if (whichNodeIndex > openNodes.size () - 1)
		sys_shutdownWithError ("whichNodeIndex is greater than openNode size: moveNodeToClosedList");

	if (openNodes.empty ())
		sys_shutdownWithError ("openNodes is empty");

	// Error here ??

//	closedNodes.push_back (openNodes[whichNodeIndex]);

	tempNode.g_movementCost = openNodes[whichNodeIndex].g_movementCost;
	tempNode.tileLocation   = openNodes[whichNodeIndex].tileLocation;
	tempNode.f_score        = openNodes[whichNodeIndex].f_score;
	tempNode.h_estMoveCost  = openNodes[whichNodeIndex].h_estMoveCost;
	tempNode.parent         = openNodes[whichNodeIndex].parent;

	//closedNodes.emplace_back (openNodes[whichNodeIndex]);
	closedNodes.emplace_back (tempNode);
	currentNodePtrClosedList = static_cast<int>(closedNodes.size () - 1);

#ifdef ASTAR_DEBUG
	printf ("Thread [ %i ]: currentNodePtrClosedList is now [ %i ] - number closedNodes [ %i ]\n", ID, currentNodePtrClosedList, static_cast<int>(closedNodes.size ()));
#endif

	if (whichNodeIndex == 0)
	{
		openNodes.erase (openNodes.begin ());
	}
	else
	{
		openNodes.erase (openNodes.begin () + whichNodeIndex);
	}
}

//--------------------------------------------------------------------------------------------------------
//
// Check if node is in the OPEN list
bool paraAStar::isNodeInOpenList (b2Vec2 whichNode)
//--------------------------------------------------------------------------------------------------------
{
	for (auto itr: openNodes)
	{
		if ((whichNode.x == itr.tileLocation.x) && (whichNode.y == itr.tileLocation.y))
			return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------
//
// Check if node is in the CLOSED list
bool paraAStar::isNodeInClosedList (b2Vec2 whichNode)
//--------------------------------------------------------------------------------------------------------
{
	for (auto itr: closedNodes)
	{
		if ((whichNode.x == itr.tileLocation.x) && (whichNode.y == itr.tileLocation.y))
			return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------
//
// Generate a new node
//
// Return FALSE if the tile is solid and no new node can be generated
// Return FALSE if the node is already used on the CLOSED list
bool paraAStar::generateNewNode (int whichDirection)
//--------------------------------------------------------------------------------------------------------
{
	std::string directionStr {};
	_pathNode2  tempNode {};
	int         moveTileCost {10};

#ifdef ASTAR_DEBUG
	printf ("Thread [ %i ]: ---\n", ID);
	printf ("Thread [ %i ]: Generate a new node based on the node with the current lowest cost.\n", ID);
#endif

	switch (whichDirection)
	{
		case NODE_LEFT:
			tempNode.tileLocation.x = closedNodes[currentNodePtrClosedList].tileLocation.x - 1;
			tempNode.tileLocation.y = closedNodes[currentNodePtrClosedList].tileLocation.y;
			moveTileCost = 10;
			directionStr = "LEFT";
			break;

		case NODE_UP:
			tempNode.tileLocation.x = closedNodes[currentNodePtrClosedList].tileLocation.x;
			tempNode.tileLocation.y = closedNodes[currentNodePtrClosedList].tileLocation.y - 1;
			moveTileCost = 10;
			directionStr = "UP";
			break;

		case NODE_RIGHT:
			tempNode.tileLocation.x = closedNodes[currentNodePtrClosedList].tileLocation.x + 1;
			tempNode.tileLocation.y = closedNodes[currentNodePtrClosedList].tileLocation.y;
			moveTileCost = 10;
			directionStr = "RIGHT";
			break;

		case NODE_DOWN:
			tempNode.tileLocation.x = closedNodes[currentNodePtrClosedList].tileLocation.x;
			tempNode.tileLocation.y = closedNodes[currentNodePtrClosedList].tileLocation.y + 1;
			moveTileCost = 10;
			directionStr = "DOWN";
			break;
	}
#ifdef ASTAR_DEBUG
	printf ("Thread [ %i ]: Parent [ %i ] Direction [ %s ]\n", ID, currentNodePtrClosedList, directionStr.c_str ());
#endif

#ifdef ASTAR_DEBUG
	printf ("Thread [ %i ]: See if the node [ %s X: %f Y: %f ] is solid or not.\n", ID, directionStr.c_str (), tempNode.tileLocation.x, tempNode.tileLocation.y);
#endif

	if (isTileSolid (static_cast<int>((tempNode.tileLocation.y * shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.x) + tempNode.tileLocation.x)))
	{
#ifdef ASTAR_DEBUG
		printf ("Thread [ %i ]: Tile is Solid - don't use.\n", ID);
#endif
		return false; // Solid - don't use
	}

//
// Player location influences path cost to move on the tiles
//
	int tileCostModify = gam_getInfluenceMapValue (static_cast<int>((tempNode.tileLocation.y * (shipdecks.at (gam_getCurrentDeckName ()).levelDimensions.x) + tempNode.tileLocation.x)));
	moveTileCost += tileCostModify;

//
// See if we have found a path to the destination tile
//
#ifdef ASTAR_DEBUG
	printf ("Thread [ %i ]: Look for destination [ %f %f ] to current [ %f %f ]\n", ID, destTile.x, destTile.y, tempNode.tileLocation.x, tempNode.tileLocation.y);
#endif

	if ((tempNode.tileLocation.x == destTile.x) && (tempNode.tileLocation.y == destTile.y))
	{
#ifdef ASTAR_DEBUG
		printf ("Thread [ %i ]: Found the destination.\n\n", ID);
#endif
		addTileToOpenNode (tempNode.tileLocation, moveTileCost, currentNodePtrClosedList);
		if (openNodes.size () > 0)
			moveNodeToClosedList (openNodes.size () - 1);
		else
			moveNodeToClosedList (0);
		pathStatus = ASTAR_STATUS_READY;
		return true;
	}
//
// See if the node is on the closed list or not
//
	if (isNodeInClosedList (tempNode.tileLocation))
	{
		return false; // Already in closed list - do not use
	}
//
// If it isn't on the open list - add it
//
	if (!isNodeInOpenList (tempNode.tileLocation))
	{
		addTileToOpenNode (tempNode.tileLocation, moveTileCost, currentNodePtrClosedList);
		return false;
	}
//
// The node is already on the open list - see if the runningCost to this node is better
//
	int    nodeIndex {0};
	size_t openNodeSize {};

	nodeIndex    = currentNodePtrClosedList;
	openNodeSize = openNodes.size ();

	if (nodeIndex <= static_cast<int>(closedNodes.size () - 1))
	{
		if (openNodes[openNodeSize - 1].g_movementCost < 0)
		{
			sys_shutdownWithError (sys_getString ("paraAStar::generateNewNode: Invalid movement cost [ %i ]\n", openNodes[openNodeSize - 1].g_movementCost));
		}

		if (nodeIndex == 0)
		{
			printf ("ERROR: nodeIndex is 0\n\n");
			return false;
		}

		if (openNodes[openNodeSize - 1].g_movementCost < closedNodes[nodeIndex - 1].g_movementCost)
		{
#ifdef ASTAR_DEBUG
			printf ("Thread [ %i ]: New node is better than the old one.\n", ID);
#endif
			addTileToOpenNode (openNodes[openNodeSize - 1].tileLocation, moveTileCost, currentNodePtrClosedList);
		}
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------
//
// Extract the path from the list of closed nodes
void paraAStar::extractPath ()
//--------------------------------------------------------------------------------------------------------
{
	int        nodeParent {-1};
	_pathNode5 tempNode {};

	printf ("[ %i ] Starting extractPath\n", ID);

	//
	// Store the destination tile
	//
	tempNode.tileLocation = destTile;
	foundPath.emplace_back (tempNode);
	//
	// Get the next tile along
	//

	if (currentNodePtrClosedList == 0)
	{
		printf ("ERROR: node pointer is 0\n\n");
		return;
	}

	printf ("[ %i ] About to set first foundPath node\n", ID);

	tempNode.tileLocation = closedNodes[currentNodePtrClosedList - 1].tileLocation;
	foundPath.emplace_back (tempNode);
	//
	// What's the parent
	//
	nodeParent = closedNodes[currentNodePtrClosedList - 1].parent;

	printf ("ClosedNodes size [ %zu ] OpenNodes size [ %zu ]\n", closedNodes.size (), openNodes.size ());

	//
	// While we haven't reached the starting tile ( no parent )
	while (-1 != nodeParent)
	{
		tempNode.tileLocation   = closedNodes[nodeParent].tileLocation;
		tempNode.f_score        = closedNodes[nodeParent].f_score;
		tempNode.g_movementCost = closedNodes[nodeParent].g_movementCost;
		tempNode.h_estMoveCost  = closedNodes[nodeParent].h_estMoveCost;
		tempNode.parent         = closedNodes[nodeParent].parent;

		foundPath.emplace_back (tempNode);

		nodeParent = closedNodes[nodeParent].parent;

		if ((tempNode.tileLocation.x == startTile.x) && (tempNode.tileLocation.y == startTile.y))
		{
			foundPath.emplace_back (tempNode);
			return;
		}
	}
}

//--------------------------------------------------------------------------------------------------------
//
// Compress the waypoints
void paraAStar::compressWaypoints ()
//--------------------------------------------------------------------------------------------------------
{
	int                 current{1};
	std::vector<b2Vec2> newPoints{};
	b2Vec2              tempPoint{};

	newPoints.clear ();
	tempPoint = wayPoints[0];
	newPoints.emplace_back (tempPoint);

	for (int i = 0; i != static_cast<int>(wayPoints.size ()) - 1; i++)
	{
		if ((newPoints[newPoints.size () - 1].x != wayPoints[current].x) && (newPoints[newPoints.size () - 1].y != wayPoints[current].y))
		{
			tempPoint = wayPoints[current - 1];
			newPoints.emplace_back (tempPoint);
			current++;
		}
		else
		{
			current++;
		}
	}
	newPoints.emplace_back (wayPoints[wayPoints.size () - 1]);

	//
	// Copy back into structure
	wayPoints.clear ();

	for (auto itr: newPoints)
	{
		wayPoints.emplace_back (itr);
	}
}

//--------------------------------------------------------------------------------------------------------
//
// Turn the AStar nodes into world position coords
void paraAStar::convertToCoords ()
//--------------------------------------------------------------------------------------------------------
{
	b2Vec2 tempWaypoint;

	for (auto itr: foundPath)
	{
		tempWaypoint = itr.tileLocation;

		tempWaypoint = sys_convertTilesToPixels (tempWaypoint);

		tempWaypoint.x += tileSize / 2;
		tempWaypoint.y += tileSize / 2;

		wayPoints.emplace_back (tempWaypoint);
	}

	std::reverse (wayPoints.begin (), wayPoints.end ());

#ifdef ASTAR_DO_PATH_COMPRESS
	if (wayPoints.size () > 4)
		compressWaypoints ();
#endif
}

//--------------------------------------------------------------------------------------------------------
//
// Swap the direction flag
void paraAStar::switchTravelDirection ()
//--------------------------------------------------------------------------------------------------------
{
	if (aStarDirection == ASTAR_DIRECTION::UP)
		aStarDirection = ASTAR_DIRECTION::DOWN;
	else
		aStarDirection = ASTAR_DIRECTION::UP;
}

//--------------------------------------------------------------------------------------------------------
//
// Get the aStar direction
ASTAR_DIRECTION paraAStar::getAStarDirection ()
//--------------------------------------------------------------------------------------------------------
{
	return aStarDirection;
}

//--------------------------------------------------------------------------------------------------------
//
// Get the waypoint pointed to by the wayPointsIndex
b2Vec2 paraAStar::getWaypoint ()
//--------------------------------------------------------------------------------------------------------
{
	if (wayPoints.size () == 0)
	{
		printf ("STOP: Waypoint array is empty.\n\n");
		aStarDirection = ASTAR_DIRECTION::ERROR;
		return b2Vec2{};
	}

	switch (aStarDirection)
	{
		case ASTAR_DIRECTION::DESTINATION:
			break;

		case ASTAR_DIRECTION::START:
			wayPointsIndex = 0;
			break;

		case ASTAR_DIRECTION::DOWN:
			wayPointsIndex--;
			if (wayPointsIndex < 0)
			{
				wayPointsIndex = 0;
				aStarDirection = ASTAR_DIRECTION::START;   // Reverse direction as reached start of waypoint array
			}
			break;

		case ASTAR_DIRECTION::UP:
			wayPointsIndex++;
			if (wayPointsIndex > static_cast<int>(wayPoints.size ()) - 1)
			{
				wayPointsIndex = static_cast<int>(wayPoints.size ()) - 1;
				aStarDirection = ASTAR_DIRECTION::DESTINATION;
			}
			break;
	}
	return wayPoints[wayPointsIndex];
}

//--------------------------------------------------------------------------------------------------------
//
// Get the path status
int paraAStar::getPathStatus ()
//--------------------------------------------------------------------------------------------------------
{
	return pathStatus;
}

//--------------------------------------------------------------------------------------------------------
//
// Return the current wayPointIndex - debug
int paraAStar::getWayPointsIndex ()
//--------------------------------------------------------------------------------------------------------
{
	return wayPointsIndex;
}

//--------------------------------------------------------------------------------------------------------
//
// A thread to run the aStar searching function
//void paraAStar::searchThread ()
int paraAStar::searchThread ()
//--------------------------------------------------------------------------------------------------------
{
	std::cout << sys_getString ("Index %i Function : %s", ID, __func__) << std::endl;

	printf ("Index %i Starting searchThread...\n", ID);

//	printf ("Address of isRunning is : %p\n", &isRunning);

	if (isRunning)
		return ASTAR_STATUS_NOT_READY;

	pathStatus = ASTAR_STATUS_NOT_READY;

#ifdef ASTAR_DEBUG
	printf ("Thread [ %i ]: AStar searchThread started...\n", ID);
#endif

	while (pathStatus == ASTAR_STATUS_NOT_READY)
	{
		isRunning = true;

		// Look for the lowest F cost node on the open list - this becomes the current node
		currentNodeIndex = findLowestCostNode ();
		if (currentNodeIndex < 0)
		{
			pathStatus     = ASTAR_STATUS_ERROR;
			wayPointsReady = false;
			isRunning      = false;
#ifdef ASTAR_DEBUG
			printf ("Thread [ %i ]: Error: couldn't find lowest cost node.\n", ID);
#endif
			return ASTAR_STATUS_ERROR; // Something has gone wrong
		}

		// Move it to the closed node list
		moveNodeToClosedList (currentNodeIndex);

		// For each of the surrounding nodes
		generateNewNode (NODE_RIGHT);
		generateNewNode (NODE_LEFT);
		generateNewNode (NODE_UP);
		generateNewNode (NODE_DOWN);
	}
	extractPath ();
	convertToCoords ();
	wayPointsIndex = 0;
	aStarDirection = ASTAR_DIRECTION::UP;
	pathStatus     = ASTAR_STATUS_READY;
	wayPointsReady = true;
	isRunning      = false;

#ifdef ASTAR_DEBUG
	printf ("\nThread [ %i ]: AStar found a valid path.\n", ID);
#endif

	printf ("\nThread [ %i ]: AStar found a valid path.\n", ID);

	return ASTAR_STATUS_READY;
}

//--------------------------------------------------------------------------------------------------------
//
// Stop using the current aStar path and reset it
void paraAStar::stopUsingPath ()
//--------------------------------------------------------------------------------------------------------
{
	closedNodes.clear ();
	openNodes.clear ();
	wayPoints.clear ();
	foundPath.clear ();
	currentNodePtrClosedList = 0;
	pathStatus               = ASTAR_STATUS_NOT_READY;
	wayPointsReady           = false;
	isRunning                = false;
	wayPointsIndex           = 0;
	aStarDirection           = ASTAR_DIRECTION::UP;
}

//--------------------------------------------------------------------------------------------------------
//
// Start a new path and create a detached thread to run the pathfinding
int paraAStar::requestNewPath (b2Vec2 start, b2Vec2 destination)
//--------------------------------------------------------------------------------------------------------
{
	std::cout << sys_getString ("Index %i Function : %s", ID, __func__) << std::endl;

	float distanceTestResult = 0.0f;

	//
	// Convert any floats to whole numbers - used for matching later on
	//
	modf (start.x, &startTile.x);
	modf (start.y, &startTile.y);

	modf (destination.x, &destTile.x);
	modf (destination.y, &destTile.y);

	distanceTestResult = b2Distance (start, destination);

#ifdef ASTAR_DEBUG
	printf ("Thread [ %i ]: AStar distance between start and destination [ %f ]\n", ID, distanceTestResult);
#endif

	if (distanceTestResult < ASTAR_PATH_TOO_SHORT_DISTANCE)
		return ASTAR_PATH_TOO_SHORT;

	stopUsingPath ();

	addTileToOpenNode (startTile, 0, -1);

#ifdef ASTAR_DEBUG
	printf ("Thread [ %i ]: About to start thread from class.\n", ID);
#endif

	return ASTAR_THREAD_STARTED;
}

//--------------------------------------------------------------------------------------------------------
//
// Start the aStar pathfinding thread
std::thread::id paraAStar::startThread ()
//--------------------------------------------------------------------------------------------------------
{
	threadID = new std::thread (&paraAStar::searchThread, this);

#ifdef ASTAR_DEBUG
	printf ("ThreadID %i\n", threadID->get_id ());
#endif

	return threadID->get_id ();

	//	threadID->detach ();
}