#include <system/util.h>
#include <game/doors.h>
#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include <game/droids.h>
#include "game/pathFind.h"

//--------------------------------------------------------------------------------------------------------
// AStar
// Droid requests a new path - is passed a ticket
// Ticket is an index into the path array
// The path assigned holds the current level and current droid
// A thread is created and detached to run the pathfinding for the path
// Droid keeps checking to see if the path is ready or not
// Flag set when the path is found and the compressed waypoints are copied into the path array
//--------------------------------------------------------------------------------------------------------

// #define DEBUG_ASTAR 1

std::vector<_nodeList> path;
int                    numAStarPaths;
bool                   d_showNodeArrays = false;
bool                   d_showAStarPath  = false;

//--------------------------------------------------------------------------------------------------------
//
// Draw debug graphic
void gam_AStarDebugDraw (b2Vec2 lineStart, b2Vec2 lineFinish, int whichPath, int count)
//--------------------------------------------------------------------------------------------------------
{
	lineStart  = sys_worldToScreen (lineStart, 2);
	lineFinish = sys_worldToScreen (lineFinish, 2);

	thickLineRGBA (renderer.renderer, lineStart.x, lineStart.y, lineFinish.x, lineFinish.y, 3, 0, 0, 200, 255);

	roundedRectangleRGBA (renderer.renderer, lineStart.x - 8, lineStart.y - 8, lineStart.x + 8, lineStart.y + 8, 2, 0, 200, 0, 255);

	roundedRectangleRGBA (renderer.renderer, lineFinish.x - 4, lineFinish.y - 4, lineFinish.x + 4, lineFinish.y + 4, 2, 0, 200, 0, 255);
}

//--------------------------------------------------------------------------------------------------------
//
// Show the open and closed nodes
void gam_AStarDebugNodes (int whichPath)
//--------------------------------------------------------------------------------------------------------
{
	b2Vec2 drawPosition;

	if (path.size () == 0)
		return;

	if (path[whichPath].openNodes.size () == 0)
		return;

	if (path[whichPath].closedNodes.size () == 0)
		return;

	for (auto openItr : path[whichPath].openNodes)
	{
		drawPosition.x = openItr.tileLocation.x * tileSize;
		drawPosition.y = openItr.tileLocation.y * tileSize;

		drawPosition = sys_worldToScreen (drawPosition, tileSize);

		boxRGBA (renderer.renderer, drawPosition.x, drawPosition.y, drawPosition.x + tileSize, drawPosition.y + tileSize, 0, 150, 0, 128);
	}

	for (auto closedItr : path[whichPath].closedNodes)
	{
		drawPosition.x = closedItr.tileLocation.x * tileSize;
		drawPosition.y = closedItr.tileLocation.y * tileSize;

		drawPosition = sys_worldToScreen (drawPosition, tileSize);

		boxRGBA (renderer.renderer, drawPosition.x, drawPosition.y, drawPosition.x + tileSize, drawPosition.y + tileSize, 0, 0, 150, 128);
	}
}

//--------------------------------------------------------------------------------------------------------
//
// Show aStar waypoint lines
void gam_AStarDebugWayPoints (int whichPath)
//--------------------------------------------------------------------------------------------------------
{
	b2Vec2 lineStart;
	b2Vec2 lineFinish;
	int       i = 0;

	if (-1 == whichPath)
		return;

	if (path.size () == 0)
		return;

	if (path[whichPath].wayPoints.size () == 0)
		return;

	if (!path[whichPath].wayPointsReady)
		return;

	for (i = 0; i != static_cast<int>(path[whichPath].wayPoints.size ()) - 2; i++)
	{
		lineStart.x = path[whichPath].wayPoints[i].x;
		lineStart.y = path[whichPath].wayPoints[i].y;

		lineFinish.x = path[whichPath].wayPoints[i + 1].x;
		lineFinish.y = path[whichPath].wayPoints[i + 1].y;

		gam_AStarDebugDraw (lineStart, lineFinish, whichPath, i);
	}

	lineStart.x = path[whichPath].wayPoints[i].x;
	lineStart.y = path[whichPath].wayPoints[i].y;

	lineFinish.x = path[whichPath].wayPoints[i + 1].x;
	lineFinish.y = path[whichPath].wayPoints[i + 1].y;

	gam_AStarDebugDraw (lineStart, lineFinish, whichPath, i);
}

