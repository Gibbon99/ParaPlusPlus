#include <game/shipDecks.h>
#include <game/player.h>
#include <system/util.h>
#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include <game/pathFind.h>
#include "classes/paraAI.h"

//-----------------------------------------------------------------------------------------------------------------------
//
// Constructor
paraAI::paraAI ()
//-----------------------------------------------------------------------------------------------------------------------
{
	//
	// Insert a value for each action score
	for (auto i = 0; i != AI_MODE_NUMBER; i++)
	{
		ai.push_back (0);
	}
	currentAIMode = AI_MODE_NUMBER;
	ai[AI_MODE_PATROL] = 50;
	wayPointDirection = WAYPOINT_DOWN;
	currentSpeed      = 0;
	aiActionCounter   = 0.0;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Deconstructor
paraAI::~paraAI ()
//-----------------------------------------------------------------------------------------------------------------------
{
	ai.clear ();
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Perform the Patrol action
void paraAI::patrol ()
//-----------------------------------------------------------------------------------------------------------------------
{
	b2Transform transform;
	float       lookAheadDistance = 0.0f;

//	destinationCoordsInMeters.x = destination.x / pixelsPerMeter;
//	destinationCoordsInMeters.y = destination.y / pixelsPerMeter;

	destinationCoordsInMeters = sys_convertToMeters (destination);

	wayPointDistanceInMeters = b2Distance (destinationCoordsInMeters, worldPositionInMeters);
	if (wayPointDistanceInMeters < 0.5)
	{
		getWaypointDestination ();
	}

	currentSpeed += acceleration;
	if (currentSpeed > maxSpeed)
		currentSpeed         = maxSpeed;

	currentVelocity = (destinationCoordsInMeters - worldPositionInMeters);
	currentVelocity.Normalize ();
	lookAheadVelocity      = currentVelocity;
	currentVelocity *= static_cast<float>(currentSpeed);

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
		playerDroidFixture = playerDroid.body->GetFixtureList ();    // Only have one fixture per body

	if (playerDroidFixture->TestPoint (lookAheadVelocity + worldPositionInMeters))
	{
		currentSpeed    = 0.0;
		currentVelocity = {0.0, 0.0};
		swapWaypointDirection ();
		aiActionCounter = 3.0;
		return;
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

	destination.x = shipdecks.at (gam_getCurrentDeckName ()).wayPoints[wayPointIndex].x;
	destination.y = shipdecks.at (gam_getCurrentDeckName ()).wayPoints[wayPointIndex].y;
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
//	if (aStarWaypointIndex > path[aStarIndex].wayPoints.size () - 1)
	if (aStarWaypointIndex == 0)
	{
		// Finished aStar
		modifyScore (AI_MODE_HEAL, -ai[AI_MODE_HEAL]);  // Reset to zero - should be fully healed
		//
		// Find nearest waypoint
		for (auto wayPointItr : shipdecks.at (gam_getCurrentDeckName ()).wayPoints)
		{
			currentDistance = b2Distance (worldPositionInMeters, sys_convertToMeters (shipdecks.at (gam_getCurrentDeckName ()).wayPoints[nearestWayPointIndex]));
			if (currentDistance < shortestDistance)
			{
				shortestDistance = currentDistance;
				newIndex         = nearestWayPointIndex;
			}
			nearestWayPointIndex++;
		}
		//
		// Got the nearest waypoint - pathfind to it    // Will it find one through a wall ??
		wayPointIndex = nearestWayPointIndex;
		getWaypointDestination ();
		return;     // Should now go back to patrol
	}

	destination = path[aStarIndex].wayPoints[aStarWaypointIndex];
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Run the heal action
void paraAI::heal ()
//-----------------------------------------------------------------------------------------------------------------------
{
	if (!gam_isAStarReady (aStarIndex))
		return;

	if (!haveAStarDestination)
	{
		aStarWaypointIndex = path[aStarIndex].wayPoints.size() - 1;
		destination = path[aStarIndex].wayPoints[aStarWaypointIndex];
		haveAStarDestination = true;

		std::cout << " Destination to start AStar : " << destination.x << " " << destination.y << std::endl;
		std::cout << "Current destination is : " << worldPositionInMeters.x << " " << worldPositionInMeters.y << std::endl;
	}

	destinationCoordsInMeters = sys_convertToMeters (destination);

	wayPointDistanceInMeters = b2Distance (destinationCoordsInMeters, worldPositionInMeters);
	if (wayPointDistanceInMeters < 0.5)
	{
		getNextAStarDestination ();
	}

	currentSpeed += acceleration;
	if (currentSpeed > maxSpeed)
		currentSpeed         = maxSpeed;

	currentVelocity = (destinationCoordsInMeters - worldPositionInMeters);
	currentVelocity.Normalize ();
	lookAheadVelocity = currentVelocity;
	currentVelocity *= static_cast<float>(currentSpeed);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Change to a new mode - run functions required before running this action
void paraAI::changeModeTo (int newAIMode)
//-----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2 tileDestination;
	b2Vec2 worldPosInPixels;

	currentAIMode = newAIMode;

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

			std::cout << "Changing to HEAL mode. Requesting aStar to healing tile." << std::endl;

			if (!haveRequestedAStar)
			{
				haveRequestedAStar   = true;
				haveAStarDestination = false;
				aStarWaypointIndex   = 0;
				tileDestination      = findHealingTile ();
				if (tileDestination.x < 0)                // No healing tiles on level - change to flee
				{
					modifyScore (AI_MODE_HEAL, -100);
					modifyScore (AI_MODE_FLEE, +100);
					return;
				}

				worldPosInPixels = sys_convertToPixels (worldPositionInMeters);
				aStarIndex       = gam_requestNewPath (b2Vec2{worldPosInPixels.x / tileSize, worldPosInPixels.y / tileSize}, tileDestination, 0, gam_getCurrentDeckName ());
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
		if (currentDistance < shortestDistance)
		{
			shortestDistance   = currentDistance;
			healingDestination = sys_convertToTiles (healItr.worldPosInPixels);
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

//	std::cout << "Health percent is now : " << healthPercent << std::endl;
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

//	std::cout << "Modify heal score by : " << 100 - healthPercent << " score is now : " << ai[AI_MODE_HEAL] << std::endl;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Process the current AI mode - Pass in the current world position in meters
void paraAI::process (b2Vec2 newWorldPosInMeters)
//-----------------------------------------------------------------------------------------------------------------------
{
	worldPositionInMeters = newWorldPosInMeters;

//	checkHealth ();
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