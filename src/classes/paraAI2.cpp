#include <system/util.h>
#include <classes/paraRandom.h>
#include <game/player.h>
#include <SDL2_gfxPrimitives.h>
#include <classes/paraLOS.h>
#include <game/pathFind.h>
#include <game/database.h>
#include <game/bullet.h>
#include "classes/paraAI2.h"

#define DEBUG_AI2 1
#define NO_THREAD 1

#define DESTINATION_ARRIVE_DISTANCE 0.5
#define DESTINATION_SLOWDOWN_DISTANCE 1.5
#define DESTINATION_SLOWDOWN_FACTOR 0.2

#define COLLIDE_WITH_PLAYER -1
#define COLLIDE_WITH_NOTHING -2

SDL_Thread *sdlThreadID;

//-----------------------------------------------------------------------------------------------------------------------
//
// Render the destination vector
void paraAI2::debugShowDestination ()
//-----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2 drawStart;
	b2Vec2 drawFinish;

	drawStart  = sys_convertMetersToPixels (worldPositionInMeters);
	drawFinish = sys_convertMetersToPixels (destinationCoordsInMeters);

	drawStart  = sys_worldToScreen (drawStart, 100);
	drawFinish = sys_worldToScreen (drawFinish, 100);

	thickLineRGBA (renderer.renderer, drawStart.x, drawStart.y, drawFinish.x, drawFinish.y, 2, 0, 200, 200, 255);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Show the current AI array values
void paraAI2::debugShowValues ()
//-----------------------------------------------------------------------------------------------------------------------
{
	int counter = 0;

	con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("%s %s %s %s %s", "        PTL", "   HNT", "   FLE", "   HEL", "   ATK"));

	for (auto droidItr : g_shipDeckItr->second.droid)
	{
		if (droidItr.getCurrentMode () == DROID_MODE_NORMAL)
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("%i   --  %2i --   %2i --   %2i --  %2i  --  %2i  -- %s - ", counter, droidItr.ai2.ai[0], droidItr.ai2.ai[1], droidItr.ai2.ai[2], droidItr.ai2.ai[3], droidItr.ai2.ai[4], getAIActionString (droidItr.ai2.getCurrentAIMode ()).c_str ()));
		else
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("%i %s", counter, "Dead"));

		counter++;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Get the current mode