//--------------------------------------------------------------------------------------------------------
//
// Find the distance from one tile to the next - Manhatten distance
int gam_AStarFindDistance (b2Vec2 fromTile, b2Vec2 toTile)
//--------------------------------------------------------------------------------------------------------
{
	int costX, costY;

#ifdef DEBUG_ASTAR
	con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("Checking distance between tiles."));
#endif

	costX = static_cast<int>(fromTile.x - toTile.x);
	costY = static_cast<int>(fromTile.y - toTile.y);

	return abs (10 * (costX + costY));
}

//--------------------------------------------------------------------------------------------------------
//
// Add a tile to the openNode list
inline void gam_AStarAddTileToOpenNode (int whichPath, b2Vec2 whichTile, int moveCost, int parent)
//--------------------------------------------------------------------------------------------------------
{
#ifdef DEBUG_ASTAR
	con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("Adding new tile to openNode list"));
#endif

	_pathNode tempNode;

	tempNode.parent       = parent;
	tempNode.tileLocation = whichTile;

	if (-1 == parent) // Adding the first node – starting tile
		tempNode.g_movementCost = 0;
	else
		tempNode.g_movementCost = moveCost + path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList].g_movementCost;

	tempNode.h_estMoveCost = gam_AStarFindDistance (whichTile, path[whichPath].destTile);
	tempNode.f_score       = tempNode.g_movementCost + tempNode.h_estMoveCost;

#ifdef DEBUG_ASTAR
	con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("Tempnode movement cost [ %i ] estMoveCost [ %i ] f_score [ %i ]", tempNode.g_movementCost, tempNode.h_estMoveCost, tempNode.f_score));
#endif

	path[whichPath].openNodes.push_back (tempNode);

#ifdef DEBUG_ASTAR
	con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("Path [ %i ] Size [ %i ] parent [ %i ] moveCost [ %i ] distanceCost [ %i ]", whichPath, path[whichPath].openNodes.size (), parent, tempNode.g_movementCost, tempNode.h_estMoveCost));
#endif
}

//--------------------------------------------------------------------------------------------------------
//
// Run this to start the pathfinding routine - pass in path index
static int gam_processAStarWithThread (void *ptr)
//--------------------------------------------------------------------------------------------------------
{
	int pathIndex;
	int testIndex;

	pathIndex = reinterpret_cast<int>((int *) ptr);
	testIndex = pathIndex;

	gam_AStarSearchThread (testIndex);

	return 0;
}

//--------------------------------------------------------------------------------------------------------
//
// See if the path is ready to use
bool gam_isAStarReady(int whichPath)
//--------------------------------------------------------------------------------------------------------
{
	return path[whichPath].pathReady;
}

//--------------------------------------------------------------------------------------------------------
//
// Start a new path and create a detached thread to run the pathfinding
int gam_requestNewPath (b2Vec2 start, b2Vec2 destination, int whichDroid, std::string whichLevel)
//--------------------------------------------------------------------------------------------------------
{
	static bool initDone = false;

	_nodeList  tempPath;
	double     distanceTest;
	SDL_Thread *thread;

	distanceTest = static_cast<double>(b2Distance (start, destination));
	if (distanceTest < 2)   // 2 Tiles
		return PATH_TOO_SHORT;

	//
	// Setup a number of paths ready to use
	if (!initDone)
	{
		for (auto i = 0; i != numAStarPaths; i++)
		{
			tempPath.inUse          = false;
			tempPath.wayPointsReady = false;
			tempPath.pathReady      = false;
			tempPath.whichDroid     = NO_DROID;
			tempPath.whichLevel     = "";
			path.push_back (tempPath);
		}
		initDone = true;
	}
	//
	// Find an unused path slot and use it
	for (int   i = 0; i != static_cast<int>(path.size ()); i++)
	{
		if (!path[i].inUse)
		{
			path[i].pathReady                = false;
			path[i].wayPointsReady           = false;
			path[i].startTile                = start;
			path[i].destTile                 = destination;
			path[i].currentNodePtrClosedList = -1;
			path[i].whichDroid               = whichDroid;
			path[i].whichLevel               = whichLevel;
			path[i].openNodes.reserve (initialNumReserveNodes);
			path[i].closedNodes.reserve (initialNumReserveNodes);
			path[i].inUse = true;
			gam_AStarAddTileToOpenNode (i, start, 0, -1);
#ifdef DEBUG_ASTAR
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("Requesting new path start [ %32.f %3.2f ] Dest [ %3.2f %3.2f ]", start.x, start.y, destination.x, destination.y));
#endif

			// TODO - change to real droid
			testDroid.aStarIndex = i;
			//
			// Create a thread and detach so it runs the pathfinding
			thread = SDL_CreateThread (gam_processAStarWithThread, "pathFindingThread", reinterpret_cast<void *>(i));
			SDL_DetachThread (thread);

			return i;
		}
	}
	sys_shutdownWithError (sys_getString ("Not enough path entries allocated. Increase the limit from the script file."));
}

