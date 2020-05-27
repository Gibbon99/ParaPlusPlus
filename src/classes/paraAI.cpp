#include <game/shipDecks.h>
#include <game/player.h>
#include <system/util.h>
#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include <game/pathFind.h>
#include <classes/paraLOS.h>
#include <classes/paraRandom.h>
#include "classes/paraAI.h"

#define DEBUG_AI 1

//-----------------------------------------------------------------------------------------------------------------------
//
// Constructor
paraAI::paraAI ()
//-----------------------------------------------------------------------------------------------------------------------
{

}

//-----------------------------------------------------------------------------------------------------------------------
//
// Deconstructor
paraAI::~paraAI ()
//-----------------------------------------------------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Get a string from the current AI Mode
std::string paraAI::getString (int whichMode)
//-----------------------------------------------------------------------------------------------------------------------
{
	switch (whichMode)
	{
		case AI_MODE_PATROL:
			return "AI_MODE_PATROL";
			break;
		case AI_MODE_HELP:
			return "AI_MODE_HELP";
			break;

		case AI_MODE_FLEE:
			return "AI_MODE_FLEE";
			break;

		case AI_MODE_HEAL:
			return "AI_MODE_HEAL";
			break;

		case AI_MODE_ATTACK:
			return "AI_MODE_ATTACK";
			break;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Insert a value for each action score
void paraAI::initAI ()
//-----------------------------------------------------------------------------------------------------------------------
{
	for (auto i = 0; i != AI_MODE_NUMBER; i++)
	{
		ai[i] = 0;
	}
	currentAIMode = AI_MODE_NUMBER;     // Will set to AI_MODE_PATROL when checking scores first run
	patrolAction  = NORMAL_PATROL;
	ai[AI_MODE_PATROL] = 50;
	ai[AI_MODE_HEAL]   = 0;
	wayPointDirection = WAYPOINT_DOWN;
	currentSpeed      = 0;
	aiActionCounter   = 0.0;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Do any acceleration and apply current speed to velocity vector
void paraAI::changeVelocity ()
//-----------------------------------------------------------------------------------------------------------------------
{
	currentSpeed += acceleration;
	if (currentSpeed > maxSpeed)
		currentSpeed = maxSpeed;

	currentVelocity = (destinationCoordsInMeters - worldPositionInMeters);
	currentVelocity.Normalize ();
	lookAheadVelocity = currentVelocity;
	currentVelocity *= static_cast<float>(currentSpeed);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Find the nearest location taking into account solid walls
b2Vec2 paraAI::findLocationWithLOS (int locationType)
//-----------------------------------------------------------------------------------------------------------------------
{
	RayCastAnyCallback callback;
	int                newWaypointIndex;
	int                shortestDistance;
	int                currentDistance;

#ifdef DEBUG_AI
	std::cout << "[ " << arrayIndex << " ] - Search for a close waypoint." << std::endl;
#endif

	switch (locationType)
	{
		case LOCATION_WAYPOINT:
			//
			// Find nearest waypoint that isn't blocked by a wall
			destinationCoordsInMeters.x = -1;
			shortestDistance = 1000;

			for (newWaypointIndex = 0; newWaypointIndex != static_cast<int>(g_shipDeckItr->second.wayPoints.size () - 1); newWaypointIndex++)
			{
				sys_getPhysicsWorld ()->RayCast (&callback, worldPositionInMeters, sys_convertToMeters (g_shipDeckItr->second.wayPoints[newWaypointIndex]));
				if (!callback.m_hit)
				{
					currentDistance = b2Distance (worldPositionInMeters, sys_convertToMeters (g_shipDeckItr->second.wayPoints[newWaypointIndex]));
					if (currentDistance < shortestDistance)
					{
						shortestDistance          = currentDistance;
						destinationCoordsInMeters = sys_convertToMeters (g_shipDeckItr->second.wayPoints[newWaypointIndex]);
						wayPointIndex             = newWaypointIndex;
#ifdef DEBUG_AI
						std::cout << "[ " << arrayIndex << " ] - Found a closer waypoint [ " << wayPointIndex << " ] [" << shortestDistance << " ] " << std::endl;
#endif
					}
				}
			}
			return destinationCoordsInMeters;
			break;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Perform the Patrol action
void paraAI::patrol ()
//-----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2     tileDestinationInTiles;
	b2Vec2     worldPosInTiles;
	paraRandom randomWaypoint;

	switch (patrolAction)
	{
		case NORMAL_PATROL:
			wayPointDistanceInMeters = b2Distance (destinationCoordsInMeters, worldPositionInMeters);
			if (wayPointDistanceInMeters < 0.5)
			{
				getWaypointDestination ();
			}

			changeVelocity ();

			//
			// Give droid time to move away
			if (aiActionCounter > 0.0)
			{
				aiActionCounter -= 0.3;
				if (aiActionCounter < 0.0)
					aiActionCounter = 0.0;
				return;
			}
			//
			// Change direction if going to run into player
			lookAheadVelocity *= LOOK_AHEAD_DISTANCE;
			if (nullptr == playerDroidFixture)
				playerDroidFixture   = playerDroid.body->GetFixtureList ();    // Only have one fixture per body

			if (playerDroidFixture->TestPoint (lookAheadVelocity + worldPositionInMeters))
			{
				currentSpeed    = 0.0;
				currentVelocity = {0.0, 0.0};
				swapWaypointDirection ();
				aiActionCounter = 3.0;
				return;
			}
			break;

		case FIND_WAYPOINT:
			destinationCoordsInMeters = findLocationWithLOS (LOCATION_WAYPOINT);
			if (destinationCoordsInMeters.x > 0)
			{
				patrolAction    = MOVE_DIRECT_TO_LOCATION;
				currentVelocity = {0, 0};
#ifdef DEBUG_AI
				std::cout << "Found the closest waypoint that is reachable directly." << std::endl;
#endif
				return;
			}
#ifdef DEBUG_AI
			std::cout << "Could not find suitable waypoint for Patrol - starting from random index." << std::endl;
#endif
			//
			// Didn't find any not blocked by a wall so do aStar trace to find one
			currentVelocity        = {0, 0};
			wayPointIndex          = randomWaypoint.get (0, static_cast<int>(g_shipDeckItr->second.wayPoints.size () - 1));
			tileDestinationInTiles = g_shipDeckItr->second.wayPoints[wayPointIndex];   // Go to random waypoint
			tileDestinationInTiles = sys_convertToTiles (tileDestinationInTiles);
			worldPosInTiles        = sys_convertToTiles (sys_convertToPixels (worldPositionInMeters));
			currentVelocity        = {0, 0};
			aStarWaypointIndex     = 0;
			haveAStarDestination   = false;
			patrolAction           = FOLLOW_ASTAR;
			aStarIndex             = gam_requestNewPath (b2Vec2{worldPosInTiles.x, worldPosInTiles.y}, tileDestinationInTiles, arrayIndex, gam_getCurrentDeckName ());

			if (aStarIndex == PATH_TOO_SHORT)
			{
				sys_shutdownWithError ("Unable to locate suitable path. Path is too short");
			}
			return;     // Should now go back to patrol

			break;

		case FOLLOW_ASTAR:
			followAStar ();
			break;

		case MOVE_DIRECT_TO_LOCATION:
			changeVelocity ();
			//
			// Have we reached the destination
			wayPointDistanceInMeters = b2Distance (destinationCoordsInMeters, worldPositionInMeters);
			if (wayPointDistanceInMeters < 0.5)
			{
				patrolAction = NORMAL_PATROL;
				getWaypointDestination ();
			}
			break;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Render the direction vector
void paraAI::renderVelocity ()
//-----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2 drawStart;
	b2Vec2 drawFinish;

	drawStart  = sys_convertToPixels (worldPositionInMeters);
	drawFinish = sys_convertToPixels (lookAheadVelocity);

	drawFinish += drawStart;

	drawStart  = sys_worldToScreen (drawStart, 100);
	drawFinish = sys_worldToScreen (drawFinish, 100);

	thickLineRGBA (renderer.renderer, drawStart.x, drawStart.y, drawFinish.x, drawFinish.y, 2, 0, 200, 200, 255);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Increase a utility action score - modify can be positive or negative value
void paraAI::modifyScore (int whichScore, int modifyAmount)
//-----------------------------------------------------------------------------------------------------------------------
{
	if ((whichScore < 0) || (whichScore > AI_MODE_NUMBER))
		return;

	ai[whichScore] += modifyAmount;

	ai[whichScore] = ai[whichScore] > 100 ? 100 : ai[whichScore];
	ai[whichScore] = ai[whichScore] < 0 ? 0 : ai[whichScore];
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the acceleration for this droid
void paraAI::setAcceleration (double newAcceleration)
//-----------------------------------------------------------------------------------------------------------------------
{
	acceleration = newAcceleration;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the current acceleration for this droid
double paraAI::getAcceleration ()
//-----------------------------------------------------------------------------------------------------------------------
{
	return acceleration;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the current maxSpeed for this droid
double paraAI::getMaxSpeed ()
//-----------------------------------------------------------------------------------------------------------------------
{
	return maxSpeed;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the current velocity vector
b2Vec2 paraAI::getVelocity ()
//-----------------------------------------------------------------------------------------------------------------------
{
	return currentVelocity;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the top speed for this droid
void paraAI::setMaxSpeed (double newMaxSpeed)
//-----------------------------------------------------------------------------------------------------------------------
{
	maxSpeed = newMaxSpeed;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the waypoint index
void paraAI::setWaypointIndex (int newIndex)
//-----------------------------------------------------------------------------------------------------------------------
{
	wayPointIndex = newIndex;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Get the current mode
int paraAI::getCurrentMode ()
//-----------------------------------------------------------------------------------------------------------------------
{
	return currentAIMode;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Get the current aStar value
int paraAI::getAStarIndex ()
//-----------------------------------------------------------------------------------------------------------------------
{
	return aStarIndex;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Get the current Patrol Action
std::string paraAI::getPatrolAction ()
//-----------------------------------------------------------------------------------------------------------------------
{
	switch (patrolAction)
	{
		case NORMAL_PATROL:
			return "NORMAL_PATROL";
			break;

		case FIND_WAYPOINT:
			return "FIND_WAYPOINT";
			break;

		case FOLLOW_ASTAR:
			return "FOLLOW_ASTAR";
			break;

		case MOVE_DIRECT_TO_LOCATION:
			return "MOVE_DIRECT_TO_LOCATION";
			break;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Someone shot this droid - set them as the target for an attack
void paraAI::setTargetDroid (int newTargetDroid)
//-----------------------------------------------------------------------------------------------------------------------
{
	targetDroid = newTargetDroid;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Get the next destination coords from the index
void paraAI::getWaypointDestination ()
//-----------------------------------------------------------------------------------------------------------------------
{
	switch (wayPointDirection)
	{
		case WAYPOINT_DOWN:
			wayPointIndex--;
			if (wayPointIndex < 0)
				wayPointIndex = static_cast<int>(shipdecks.at (gam_getCurrentDeckName ()).wayPoints.size () - 1);
			break;

		case WAYPOINT_UP:
			wayPointIndex++;
			if (wayPointIndex > static_cast<int>(shipdecks.at (gam_getCurrentDeckName ()).wayPoints.size () - 1))
				wayPointIndex = 0;
			break;
	}

	destinationCoordsInMeters = sys_convertToMeters (shipdecks.at (gam_getCurrentDeckName ()).wayPoints[wayPointIndex]);
//	destination.x = shipdecks.at (gam_getCurrentDeckName ()).wayPoints[wayPointIndex].x;
//	destination.y = shipdecks.at (gam_getCurrentDeckName ()).wayPoints[wayPointIndex].y;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Change waypoint direction
void paraAI::swapWaypointDirection ()
//-----------------------------------------------------------------------------------------------------------------------
{
	if (wayPointDirection == WAYPOINT_UP)
		wayPointDirection = WAYPOINT_DOWN;
	else
		wayPointDirection = WAYPOINT_UP;

	getWaypointDestination ();
}


//-----------------------------------------------------------------------------------------------------------------------
//
// Get the next waypoint from the aStar path
void paraAI::getNextAStarDestination ()
//-----------------------------------------------------------------------------------------------------------------------
{
	int   nearestWayPointIndex = 0;
	int   newIndex             = 0;
	float shortestDistance     = 1000;
	float currentDistance;

	aStarWaypointIndex--;
	if (aStarWaypointIndex < 0)    // Reached the AStar destination
	{
#ifdef DEBUG_AI
		std::cout << "[ " << arrayIndex << " ]" << " Reached the end of the AStar for mode " << getString (currentAIMode) << std::endl;
#endif
		switch (currentAIMode)
		{
			case AI_MODE_HEAL:
				currentAIMode   = AI_MODE_NUMBER;     // Set to something not AI_MODE_PATROL, otherwise the match stops the change happening
				patrolAction    = FIND_WAYPOINT;
				currentVelocity = {0, 0};
				//
				// Clear the path for reuse
				gam_AStarRemovePath (aStarIndex);
				aStarIndex           = -1;
				//haveRequestedAStar   = false;
				aStarWaypointIndex   = 0;
				haveAStarDestination = false;

				// Clear heal mode
				modifyScore (AI_MODE_HEAL, -100);  // Reset to zero - should be fully healed
#ifdef DEBUG_AI
				std::cout << "[ " << arrayIndex << " ]" << " Finished HEAL mode. Health score is now " << ai[AI_MODE_HEAL] << std::endl;
#endif
				break;

			case AI_MODE_PATROL:
				switch (patrolAction)
				{
					case FOLLOW_ASTAR:
						//
						// Reached destination - change back to normal patrol
						patrolAction = NORMAL_PATROL;
						getWaypointDestination ();
						currentVelocity = {0, 0};
						//
						// Clear the path for reuse
						gam_AStarRemovePath (aStarIndex);
						aStarIndex           = -1;
						//haveRequestedAStar   = false;
						aStarWaypointIndex   = 0;
						haveAStarDestination = false;

#ifdef DEBUG_AI
						std::cout << "[ " << arrayIndex << " ]" << " Finished following ASTAR - change back to NORMAL_PATROL : " << getString (currentAIMode) << std::endl;
#endif
						break;
				}
		}

		return;
	}
	destinationCoordsInMeters = sys_convertToMeters (path[aStarIndex].wayPoints[aStarWaypointIndex]);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Follow a AStar path
void paraAI::followAStar ()
//-----------------------------------------------------------------------------------------------------------------------
{
	if (!gam_isAStarReady (aStarIndex))
		return;

	if (!haveAStarDestination)
	{
		aStarWaypointIndex        = static_cast<int>(path[aStarIndex].wayPoints.size () - 1);
		destinationCoordsInMeters = sys_convertToMeters (path[aStarIndex].wayPoints[aStarWaypointIndex]);
		haveAStarDestination      = true;
	}

	wayPointDistanceInMeters = b2Distance (destinationCoordsInMeters, worldPositionInMeters);
	if (wayPointDistanceInMeters < 0.5)
	{
		getNextAStarDestination ();
	}

	changeVelocity ();
};

//-----------------------------------------------------------------------------------------------------------------------
//
// Run the heal action
void paraAI::heal ()
//-----------------------------------------------------------------------------------------------------------------------
{
	followAStar ();
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the index into the vector array for this droid
void paraAI::setArrayIndex (int newIndex)
//-----------------------------------------------------------------------------------------------------------------------
{
	arrayIndex = newIndex;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Change to a new mode - run functions required before running this action
void paraAI::changeModeTo (int newAIMode)
//-----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2 tileDestination;
	b2Vec2 localWorldPosInPixels;

	currentAIMode = newAIMode;
	if (aStarIndex > -1)        // Remove any aStar in use if changing mode
	{
		gam_AStarRemovePath (aStarIndex);
		aStarIndex = -1;
	}

#ifdef DEBUG_AI
	std::cout << "[ " << arrayIndex << " ]" << " Changing to new AI mode : " << getString (currentAIMode) << " : " << getPatrolAction () << std::endl;
#endif

	switch (newAIMode)
	{
		case AI_MODE_PATROL:
			getWaypointDestination ();
			break;

		case AI_MODE_ATTACK:
			break;

		case AI_MODE_FLEE:
			break;

		case AI_MODE_HEAL:

#ifdef DEBUG_AI
			std::cout << "[ " << arrayIndex << " ]" << " Changing to HEAL mode. Requesting aStar to healing tile." << std::endl;
#endif
			haveAStarDestination = false;
			tileDestination      = findHealingTile ();
			if (tileDestination.x < 0)                // No healing tiles on level - change to flee
			{
				modifyScore (AI_MODE_HEAL, -100);
				modifyScore (AI_MODE_FLEE, +100);
				return;
			}

			localWorldPosInPixels = sys_convertToPixels (worldPositionInMeters);

			aStarIndex = gam_requestNewPath (b2Vec2{localWorldPosInPixels.x / tileSize, localWorldPosInPixels.y / tileSize}, tileDestination, arrayIndex, gam_getCurrentDeckName ());
			if (PATH_TOO_SHORT == aStarIndex)
			{
				modifyScore (AI_MODE_HEAL, -60); // Reset to below Patrol level
#ifdef DEBUG_AI
				std::cout << "[ " << arrayIndex << " ]" << " Too close to healing tile to get a path. Set patrolAction to FIND_WAYPOINT" << std::endl;
#endif
				patrolAction = FIND_WAYPOINT;
				checkScores ();
			}
			break;

		case AI_MODE_HELP:
			break;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Check to see if the current mode needs to change
void paraAI::checkScores ()
//-----------------------------------------------------------------------------------------------------------------------
{
	int highestScore = 0;
	int scoreMemory  = AI_MODE_NUMBER;

	sanityCheck ();

	for (auto i = 0; i != AI_MODE_NUMBER; i++)
	{
		if (ai[i] > highestScore)
		{
			highestScore = ai[i];
			scoreMemory  = i;
		}
	}
	if (scoreMemory == currentAIMode)     // already doing highest score action
		return;
	//
	// Change to new mode
	changeModeTo (scoreMemory);

#ifdef DEBUG_AI
	std::cout << "AI Score [ " << ai[0] << " " << ai[1] << " " << ai[2] << " " << ai[3] << " " << ai[4] << " ]" << std::endl;
#endif
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Find a healing tile - if none on this level then flee
b2Vec2 paraAI::findHealingTile ()
//-----------------------------------------------------------------------------------------------------------------------
{
	float  shortestDistance = 1000;
	float  currentDistance;
	b2Vec2 healingDestination;

	if (shipdecks.at (gam_getCurrentDeckName ()).healing.empty ())
	{
		healingDestination.x = -1;
		healingDestination.y = -1;
		return healingDestination;
	}

	for (auto healItr : shipdecks.at (gam_getCurrentDeckName ()).healing)
	{
		currentDistance = b2Distance (worldPositionInMeters, healItr.worldPosInMeters);
		if (currentDistance > 2.0)      // Needs to be far enough away to create a aStar path
		{
			if (currentDistance < shortestDistance)
			{
				shortestDistance   = currentDistance;
				healingDestination = sys_convertToTiles (healItr.worldPosInPixels);
			}
		}
	}
	return healingDestination;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the current percentage of health left
void paraAI::setHealthPercent (float newHealthPercent)
//-----------------------------------------------------------------------------------------------------------------------
{
	healthPercent = newHealthPercent;

#ifdef DEBUG_AI
	std::cout << "[ " << arrayIndex << " ] Health percent is now : " << healthPercent << std::endl;
#endif
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Do a check on current health level to see if the score needs to be modified
void paraAI::checkHealth ()
//-----------------------------------------------------------------------------------------------------------------------
{
	if (healthPercent > 99)
		return;

	modifyScore (AI_MODE_HEAL, 100 - healthPercent);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Process the current AI mode - Pass in the current world position in meters
void paraAI::process (b2Vec2 newWorldPosInMeters)
//-----------------------------------------------------------------------------------------------------------------------
{
	worldPositionInMeters = newWorldPosInMeters;

	checkScores ();

	switch (currentAIMode)
	{
		case AI_MODE_PATROL:
			patrol ();
			break;

		case AI_MODE_ATTACK:
			break;

		case AI_MODE_FLEE:
			break;

		case AI_MODE_HEAL:
			heal ();
			break;

		case AI_MODE_HELP:
			break;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Show the current AI array values
void paraAI::showValues ()
//-----------------------------------------------------------------------------------------------------------------------
{
	for (auto droidItr : shipdecks.at (gam_getCurrentDeckName ()).droid)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("%i %i %i %i %i %i %s - %s aStar %i ", droidItr.index, droidItr.ai.ai[0], droidItr.ai.ai[1], droidItr.ai.ai[2], droidItr.ai.ai[3], droidItr.ai.ai[4], getString (droidItr.ai.getCurrentMode ()).c_str (),
		                                                            droidItr.ai.getCurrentMode () == AI_MODE_PATROL ? droidItr.ai.getPatrolAction ().c_str () : " ", droidItr.ai.getAStarIndex ()));
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Check to fix HEAL
void paraAI::sanityCheck ()
//-----------------------------------------------------------------------------------------------------------------------
{
	if ((currentAIMode == AI_MODE_HEAL) && (ai[AI_MODE_HEAL] == 100) && (-1 == aStarIndex))
	{
		modifyScore (AI_MODE_HEAL, -60);
		patrolAction = FIND_WAYPOINT;

#ifdef DEBUG_AI
		std::cout << "[ " << arrayIndex << " ]" << " Droid in HEAL defect. Changing to Patrol." << std::endl;
#endif
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// DEBUG
int paraAI::getHealScore ()
//-----------------------------------------------------------------------------------------------------------------------
{
	return ai[AI_MODE_HEAL];
}
