#include <game/shipDecks.h>
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
		ai.push_back(0);
	}
	currentAIMode = AI_MODE_NUMBER;
	ai[AI_MODE_PATROL] = 50;
	wayPointDirection = WAYPOINT_DOWN;
	currentSpeed = 300;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Deconstructor
paraAI::~paraAI()
//-----------------------------------------------------------------------------------------------------------------------
{
	ai.clear();
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Perform the Patrol action
void paraAI::patrol()
//-----------------------------------------------------------------------------------------------------------------------
{
	destinationCoordsInMeters.x = destination.x / pixelsPerMeter;
	destinationCoordsInMeters.y = destination.y / pixelsPerMeter;

	wayPointDistanceInMeters = b2Distance(destinationCoordsInMeters, worldPositionInMeters);
	if (wayPointDistanceInMeters < 0.5)
	{
		getWaypointDestination();
	}

	currentVelocity = (destinationCoordsInMeters - worldPositionInMeters);
	currentVelocity.Normalize();
	currentVelocity *= static_cast<float>(currentSpeed);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Increase a utility action score - modify can be positive or negative value
void paraAI::modifyScore(int whichScore, int modifyAmount)
//-----------------------------------------------------------------------------------------------------------------------
{
	if ((whichScore < 0) || (whichScore > AI_MODE_NUMBER))
		return;

	ai[whichScore] += modifyAmount;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the acceleration for this droid
void paraAI::setAcceleration(double newAcceleration)
//-----------------------------------------------------------------------------------------------------------------------
{
	acceleration = newAcceleration;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the current acceleration for this droid
double paraAI::getAcceleration()
//-----------------------------------------------------------------------------------------------------------------------
{
	return acceleration;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the current maxSpeed for this droid
double paraAI::getMaxSpeed()
//-----------------------------------------------------------------------------------------------------------------------
{
	return maxSpeed;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the current velocity vector
b2Vec2 paraAI::getVelocity()
//-----------------------------------------------------------------------------------------------------------------------
{
	return currentVelocity;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the top speed for this droid
void paraAI::setMaxSpeed(double newMaxSpeed)
//-----------------------------------------------------------------------------------------------------------------------
{
	maxSpeed = newMaxSpeed;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the waypoint index
void paraAI::setWaypointIndex(int newIndex)
//-----------------------------------------------------------------------------------------------------------------------
{
	wayPointIndex = newIndex;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Get the current mode
int paraAI::getCurrentMode()
//-----------------------------------------------------------------------------------------------------------------------
{
	return currentAIMode;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Get the next destination coords from the index
void paraAI::getWaypointDestination()
//-----------------------------------------------------------------------------------------------------------------------
{
	destination.x = shipdecks.at(gam_getCurrentDeckName()).wayPoints[wayPointIndex].x;
	destination.y = shipdecks.at(gam_getCurrentDeckName()).wayPoints[wayPointIndex].y;

	switch (wayPointDirection)
	{
		case WAYPOINT_DOWN:
			wayPointIndex--;
			if (wayPointIndex < 0)
				wayPointIndex = static_cast<int>(shipdecks.at(gam_getCurrentDeckName()).wayPoints.size() - 1);
			break;

		case WAYPOINT_UP:
			wayPointIndex++;
			if (wayPointIndex > static_cast<int>(shipdecks.at(gam_getCurrentDeckName()).wayPoints.size() - 1))
				wayPointIndex = 0;
			break;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Change to a new mode - run functions required before running this action
void paraAI::changeModeTo(int newAIMode)
//-----------------------------------------------------------------------------------------------------------------------
{
	currentAIMode = newAIMode;

	switch (newAIMode)
	{
		case AI_MODE_PATROL:
			getWaypointDestination();
			break;

		case AI_MODE_ATTACK:
			break;

		case AI_MODE_FLEE:
			break;

		case AI_MODE_HEAL:
			break;

		case AI_MODE_HELP:
			break;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Check to see if the current mode needs to change
void paraAI::checkScores()
//-----------------------------------------------------------------------------------------------------------------------
{
	int highestScore = 0;
	int scoreMemory = AI_MODE_NUMBER;

	for (auto i = 0; i != AI_MODE_NUMBER; i++)
	{
		if (ai[i] > highestScore)
		{
			highestScore = ai[i];
			scoreMemory = i;
		}
	}
	if (scoreMemory == currentAIMode)     // already doing highest score action
		return;
	//
	// Change to new mode
	changeModeTo(scoreMemory);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Process the current AI mode - Pass in the current world position in meters
void paraAI::process(b2Vec2 newWorldPosInMeters)
//-----------------------------------------------------------------------------------------------------------------------
{
	worldPositionInMeters = newWorldPosInMeters;

	checkScores();

	switch (currentAIMode)
	{
		case AI_MODE_PATROL:
			patrol();
			break;

		case AI_MODE_ATTACK:
			break;

		case AI_MODE_FLEE:
			break;

		case AI_MODE_HEAL:
			break;

		case AI_MODE_HELP:
			break;
	}
}