//--------------------------------------------------------------------------------------------------------
//
// Find the node with the lowest totalCost
int gam_AStarFindLowestCostNode (int whichPath)
//--------------------------------------------------------------------------------------------------------
{
	int lowestCost;
	int lowestNodeIndexArray;

	lowestCost           = 50000;
	lowestNodeIndexArray = 0;

	if (path[whichPath].openNodes.empty ())
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("ERROR: No open nodes to search in gam_AStarFindLowestCostNode - path [ %i ]", whichPath));
		return -1;
	}

#ifdef DEBUG_ASTAR
	con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("Search the open nodes and find the one with the lowest total cost"));
#endif

	for (int i = 0; i != (int) path[whichPath].openNodes.size (); i++)
	{
		if (path[whichPath].openNodes[i].f_score < lowestCost)  // Change to 0 ??
		{
			lowestCost           = path[whichPath].openNodes[i].f_score;
			lowestNodeIndexArray = i;
		}
	}
	return lowestNodeIndexArray;
}

//--------------------------------------------------------------------------------------------------------
//
// Move a node from the openNode list to the closedNode list
void gam_AStarMoveNodeToClosedList (int whichPath, int whichNodeIndex)
//--------------------------------------------------------------------------------------------------------
{
	_pathNode                        tempNode;
	std::vector<_pathNode>::iterator itr;
	int                              indexCount = 0;

#ifdef DEBUG_ASTAR
	con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("Move open node [ %i ] to closedNode list", whichNodeIndex));
#endif

	tempNode = path[whichPath].openNodes[whichNodeIndex];
	path[whichPath].closedNodes.push_back (tempNode);
	path[whichPath].currentNodePtrClosedList = path[whichPath].closedNodes.size () - 1;

#ifdef DEBUG_ASTAR
	printf ("currentNodePtrClosedList is now [ %i ] - number closedNodes [ %i ]\n", path[whichPath].currentNodePtrClosedList, static_cast<int>(path[whichPath].closedNodes.size ()) - 1);
#endif

	indexCount = 0;
	for (itr   = path[whichPath].openNodes.begin (); itr != path[whichPath].openNodes.end (); itr++)
	{
		if (indexCount == whichNodeIndex)
		{
			path[whichPath].openNodes.erase (itr);
			return;
		}
		indexCount++;
	}
}

//--------------------------------------------------------------------------------------------------------
//
// Is a tile considered 'solid' or not
bool gam_AStarIsTileSolid (int tileIndex, int whichPath)
//--------------------------------------------------------------------------------------------------------
{
	if (tileIndex < 0)
		return true;

	if (tileIndex > shipdecks.at (path[whichPath].whichLevel).tiles.size ())
		sys_shutdownWithError (sys_getString ("Access outside size of tile array."));

	int whichTile = shipdecks.at (path[whichPath].whichLevel).tiles[tileIndex];

	switch (whichTile)
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

	if (whichTile <= NO_PASS_TILE)    // This stops the wall tile being considered passable
		return true;
	else
		return false;

	return true;    // Should never get here
}