int paraAI2::getCurrentAIMode ()
//-----------------------------------------------------------------------------------------------------------------------
{
	return currentAIMode;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Get a string from the current AI Mode
std::string paraAI2::getAIActionString (int whichMode)
//-----------------------------------------------------------------------------------------------------------------------
{
	switch (whichMode)
	{
		case AI2_MODE_PATROL:
			return "AI2_MODE_PATROL";
			break;

		case AI2_MODE_HUNT:
			return "AI2_MODE_HUNT";
			break;

		case AI2_MODE_FLEE:
			return "AI2_MODE_FLEE";
			break;

		case AI2_MODE_HEAL:
			return "AI2_MODE_HEAL";
			break;

		case AI2_MODE_ATTACK:
			return "AI2_MODE_ATTACK";
			break;
	}

	return (sys_getString ("Unknown AI_MODE - mode [ %i ]", whichMode));
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Start attacking the target
void paraAI2::attack ()
//-----------------------------------------------------------------------------------------------------------------------
{
	if (targetDroid == NO_ATTACK_TARGET)
		return;

	//
	// Stop having a target if the droid is not in a normal state
	if (targetDroid != -1)
	{
		if (g_shipDeckItr->second.droid[targetDroid].getCurrentMode () != DROID_MODE_NORMAL)
			targetDroid = NO_ATTACK_TARGET;
	}

	if ((currentAttackDistance > desiredAttackDistance2) && (currentAttackDistance < desiredAttackDistance2 + paddingSize2))
	{
		if (dataBaseEntry[g_shipDeckItr->second.droid[arrayIndex].getDroidType ()].canShoot)
		{
			if (targetDroid != NO_ATTACK_TARGET)
			{
				if (g_shipDeckItr->second.droid[arrayIndex].getWeaponCanFire ())
					gam_addBullet (arrayIndex);
			}
		}
		return;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Find the waypoint furthest away from the player
b2Vec2 paraAI2::findFleeTile ()
//-----------------------------------------------------------------------------------------------------------------------
{
	int   newWaypointIndex;
	float longestDistance = 1;
	float currentDistance = 0;

	for (newWaypointIndex = 0; newWaypointIndex != static_cast<int>(g_shipDeckItr->second.wayPoints.size () - 1); newWaypointIndex++)
	{
		if (sys_convertPixelsToMeters (playerDroid.getWorldPosInPixels()) != sys_convertPixelsToMeters (g_shipDeckItr->second.wayPoints[newWaypointIndex]))
		{
			currentDistance = b2Distance (sys_convertPixelsToMeters (playerDroid.getWorldPosInPixels()), sys_convertPixelsToMeters (g_shipDeckItr->second.wayPoints[newWaypointIndex]));
			if (currentDistance > longestDistance)
			{
				longestDistance     = currentDistance;
				patrolWayPointIndex = newWaypointIndex;
			}
		}
	}
	return sys_convertPixelsToTiles(g_shipDeckItr->second.wayPoints[patrolWayPointIndex]);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Find a healing tile - if none on this level then flee
b2Vec2 paraAI2::findHealingTile ()
//-----------------------------------------------------------------------------------------------------------------------
{
	float  shortestDistance = 1000;
	float  currentDistance;
	b2Vec2 healingDestination;

	if (g_shipDeckItr->second.healing.empty ())
	{
		healingDestination.x = -1;
		healingDestination.y = -1;
		return healingDestination;
	}

	for (auto healItr : g_shipDeckItr->second.healing)
	{
		currentDistance = b2Distance (worldPositionInMeters, healItr.worldPosInMeters);
		if (currentDistance > 2.0)      // Needs to be far enough away to create a aStar path
		{
			if (currentDistance < shortestDistance)
			{
				shortestDistance   = currentDistance;
				healingDestination = sys_convertPixelsToTiles (healItr.worldPosInPixels);
			}
		}
	}
	return healingDestination;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Find the first available waypoint that is not blocked by a wall or door
b2Vec2 paraAI2::findOpenWaypoint ()
//-----------------------------------------------------------------------------------------------------------------------
{
	RayCastWallHitCallback callback;
	b2Vec2                 invalidWaypoint = {-1, -1};

	for (int newWaypointIndex = 0; newWaypointIndex != static_cast<int>(g_shipDeckItr->second.wayPoints.size () - 1); newWaypointIndex++)
	{
		if (worldPositionInMeters != sys_convertPixelsToMeters (g_shipDeckItr->second.wayPoints[newWaypointIndex]))
		{
			if (patrolWayPointIndex != newWaypointIndex)    // Don't check current waypoint
			{
				sys_getPhysicsWorld ()->RayCast (&callback, worldPositionInMeters, sys_convertPixelsToMeters (g_shipDeckItr->second.wayPoints[newWaypointIndex]));
				if (!callback.m_hit)    // Found a waypoint not crossing a wall
				{
					switch (currentAIMode)
					{
						case AI2_MODE_PATROL:
							patrolWayPointIndex = newWaypointIndex;
							break;
					}
					return g_shipDeckItr->second.wayPoints[newWaypointIndex];
				}
			}
		}
	}

	printf ("Break - couldn't find an open waypoint.\n");

	return invalidWaypoint;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Find the nearest location taking into account solid walls
b2Vec2 paraAI2::findLocationWithLOS (int locationType)
//-----------------------------------------------------------------------------------------------------------------------
{
	RayCastAnyCallback callback;
	int                newWaypointIndex;
	int                shortestDistance;
	int                currentDistance;

#ifdef DEBUG_AI2
	std::cout << "[ " << arrayIndex << " ] - Search for a visible waypoint." << std::endl;
#endif

	switch (locationType)
	{
		case AI2_FIND_WAYPOINT:
			//
			// Find nearest waypoint that isn't blocked by a wall
			destinationCoordsInMeters.x = -1;
			shortestDistance = 1000;

			for (newWaypointIndex = 0; newWaypointIndex != static_cast<int>(g_shipDeckItr->second.wayPoints.size () - 1); newWaypointIndex++)
			{
				if (worldPositionInMeters != sys_convertPixelsToMeters (g_shipDeckItr->second.wayPoints[newWaypointIndex]))
				{
					sys_getPhysicsWorld ()->RayCast (&callback, worldPositionInMeters, sys_convertPixelsToMeters (g_shipDeckItr->second.wayPoints[newWaypointIndex]));
					if (!callback.m_hit)
					{
						currentDistance = b2Distance (worldPositionInMeters, sys_convertPixelsToMeters (g_shipDeckItr->second.wayPoints[newWaypointIndex]));
						if (currentDistance < shortestDistance)
						{
							shortestDistance          = currentDistance;
							destinationCoordsInMeters = sys_convertPixelsToMeters (g_shipDeckItr->second.wayPoints[newWaypointIndex]);
							switch (currentAIMode)
							{
								case AI2_MODE_PATROL:
									patrolWayPointIndex = newWaypointIndex;
									break;
							}
#ifdef DEBUG_AI2
							std::cout << "[ " << arrayIndex << " ] - Found a closer waypoint [ " << patrolWayPointIndex << " ] [" << shortestDistance << " ] " << std::endl;
#endif
						}
					}
				}
			}
			return destinationCoordsInMeters;
			break;
	}
	return b2Vec2{0, 0};
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Sanity check that droid is not attempting to get to a destination behind a wall
// Return false if there is a wall between droid and destination
bool paraAI2::isDestinationValid ()
//-----------------------------------------------------------------------------------------------------------------------
{
	RayCastWallHitCallback rayCastCallback;

	if ((destinationCoordsInMeters.x <= 0) || (destinationCoordsInMeters.y <= 0))
		return false;

	if (worldPositionInMeters != destinationCoordsInMeters)
	{
		sys_getPhysicsWorld ()->RayCast (&rayCastCallback, worldPositionInMeters, destinationCoordsInMeters);
		if (rayCastCallback.m_hit)
			return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Insert a value for each action score
void paraAI2::initAI ()
//-----------------------------------------------------------------------------------------------------------------------
{
	paraRandom randomDirection;
	int        whichDirection;

	for (auto i = 0; i != AI_MODE_NUMBER; i++)
	{
		ai[i] = 0;
	}
	currentAIMode = AI2_MODE_PATROL;     // Will set to AI_MODE_PATROL when checking scores first run
	ai[AI2_MODE_PATROL] = 50;
	currentSpeed = 0;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Get the next destination coordinates
void paraAI2::getNextDestination ()
//-----------------------------------------------------------------------------------------------------------------------
{
	previousDestinationInMeters = destinationCoordsInMeters;

	switch (currentAIMode)
	{
		case AI2_MODE_PATROL:
			switch (patrolWaypointDirection)
			{
				case PATROL_WAYPOINT_DIRECTION_DOWN:
					patrolWayPointIndex--;
					if (patrolWayPointIndex < 0)
						patrolWayPointIndex = static_cast<int>(g_shipDeckItr->second.wayPoints.size () - 1);
					break;

				case PATROL_WAYPOINT_DIRECTION_UP:
					patrolWayPointIndex++;
					if (patrolWayPointIndex > static_cast<int>(g_shipDeckItr->second.wayPoints.size () - 1))
						patrolWayPointIndex = 0;
					break;
			}
			destinationCoordsInMeters = sys_convertPixelsToMeters (g_shipDeckItr->second.wayPoints[patrolWayPointIndex]);
			break;

		case AI2_MODE_HEAL:
		case AI2_MODE_FLEE:
			//
			// Make sure thread has stopped running and the waypoints are ready to use
			if ((aStar.stillRunning ()) || (!aStar.areWaypointsReady ()))
				return;

			destinationCoordsInMeters = aStar.getWaypoint (patrolWaypointDirection);

			if (destinationCoordsInMeters.x == PATROL_WAYPOINT_DIRECTION_UP)  // Reached last waypoint - destination
			{
				reachedDestination ();
				return;
			}

			if (destinationCoordsInMeters.x == PATROL_WAYPOINT_DIRECTION_DOWN)  // Reached start of waypoint array
			{
				patrolWaypointDirection   = PATROL_WAYPOINT_DIRECTION_UP;
				destinationCoordsInMeters = aStar.getWaypoint (patrolWaypointDirection);
			}

			destinationCoordsInMeters.x += tileSize / 2;
			destinationCoordsInMeters.y += tileSize / 2;

			destinationCoordsInMeters = sys_convertPixelsToMeters (destinationCoordsInMeters);
			break;
	}

	//
	// See if the droid can get to it's destination
	//
	if (!isDestinationValid ())
	{
		destinationCoordsInMeters = sys_convertPixelsToMeters (findOpenWaypoint ());
		if ((destinationCoordsInMeters.x < 0) || (destinationCoordsInMeters.y < 0))
		{
			// TODO: Do something clever here
			printf ("Got invalid coordinates back\n\n");
		}

#ifdef DEBUG_AI2
		printf ("Droid stuck behind wall - got new coords [ %3.3f %3.3f ]\n", destinationCoordsInMeters.x, destinationCoordsInMeters.y);
#endif
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Process the movement to the destination coordinates
void paraAI2::doMovement (b2Vec2 newWorldPosInMeters)
//-----------------------------------------------------------------------------------------------------------------------
{
	int potentialCollideResult = COLLIDE_WITH_NOTHING;

	worldPositionInMeters = newWorldPosInMeters;

	switch (currentAIMode)
	{
		case AI2_MODE_ATTACK:
			if (targetDroid == -1)      // Get distance and direction to the player
			{
				currentAttackDistance = b2Distance (sys_convertPixelsToMeters (playerDroid.getWorldPosInPixels()), sys_convertPixelsToMeters (g_shipDeckItr->second.droid[arrayIndex].getWorldPosInPixels()));
				directionAttackVector = sys_convertPixelsToMeters (playerDroid.getWorldPosInPixels()) - sys_convertPixelsToMeters (g_shipDeckItr->second.droid[arrayIndex].getWorldPosInPixels());
			}
			else                        // Get distance and direction to the other droid
			{
				currentAttackDistance = b2Distance (sys_convertPixelsToMeters (g_shipDeckItr->second.droid[targetDroid].getWorldPosInPixels()), sys_convertPixelsToMeters (g_shipDeckItr->second.droid[arrayIndex].getWorldPosInPixels()));
				directionAttackVector = sys_convertPixelsToMeters (g_shipDeckItr->second.droid[targetDroid].getWorldPosInPixels()) - sys_convertPixelsToMeters (g_shipDeckItr->second.droid[arrayIndex].getWorldPosInPixels());
			}
			break;

		default:
			if (b2Distance (destinationCoordsInMeters, worldPositionInMeters) < DESTINATION_ARRIVE_DISTANCE)    // Conditional jump or move on uninitialised value
				getNextDestination ();

			//
			// Action to take to avoid a potential collision
			//
			potentialCollideResult = checkPotentialCollision ();
			if (potentialCollideResult == COLLIDE_WITH_PLAYER)      // Switch direction to avoid hitting player
			{
				currentSpeed    = maxSpeed * 0.5; //0.0;
				currentVelocity = {0.0, 0.0};
				switchWaypointDirection ();
			}
			else if (potentialCollideResult != COLLIDE_WITH_NOTHING)    // Switch direction if going to hit another droid
			{
				currentSpeed    = maxSpeed * 0.5;
				currentVelocity = {0.0, 0.0};
				if (potentialCollideResult > g_shipDeckItr->second.droid[arrayIndex].getDroidType ())
				{
					switchWaypointDirection ();
				}
			}

			processVelocity ();
			break;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the waypoint index
void paraAI2::setWaypointIndex (int newIndex)
//-----------------------------------------------------------------------------------------------------------------------
{
	patrolWayPointIndex = newIndex;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Check if droid is heading for a collision with player or another droid
// Return TRUE if going to collide
int paraAI2::checkPotentialCollision ()
//-----------------------------------------------------------------------------------------------------------------------
{
	lookAheadVelocity *= LOOK_AHEAD_DISTANCE;
	//
	// Change direction if going to run into player
	droidFixture = playerDroid.body->GetFixtureList ();    // Only have one fixture per body

	if (droidFixture->TestPoint (lookAheadVelocity + worldPositionInMeters))
		return COLLIDE_WITH_PLAYER;

	//
	// Change direction if going to run into another droid
	for (auto &droidItr : g_shipDeckItr->second.droid)
	{
		if (droidItr.getIndex() != arrayIndex)       // don't check self
		{
			if (droidItr.body != nullptr)       // still got a valid physics body
			{
				droidFixture = droidItr.body->GetFixtureList (); // Only have one fixture
				if (droidFixture->TestPoint (lookAheadVelocity + worldPositionInMeters))
					return droidItr.getDroidType ();
			}
		}
	}
	return COLLIDE_WITH_NOTHING;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Swap the direction of travel for PATROL using waypoints
void paraAI2::switchWaypointDirection ()
//-----------------------------------------------------------------------------------------------------------------------
{
	switch (currentAIMode)
	{
		case AI2_MODE_PATROL:
		case AI2_MODE_HEAL:
		case AI2_MODE_FLEE:
			if (patrolWaypointDirection == PATROL_WAYPOINT_DIRECTION_DOWN)
				patrolWaypointDirection = PATROL_WAYPOINT_DIRECTION_UP;
			else
				patrolWaypointDirection = PATROL_WAYPOINT_DIRECTION_DOWN;
			break;
	}

	getNextDestination ();
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the patrol waypoint direction
void paraAI2::setWaypointDirection (int newDirection)
//-----------------------------------------------------------------------------------------------------------------------
{
	patrolWaypointDirection = newDirection;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Do any acceleration and apply current speed to velocity ( direction ) vector
// Update lookAheadVelocity vector with new direction vector
void paraAI2::processVelocity ()
//-----------------------------------------------------------------------------------------------------------------------
{
	//
	// Reduce speed as approaching destination
	//
	if (b2Distance (destinationCoordsInMeters, worldPositionInMeters) < DESTINATION_SLOWDOWN_DISTANCE)  // Conditional jump on unitialised value
	{
		//
		// Only slow down if not going full speed
		if (currentSpeed >= (maxSpeed * 0.75))
		{
			currentSpeed -= gravity;
			if (currentSpeed < maxSpeed * DESTINATION_SLOWDOWN_FACTOR)
				currentSpeed = maxSpeed * DESTINATION_SLOWDOWN_FACTOR;
		}
	}
	else
	{
		currentSpeed += acceleration;
		if (currentSpeed > maxSpeed)
			currentSpeed = maxSpeed;
	}

	currentVelocity = (destinationCoordsInMeters - worldPositionInMeters);
	currentVelocity.Normalize ();
	lookAheadVelocity = currentVelocity;
	currentVelocity *= static_cast<float>(currentSpeed);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the current velocity vector
b2Vec2 paraAI2::getVelocity ()
//-----------------------------------------------------------------------------------------------------------------------
{
	return currentVelocity;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the acceleration for this droid
void paraAI2::setAcceleration (double newAcceleration)
//-----------------------------------------------------------------------------------------------------------------------
{
	acceleration = newAcceleration;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the top speed for this droid
void paraAI2::setMaxSpeed (double newMaxSpeed)
//-----------------------------------------------------------------------------------------------------------------------
{
	maxSpeed = newMaxSpeed;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the destination location - pass in destination in pixel ( world ) coordinates
void paraAI2::setDestinationInPixels (b2Vec2 newDestination)
//-----------------------------------------------------------------------------------------------------------------------
{
	destinationCoordsInMeters = sys_convertPixelsToMeters (newDestination);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the destination location - pass in physics coordinates ( meters )
void paraAI2::setDestinationInMeters (b2Vec2 newDestination)
//-----------------------------------------------------------------------------------------------------------------------
{
	destinationCoordsInMeters = newDestination;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the index into the vector array for this droid
void paraAI2::setArrayIndex (int newIndex)
//-----------------------------------------------------------------------------------------------------------------------
{
	arrayIndex = newIndex;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the current target
int paraAI2::getTargetDroid ()
//-----------------------------------------------------------------------------------------------------------------------
{
	return targetDroid;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the current acceleration for this droid
double paraAI2::getAcceleration ()
//-----------------------------------------------------------------------------------------------------------------------
{
	return acceleration;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the current maxSpeed for this droid
double paraAI2::getMaxSpeed ()
//-----------------------------------------------------------------------------------------------------------------------
{
	return maxSpeed;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Someone shot this droid - set them as the target for an attack
void paraAI2::setTargetDroid (int newTargetDroid)
//-----------------------------------------------------------------------------------------------------------------------
{
	targetDroid = newTargetDroid;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the index into the vector array for this droid
int paraAI2::getArrayIndex ()
//-----------------------------------------------------------------------------------------------------------------------
{
	return arrayIndex;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Increase a utility action score - modify can be positive or negative value
void paraAI2::modifyAIScore (int whichScore, int modifyAmount)
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
// Check to see if the current mode needs to change
void paraAI2::checkAIScores ()
//-----------------------------------------------------------------------------------------------------------------------
{
	int highestScore = 0;
	int newAIAction  = 0;

	//
	// Prioritise healing above others
	if (ai[AI2_MODE_HEAL] >= 90)
	{
		modifyAIScore (AI2_MODE_PATROL, -70);
		modifyAIScore (AI2_MODE_FLEE, -70);
		modifyAIScore (AI2_MODE_HUNT, -70);
		modifyAIScore (AI2_MODE_ATTACK, -70);

		if (currentAIMode != AI2_MODE_HEAL)
			changeAIModeTo (AI2_MODE_HEAL);
		return;
	}
	//
	// Check for other actions to take
	for (auto i = 0; i != AI2_MODE_NUMBER; i++)
	{
		if (ai[i] > highestScore)
		{
			highestScore = ai[i];
			newAIAction  = i;
		}
	}
	//
	// Already doing this action
	if (currentAIMode == newAIAction)
		return;

	changeAIModeTo (newAIAction);

#ifdef DEBUG_AI2
	std::cout << "AI Score [ " << ai[0] << " " << ai[1] << " " << ai[2] << " " << ai[3] << " " << ai[4] << " ]" << std::endl;
#endif
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the current percentage of health left
void paraAI2::setHealValue (float newHealthPercent)
//-----------------------------------------------------------------------------------------------------------------------
{
	ai[AI2_MODE_HEAL] = 100 - newHealthPercent;

#ifdef DEBUG_AI
	//	std::cout << "[ " << arrayIndex << " ] Health percent is now : " << healthPercent << std::endl;
#endif
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Change to a new mode - run functions required before running the new AI action
void paraAI2::changeAIModeTo (int newAIMode)
//-----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2 tileDestination;
	b2Vec2 localWorldPosInPixels;
	int    returnCode;

	std::thread aStarThread2;


	if (aStar.stillRunning ())
		return;     // Wait for thread to finish before changing mode

	aStar.stopUsingPath ();

	previousAIMode = currentAIMode;
	currentAIMode  = newAIMode;

#ifdef DEBUG_AI2
	std::cout << "[ " << arrayIndex << " ]" << " Changing to new AI mode : " << getAIActionString (currentAIMode) << " : " << std::endl;
#endif

	switch (currentAIMode)
	{

		//-------------------------- AI_MODE_PATROL ---------------------------

		case AI2_MODE_PATROL:    // Find the nearest clear waypoint and move to it

#ifdef DEBUG_AI2
			std::cout << "[ " << arrayIndex << " ]" << " Changing to PATROL mode. Looking for nearest waypoint to move to." << std::endl;
#endif

			destinationCoordsInMeters = sys_convertPixelsToMeters (findOpenWaypoint ());
			if ((destinationCoordsInMeters.x < 0) || (destinationCoordsInMeters.y < 0))
			{
				//
				// TODO: Something clever
				printf ("paraAI2:: Couldn't find a nearest waypoint to set patrol to.\n");
				return;
			}
			break;

			//-------------------------- AI_MODE_HEAL ---------------------------

		case AI2_MODE_HEAL:

#ifdef DEBUG_AI2
			std::cout << "[ " << arrayIndex << " ]" << " Changing to HEAL mode. Requesting aStar to healing tile." << std::endl;
#endif
			tileDestination = findHealingTile ();
			if (tileDestination.x < 0)                // No healing tiles on level - change to flee
			{
				modifyAIScore (AI2_MODE_HEAL, -100);
				modifyAIScore (AI2_MODE_FLEE, 40);
				return;
			}

#ifdef DEBUG_AI2
			std::cout << "[ " << arrayIndex << " ]" << " Found the destination healing tile." << std::endl;
#endif
			localWorldPosInPixels = sys_convertMetersToPixels (worldPositionInMeters);

			aStar.setID (arrayIndex);

			aStar.requestNewPath (sys_convertPixelsToTiles (localWorldPosInPixels), tileDestination);

#ifdef NO_THREAD
			aStar.searchThread ();
#else
			sdlThreadID = SDL_CreateThread (aStar.startThread, "healThread", &aStar);
			SDL_DetachThread(sdlThreadID);
#endif

#ifdef DEBUG_AI2
			returnCode = aStar.getPathStatus ();
			std::cout << "[ " << arrayIndex << " ]" << " aStar return code is ." << "[ " << returnCode << " ]" << std::endl;
#endif

			if (ASTAR_PATH_TOO_SHORT == returnCode) // TODO: Move straight to healing tile if not blocked by wall ??
			{
				modifyAIScore (AI2_MODE_HEAL, -100);
				modifyAIScore (AI2_MODE_FLEE, 40);
				modifyAIScore (AI2_MODE_PATROL, -70);
			}

			patrolWaypointDirection = PATROL_WAYPOINT_DIRECTION_UP;
			patrolWayPointIndex     = 0;

			break;

			//-------------------------- AI_MODE_FLEE ---------------------------

		case AI2_MODE_FLEE:
#ifdef DEBUG_AI2
			std::cout << "[ " << arrayIndex << " ]" << " Changing to FLEE mode. Requesting aStar to flee tile." << std::endl;
#endif
			tileDestination = findFleeTile ();
			if (tileDestination.x < 0)                // Couldn't find a suitable tile - change to PATROL
			{
				modifyAIScore (AI2_MODE_FLEE, -100);
				modifyAIScore (AI2_MODE_PATROL, 40);
				return;
			}

#ifdef DEBUG_AI2
			std::cout << "[ " << arrayIndex << " ]" << " Found the destination flee tile." << std::endl;
#endif
			localWorldPosInPixels = sys_convertMetersToPixels (worldPositionInMeters);

			aStar.setID (arrayIndex);

			aStar.requestNewPath (sys_convertPixelsToTiles (localWorldPosInPixels), tileDestination);

#ifdef NO_THREAD
			aStar.searchThread ();
#else
			sdlThreadID = SDL_CreateThread (aStar.startThread, "fleeThread", &aStar);
			SDL_DetachThread(sdlThreadID);
#endif

#ifdef DEBUG_AI2
			returnCode = aStar.getPathStatus ();
			std::cout << "[ " << arrayIndex << " ]" << " aStar return code is ." << "[ " << returnCode << " ]" << std::endl;
#endif
			patrolWaypointDirection = PATROL_WAYPOINT_DIRECTION_UP;
			patrolWayPointIndex     = 0;
			break;

			//-------------------------- AI_MODE_ATTACK ---------------------------

		case AI2_MODE_ATTACK:
			if (!dataBaseEntry[g_shipDeckItr->second.droid[arrayIndex].getDroidType ()].canShoot)
			{
				modifyAIScore (AI2_MODE_ATTACK, -100);
				modifyAIScore (AI2_MODE_FLEE, 40);
			}
			break;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Reached the destination waypoint
void paraAI2::reachedDestination ()
//-----------------------------------------------------------------------------------------------------------------------
{

	switch (currentAIMode)
	{
		case AI2_MODE_HEAL:
			modifyAIScore (AI2_MODE_HEAL, -100);
			modifyAIScore (AI2_MODE_PATROL, +50);
			//
			// Fully heal

			g_shipDeckItr->second.droid[arrayIndex].setCurrentHealth (dataBaseEntry[g_shipDeckItr->second.droid[arrayIndex].getDroidType ()].maxHealth);    // Need another way of doing this
			gam_setHealthAnimation (arrayIndex);

			break;

		case AI2_MODE_FLEE:
			modifyAIScore (AI2_MODE_FLEE, -90);
			modifyAIScore (AI2_MODE_PATROL, +70);
			break;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Process the current AI mode - Pass in the current world position in meters
void paraAI2::process (b2Vec2 newWorldPosInMeters)
//-----------------------------------------------------------------------------------------------------------------------
{
	worldPositionInMeters = newWorldPosInMeters;

	checkAIScores ();

	switch (currentAIMode)
	{
		case AI_MODE_PATROL:
//			patrol ();
			break;

		case AI_MODE_ATTACK:
			attack ();
			break;

		case AI_MODE_FLEE:
//			flee ();
			break;

		case AI_MODE_HEAL:
//			heal ();
			break;

		case AI_MODE_HUNT:
//			hunt ();
			break;

		default:
			printf ("ERROR: Process invalid AI mode [ %i ]\n", currentAIMode);
			break;
	}
}