//--------------------------------------------------------------------------------------------------------
//
// Check if node is in the CLOSED list
bool gam_AStarIsNodeInClosedList (int whichPath, b2Vec2 whichNode)
//--------------------------------------------------------------------------------------------------------
{
	whichNode.x = (int) whichNode.x;
	whichNode.y = (int) whichNode.y;

	for (int i = 0; i != (int) path[whichPath].closedNodes.size (); i++)
	{
		if ((whichNode.x == path[whichPath].closedNodes[i].tileLocation.x) && (whichNode.y == path[whichPath].closedNodes[i].tileLocation.y))
			return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------
//
// Check if node is in the OPEN list
bool gam_AStarIsNodeInOpenList (int whichPath, b2Vec2 whichNode)
//--------------------------------------------------------------------------------------------------------
{
	whichNode.x = (int) whichNode.x;
	whichNode.y = (int) whichNode.y;

	for (int i = 0; i != (int) path[whichPath].openNodes.size (); i++)
	{
		if ((whichNode.x == path[whichPath].openNodes[i].tileLocation.x) && (whichNode.y == path[whichPath].openNodes[i].tileLocation.y))
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
bool gam_AStarGenerateNewNode (int whichPath, int whichDirection)
//--------------------------------------------------------------------------------------------------------
{
	std::string directionStr;
	_pathNode   tempNode;
	int         moveTileCost;

	if (path[whichPath].pathReady)
		return true;

#ifdef DEBUG_ASTAR
	con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("---"));
	con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("Generate a new node based on the node with the current lowest cost."));
#endif

	switch (whichDirection)
	{
		case NODE_LEFT:
			tempNode.tileLocation.x = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList].tileLocation.x - 1;
			tempNode.tileLocation.y = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList].tileLocation.y;
			moveTileCost = 10;
			directionStr = "LEFT";
			break;

		case NODE_UP:
			tempNode.tileLocation.x = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList].tileLocation.x;
			tempNode.tileLocation.y = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList].tileLocation.y - 1;
			moveTileCost = 10;
			directionStr = "UP";
			break;

		case NODE_RIGHT:
			tempNode.tileLocation.x = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList].tileLocation.x + 1;
			tempNode.tileLocation.y = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList].tileLocation.y;
			moveTileCost = 10;
			directionStr = "RIGHT";
			break;

		case NODE_DOWN:
			tempNode.tileLocation.x = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList].tileLocation.x;
			tempNode.tileLocation.y = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList].tileLocation.y + 1;
			moveTileCost = 10;
			directionStr = "DOWN";
			break;
	}
#ifdef DEBUG_ASTAR
	con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("Parent [ %i ] Direction [ %s ]", path[whichPath].currentNodePtrClosedList, directionStr.c_str ()));
#endif

	//
	// Change to whole numbers for matching later on
	//
	tempNode.tileLocation.x = (int) tempNode.tileLocation.x;
	tempNode.tileLocation.y = (int) tempNode.tileLocation.y;

#ifdef DEBUG_ASTAR
	con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("See if the node is solid or not"));
#endif

	if (gam_AStarIsTileSolid ((tempNode.tileLocation.y * (shipdecks.at (path[whichPath].whichLevel).levelDimensions.x) + tempNode.tileLocation.x), whichPath))
	{
		return false; // Solid - don't use
	}

	int tileCostModify = gam_getInfluenceMapValue ((tempNode.tileLocation.y * (shipdecks.at (path[whichPath].whichLevel).levelDimensions.x) + tempNode.tileLocation.x));
	moveTileCost += tileCostModify;

	static int count = 0;

	if (path[whichPath].currentNodePtrClosedList == 59)
	{
		std::cout << "Break here : " << count << std::endl;
		count++;
	}

	//
	// See if we have found a path to the destination tile
	if ((tempNode.tileLocation.x == path[whichPath].destTile.x) && (tempNode.tileLocation.y == path[whichPath].destTile.y))
	{
		gam_AStarAddTileToOpenNode (whichPath, tempNode.tileLocation, moveTileCost, path[whichPath].currentNodePtrClosedList);
		gam_AStarMoveNodeToClosedList (whichPath, path[whichPath].openNodes.size ());
		path[whichPath].pathReady = true;
		return true;
	}
	//
	// See if the node is on the closed list or not
	if (gam_AStarIsNodeInClosedList (whichPath, tempNode.tileLocation))
	{
		return false; // Already in closed list - do not use
	}
	//
	// If it isn't on the open list - add it
	if (!gam_AStarIsNodeInOpenList (whichPath, tempNode.tileLocation))
	{
		gam_AStarAddTileToOpenNode (whichPath, tempNode.tileLocation, moveTileCost, path[whichPath].currentNodePtrClosedList);
		return false;
	}
	//
	// The node is already on the open list - see if the runningCost to this node is better
	int    nodeIndex = 0;
	size_t openNodeSize;
	size_t closedNodeSize;

	nodeIndex      = path[whichPath].currentNodePtrClosedList;
	openNodeSize   = path[whichPath].openNodes.size ();
	closedNodeSize = path[whichPath].closedNodes.size ();

	if (nodeIndex <= static_cast<int>(closedNodeSize - 1))
	{
		if (path[whichPath].openNodes[openNodeSize - 1].g_movementCost < 0)
		{
			sys_shutdownWithError (sys_getString ("Invalid movement cost [ %i ]", path[whichPath].openNodes[openNodeSize - 1].g_movementCost));
		}

		if (path[whichPath].openNodes[openNodeSize - 1].g_movementCost < path[whichPath].closedNodes[nodeIndex - 1].g_movementCost)
		{
#ifdef DEBUG_ASTAR
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("New node is better than the old one"));
#endif
			gam_AStarAddTileToOpenNode (whichPath, path[whichPath].openNodes[openNodeSize - 1].tileLocation, moveTileCost, path[whichPath].currentNodePtrClosedList);
		}
	}
	return true;
}

//--------------------------------------------------------------------------------------------------------
//
// Extract the path from the list of closed nodes
void gam_AStarExtractPath (int whichPath)
//--------------------------------------------------------------------------------------------------------
{
	int       nodeParent = -1;
	_pathNode tempNode;

	//
	// Store the destination tile
	tempNode.tileLocation = path[whichPath].destTile;
	path[whichPath].foundPath.push_back (tempNode);
	//
	// Get the next tile along
	tempNode.tileLocation = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList - 1].tileLocation;
	path[whichPath].foundPath.push_back (tempNode);
	//
	// What's the parent
	nodeParent = path[whichPath].closedNodes[path[whichPath].currentNodePtrClosedList - 1].parent;
	//
	// While we haven't reached the starting tile ( no parent )
	while (-1 != nodeParent)
	{
		tempNode = path[whichPath].closedNodes[nodeParent];
		path[whichPath].foundPath.push_back (tempNode);

		nodeParent = path[whichPath].closedNodes[nodeParent].parent;

		if ((tempNode.tileLocation.x == path[whichPath].startTile.x) && (tempNode.tileLocation.y == path[whichPath].startTile.y))
		{
			path[whichPath].foundPath.push_back (tempNode);
			return;
		}
	}
}

//--------------------------------------------------------------------------------------------------------
//
// Compress the waypoints
void gam_AStarCompressWaypoints (int whichPath)
//--------------------------------------------------------------------------------------------------------
{
	int                 current = 1;
	std::vector<b2Vec2> newPoints;
	b2Vec2              tempPoint;

	tempPoint = path[whichPath].wayPoints[0];
	newPoints.push_back (tempPoint);

	for (int i = 0; i != (int) path[whichPath].wayPoints.size () - 1; i++)
	{
		if ((tempPoint.x != path[whichPath].wayPoints[current].x) || (tempPoint.y != path[whichPath].wayPoints[current].y))
		{
			tempPoint = path[whichPath].wayPoints[current];
			newPoints.push_back (tempPoint);
			current++;
		}
		else
		{
			current++;
		}
	}

//	newPoints.push_back (path[whichPath].wayPoints[current]);

	//
	// Copy back into structure
	path[whichPath].wayPoints.clear ();
	for (int i = 0; i != (int) newPoints.size () - 1; i++)
	{
		path[whichPath].wayPoints.push_back (newPoints[i]);

#ifdef DEBUG_ASTAR
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("%i %3.2f %3.2f\n", i, path[whichPath].wayPoints[i].x, path[whichPath].wayPoints[i].y));
#endif
	}
}

//--------------------------------------------------------------------------------------------------------
//
// Turn the AStar nodes into world position coords
void gam_AStarConvertToCoords (int whichPath)
//--------------------------------------------------------------------------------------------------------
{
	b2Vec2 tempWaypoint;

	for (int i = 0; i != (int) path[whichPath].foundPath.size (); i++)
	{
		tempWaypoint.x = path[whichPath].foundPath[i].tileLocation.x * tileSize;
		tempWaypoint.y = path[whichPath].foundPath[i].tileLocation.y * tileSize;

		tempWaypoint.x += tileSize / 2;
		tempWaypoint.y += tileSize / 2;

		path[whichPath].wayPoints.push_back (tempWaypoint);

		tempWaypoint.x /= tileSize;
		tempWaypoint.y /= tileSize;
	}

	return;

	if (path[whichPath].wayPoints.size () > 4)
		gam_AStarCompressWaypoints (whichPath);
}

//--------------------------------------------------------------------------------------------------------
//
// Clear coord memory and flag ready for reuse
void gam_AStarRemovePath (int whichPath)
//--------------------------------------------------------------------------------------------------------
{
	if (!path[whichPath].inUse)
	{
		printf ("Error: Path not in use - can not remove. Path [ %i ]\n", whichPath);
		return;
	}

	// TODO - reset droid aStarIndexes
//	shipdecks.at (path[whichPath].whichLevel).droids[path[whichPath].whichDroid].aStarPathIndex = -1;

	path[whichPath].pathReady                = false;
	path[whichPath].wayPointsReady           = false;
	path[whichPath].currentNodePtrClosedList = 0;
	path[whichPath].whichDroid               = -1;
	path[whichPath].whichLevel               = "";
	path[whichPath].openNodes.clear ();
	path[whichPath].closedNodes.clear ();
	path[whichPath].wayPoints.clear ();
	path[whichPath].foundPath.clear ();
	path[whichPath].inUse = false;
}

//--------------------------------------------------------------------------------------------------------
//
// Reset all path entries
void gam_resetAllPaths ()
//--------------------------------------------------------------------------------------------------------
{
	for (int i = 0; i != (int) path.size (); i++)
	{
		gam_AStarRemovePath (i);
	}
}

//--------------------------------------------------------------------------------------------------------
//
// A thread to run the aStar searching function
void gam_AStarSearchThread (int whichPathArg)
//--------------------------------------------------------------------------------------------------------
{
	int currentNodeIndex = 0;

#ifdef DEBUG_ASTAR
	con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("searchThread - whichPath [ %i ]", whichPathArg));
#endif

	if (path.size () == 0)
		return;

	if ((whichPathArg < 0) || (whichPathArg > static_cast<int>(path.size ())))
		return;

	if (!path[whichPathArg].inUse)
		return;

	std::cout << "Path index : " << whichPathArg << std::endl;

	while (!path[whichPathArg].pathReady)
	{
		if (!path[whichPathArg].pathReady)
		{
			// Look for the lowest F cost node on the open list - this becomes the current node
			currentNodeIndex = gam_AStarFindLowestCostNode (whichPathArg);
//		if (currentNodeIndex < 0)
//			return;

			// Move it to the closed node list
			gam_AStarMoveNodeToClosedList (whichPathArg, currentNodeIndex);

			// For each of the surrounding nodes
			gam_AStarGenerateNewNode (whichPathArg, NODE_RIGHT);
			gam_AStarGenerateNewNode (whichPathArg, NODE_LEFT);
			gam_AStarGenerateNewNode (whichPathArg, NODE_UP);
			gam_AStarGenerateNewNode (whichPathArg, NODE_DOWN);
		}
		else
		{
			gam_AStarExtractPath (whichPathArg);
			gam_AStarConvertToCoords (whichPathArg);
			path[whichPathArg].wayPointsReady = true;
		}
	}

	gam_AStarExtractPath (whichPathArg);
	gam_AStarConvertToCoords (whichPathArg);
	path[whichPathArg].wayPointsReady = true;
}
