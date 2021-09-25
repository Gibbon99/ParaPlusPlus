#include <SDL2_gfxPrimitives.h>
#include <thread>
#include "classes/paraRandom.h"
#include "classes/paraLOS.h"
#include "classes/paraAI2.h"
#include "system/util.h"
#include "game/player.h"
#include "game/pathFind.h"
#include "game/database.h"
#include "classes/paraBullet.h"

#define DEBUG_AI2 1
#define NO_THREAD 1

#define DESTINATION_ARRIVE_DISTANCE 0.35
#define DESTINATION_SLOWDOWN_DISTANCE 1.7
#define DESTINATION_SLOWDOWN_FACTOR 0.4

#define COLLIDE_WITH_PLAYER -1
#define COLLIDE_WITH_NOTHING -2

float LOOK_AHEAD_DISTANCE {2.5};

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

	for (auto droidItr: g_shipDeckItr->second.droid)
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
			return "PATROL";
			break;

		case AI2_MODE_HUNT:
			return "HUNT";
			break;

		case AI2_MODE_FLEE:
			return "FLEE";
			break;

		case AI2_MODE_HEAL:
			return "HEAL";
			break;

		case AI2_MODE_ATTACK:
			return "ATTACK";
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
	DBGVAR(std::cout, arrayIndex);

//	std::cout << sys_getString ("Index %i Function : %s", arrayIndex, __func__) << std::endl;

	if (targetDroid == NO_ATTACK_TARGET)
	{
		modifyAIScore (AI2_MODE_ATTACK, -50 + difficultyValue);
		return;
	}
	//
	// Check if target is still visible or not - either player or other droid
	checkAttackVisibility ();
	//
	// Stop having a target if the droid is not in a normal state
	if (targetDroid != TARGET_PLAYER)
	{
		if (g_shipDeckItr->second.droid[targetDroid].getCurrentMode () != DROID_MODE_NORMAL)
		{
			targetDroid = NO_ATTACK_TARGET;
			return;
		}
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
// Run the hunt action
void paraAI2::hunt ()
//-----------------------------------------------------------------------------------------------------------------------
{
	//
	// Add counter to slowly reduce hunt value over time
	huntCountdownValue -= 1.0f * huntCountdownModifier;
	if (huntCountdownValue < 0.0f)
	{
		huntCountdownValue = 1.0f;
		modifyAIScore (AI2_MODE_HUNT, -10);
	}

	//
	// See if we are now in attack range of the player
	if ((currentAttackDistance > desiredAttackDistance2) && (currentAttackDistance < desiredAttackDistance2 + paddingSize2))
	{
#ifdef DEBUG_AI2
		std::cout << "[ " << arrayIndex << " ]" << " - Within attack range of player while in HUNT mode." << std::endl;
#endif

		modifyAIScore (AI2_MODE_HUNT, -60);
		modifyAIScore (AI2_MODE_ATTACK, 60 + difficultyValue);
		return;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Find the waypoint furthest away from the player
b2Vec2 paraAI2::findFleeTile ()
//-----------------------------------------------------------------------------------------------------------------------
{
	int   newWaypointIndex {};
	float longestDistance = 1;
	float currentDistance = 0;

	for (newWaypointIndex = 0; newWaypointIndex != static_cast<int>(g_shipDeckItr->second.wayPoints.size () - 1); newWaypointIndex++)
	{
		if (sys_convertPixelsToMeters (playerDroid.getWorldPosInPixels ()) != sys_convertPixelsToMeters (g_shipDeckItr->second.wayPoints[newWaypointIndex]))
		{
			currentDistance = b2Distance (sys_convertPixelsToMeters (playerDroid.getWorldPosInPixels ()), sys_convertPixelsToMeters (g_shipDeckItr->second.wayPoints[newWaypointIndex]));
			if (currentDistance > longestDistance)
			{
				longestDistance     = currentDistance;
				patrolWayPointIndex = newWaypointIndex;
			}
		}
	}
	return sys_convertPixelsToTiles (g_shipDeckItr->second.wayPoints[patrolWayPointIndex]);
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

	for (auto healItr: g_shipDeckItr->second.healing)
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
	b2Vec2                 invalidWaypoint {-1, -1};

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
b2Vec2 paraAI2::findLocationWithLOS (AI2_PATROL_ACTIONS locationType)
//-----------------------------------------------------------------------------------------------------------------------
{
	RayCastAnyCallback callback;
	b2Vec2             newDestinationCoordsInMeters {};
	int                newWaypointIndex {};
	int                shortestDistance {};
	int                currentDistance {};

#ifdef DEBUG_AI2
	std::cout << "[ " << arrayIndex << " ] - Search for a visible waypoint." << std::endl;
#endif

	switch (locationType)
	{
		case AI2_PATROL_ACTIONS::AI2_FIND_WAYPOINT:
			//
			// Find nearest waypoint that isn't blocked by a wall
			newDestinationCoordsInMeters.x = -1;
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
							if (shortestDistance > 0)
							{
								shortestDistance             = currentDistance;
								newDestinationCoordsInMeters = sys_convertPixelsToMeters (g_shipDeckItr->second.wayPoints[newWaypointIndex]);
								switch (currentAIMode)
								{
									case AI2_MODE_PATROL:
										patrolWayPointIndex = newWaypointIndex;
										break;
								}
							}
#ifdef DEBUG_AI2
							std::cout << "[ " << arrayIndex << " ] - Found a closer waypoint [ " << patrolWayPointIndex << " ] [" << shortestDistance << " ] " << std::endl;
#endif
						}
					}
				}
			}
			return newDestinationCoordsInMeters;
			break;
	}
	return b2Vec2 {0, 0};
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

	for (auto i = 0; i != AI2_MODE_NUMBER; i++)
	{
		ai[i] = 0;
	}
	currentAIMode = AI2_MODE_PATROL;     // Will set to AI_MODE_PATROL when checking scores first run
	ai[AI2_MODE_PATROL] = 50;
	currentSpeed = 0;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the current destinationCoordsInMeters variable for debugging
b2Vec2 paraAI2::debugGetDestinationCoordsInMeters ()
//-----------------------------------------------------------------------------------------------------------------------
{
	return destinationCoordsInMeters;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Get the next destination coordinates
void paraAI2::getNextDestination ()
//-----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2 nextDestinationInPixels {};

	DBGVAR(std::cout, arrayIndex);

	//std::cout << sys_getString ("Index %i Function : %s", arrayIndex, __func__) << std::endl;

	previousDestinationInMeters = destinationCoordsInMeters;

	switch (currentAIMode)
	{
		case AI2_MODE_PATROL:
			switch (patrolWaypointDirection)
			{
				case PATROL_WAYPOINT_DIRECTION::DOWN:
					patrolWayPointIndex--;
					if (patrolWayPointIndex < 0)
						patrolWayPointIndex = static_cast<int>(g_shipDeckItr->second.wayPoints.size () - 1);
					break;

				case PATROL_WAYPOINT_DIRECTION::UP:
					patrolWayPointIndex++;
					if (patrolWayPointIndex > static_cast<int>(g_shipDeckItr->second.wayPoints.size () - 1))
						patrolWayPointIndex = 0;
					break;
			}
			destinationCoordsInMeters = sys_convertPixelsToMeters (g_shipDeckItr->second.wayPoints[patrolWayPointIndex]);
			break;

		case AI2_MODE_HEAL:
		case AI2_MODE_FLEE:

#ifdef NO_THREAD
#else
			//
			// Make sure thread has stopped running and the waypoints are ready to use
			if ((aStar.stillRunning ()) || (!aStar.areWaypointsReady ()))
				return;
#endif
			switch (aStar.getAStarDirection ())
			{
				case ASTAR_DIRECTION::ERROR:
					break;

				case ASTAR_DIRECTION::DESTINATION:
					reachedDestination ();
					break;

				case ASTAR_DIRECTION::START:
					aStar.switchTravelDirection ();
					break;

				default:    // Move UP or DOWN array
					destinationCoordsInMeters = sys_convertPixelsToMeters (aStar.getWaypoint ());
					break;
			}
	}

	/*
	//
	// See if the droid can get to it's destination
	//
	if (!isDestinationValid ())
	{
#ifdef DEBUG_AI2
		printf ("[ %i ] - Invalid destination coords [ %3.3f %3.3f ]\n", arrayIndex, destinationCoordsInMeters.x, destinationCoordsInMeters.y);
#endif

		destinationCoordsInMeters = previousDestinationInMeters;

		destinationCoordsInMeters = sys_convertPixelsToMeters (findOpenWaypoint ());
		if ((destinationCoordsInMeters.x < 0) || (destinationCoordsInMeters.y < 0))
		{
			// TODO: Do something clever here
			printf ("Got invalid coordinates back\n\n");
		}


	}
	*/
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Process the movement to the destination coordinates
void paraAI2::doMovement (b2Vec2 newWorldPosInMeters)
//-----------------------------------------------------------------------------------------------------------------------
{
	DBGVAR(std::cout, arrayIndex);

//	std::cout << sys_getString ("Index %i Function : %s", arrayIndex, __func__) << std::endl;

//	int potentialCollideResult = COLLIDE_WITH_NOTHING;

	worldPositionInMeters = newWorldPosInMeters;

	if (destinationCoordsInMeters.x < 0)
	{
		printf ("Have an invalid destination\n");
		return;
	}

	switch (currentAIMode)
	{
		case AI2_MODE_HUNT: //TOD Add in thread aware here as well - like heal, flee
			currentAttackDistance = b2Distance (sys_convertPixelsToMeters (playerDroid.getWorldPosInPixels ()), worldPositionInMeters);
			break;

		case AI2_MODE_ATTACK:

			std::cout << sys_getString ("Index %i Function : %s Check targetDroid == -1", arrayIndex, __func__) << std::endl;

			if (targetDroid == -1)      // Get distance and direction to the player
			{
				currentAttackDistance = b2Distance (sys_convertPixelsToMeters (playerDroid.getWorldPosInPixels ()), worldPositionInMeters);
				directionAttackVector = sys_convertPixelsToMeters (playerDroid.getWorldPosInPixels ()) - worldPositionInMeters;
			}
			else if (targetDroid != NO_ATTACK_TARGET)                        // Get distance and direction to the other droid
			{

				std::cout << sys_getString ("Index %i Function : %s Check targetDroid != NO_ATTACK_TARGET", arrayIndex, __func__) << std::endl;

				currentAttackDistance = b2Distance (sys_convertPixelsToMeters (g_shipDeckItr->second.droid[targetDroid].getWorldPosInPixels ()), worldPositionInMeters);
				directionAttackVector = sys_convertPixelsToMeters (g_shipDeckItr->second.droid[targetDroid].getWorldPosInPixels ()) - worldPositionInMeters;
			}

			std::cout << sys_getString ("Index %i Function : %s currentAttackDistance > desiredAttackDistance2", arrayIndex, __func__) << std::endl;

			if (currentAttackDistance > desiredAttackDistance2)    // Droid is too far away - move closer
			{
				directionAttackVector.Normalize ();
				directionAttackVector *= currentSpeed;
				currentVelocity = directionAttackVector;
				return;
			}

			if (currentAttackDistance < desiredAttackDistance2)     // Too close - move away - but check that path is clear ?
			{

				std::cout << sys_getString ("Index %i Function : %s currentAttackDistance < desiredAttackDistance2", arrayIndex, __func__) << std::endl;

				directionAttackVector = -directionAttackVector;
				directionAttackVector.Normalize ();
				directionAttackVector *= currentSpeed;
				currentVelocity       = directionAttackVector;
				return;
			}

			break;

		case AI2_MODE_HEAL:
		case AI2_MODE_FLEE:
#ifdef NO_THREAD
			if (b2Distance (destinationCoordsInMeters, worldPositionInMeters) < DESTINATION_ARRIVE_DISTANCE)    // Conditional jump or move on uninitialised value
				getNextDestination ();

			//
			// Action to take to avoid a potential collision while following a aStar path
			//
			checkPotentialCollision ();
#else
			if (ASTAR_STATUS_READY == aStar.getPathStatus ())
			{
				if (b2Distance (destinationCoordsInMeters, worldPositionInMeters) < DESTINATION_ARRIVE_DISTANCE)    // Conditional jump or move on uninitialised value
					getNextDestination ();

				checkPotentialCollision ();
			}
#endif
			break;

		default:
			if (b2Distance (destinationCoordsInMeters, worldPositionInMeters) < DESTINATION_ARRIVE_DISTANCE)    // Conditional jump or move on uninitialised value
				getNextDestination ();

			checkPotentialCollision ();
			break;
	}
	processVelocity ();
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
	DBGVAR(std::cout, arrayIndex);

	//std::cout << sys_getString ("Index %i Function : %s", arrayIndex, __func__) << std::endl;

	lookAheadVelocity *= LOOK_AHEAD_DISTANCE + (g_shipDeckItr->second.droid[arrayIndex].getDroidType () / 10);
	//
	// Change direction if going to run into player
	droidFixture = playerDroid.body->GetFixtureList ();    // Only have one fixture per body

	if (droidFixture->TestPoint (lookAheadVelocity + worldPositionInMeters))    // Avoid player
	{
		currentSpeed    = maxSpeed * 0.5; // 0.0;
		currentVelocity = {0.0, 0.0};
		switchWaypointDirection ();
		return COLLIDE_WITH_PLAYER;
	}
	//
	// Change direction if going to run into another droid
	for (auto &droidItr: g_shipDeckItr->second.droid)
	{
		if (droidItr.getIndex () != arrayIndex)       // don't check self
		{
			if (droidItr.body != nullptr)       // still got a valid physics body
			{
				droidFixture = droidItr.body->GetFixtureList (); // Only have one fixture
				if (droidFixture->TestPoint (lookAheadVelocity + worldPositionInMeters))
				{
					currentSpeed             = maxSpeed * 0.5;
					currentVelocity          = {0.0, 0.0};
					if (swapDirectionCounter < IGNORE_SWAP_DIRECTION_LIMIT)
					{
						switchWaypointDirection ();
						swapDirectionCounter++;
					}
					else
						swapDirectionCounter = 0;

					return droidItr.getDroidType ();
				}

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
			if (patrolWaypointDirection == PATROL_WAYPOINT_DIRECTION::DOWN)
				patrolWaypointDirection = PATROL_WAYPOINT_DIRECTION::UP;
			else
				patrolWaypointDirection = PATROL_WAYPOINT_DIRECTION::DOWN;
			break;

		case AI2_MODE_HEAL:
		case AI2_MODE_FLEE:
			aStar.switchTravelDirection ();
			break;
	}

	getNextDestination ();
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the patrol waypoint direction
void paraAI2::setWaypointDirection (PATROL_WAYPOINT_DIRECTION newDirection)
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
	DBGVAR(std::cout, arrayIndex);

	//std::cout << sys_getString ("Index %i Function : %s", arrayIndex, __func__) << std::endl;

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
// Debug - Return current patrolWaypointIndex
int paraAI2::getPatrolWaypointIndex ()
//-----------------------------------------------------------------------------------------------------------------------
{
	return patrolWayPointIndex;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Increase a utility action score - modify can be positive or negative value
void paraAI2::modifyAIScore (int whichScore, int modifyAmount)
//-----------------------------------------------------------------------------------------------------------------------
{
	DBGVAR(std::cout, arrayIndex);

	//std::cout << sys_getString ("Index %i Function : %s", arrayIndex, __func__) << std::endl;

	if ((whichScore < 0) || (whichScore > AI2_MODE_PATROL))
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
	DBGVAR(std::cout, arrayIndex);

	//std::cout << sys_getString ("Index %i Function : %s", arrayIndex, __func__) << std::endl;

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
// Run common aStar functions
void paraAI2::runAStarCode (b2Vec2 destinationTile)
//-----------------------------------------------------------------------------------------------------------------------
{
	aStar.setID (arrayIndex);

	aStar.requestNewPath (sys_convertPixelsToTiles (sys_convertMetersToPixels (worldPositionInMeters)), destinationTile);

#ifdef NO_THREAD
	aStar.searchThread ();
#else
	aStar.startThread ();
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

	std::cout << sys_getString ("Index %i Function : %s", arrayIndex, __func__) << std::endl;

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
			std::cout << "[ " << arrayIndex << " ]" << " - Changing to PATROL mode. Looking for nearest waypoint to move to." << std::endl;
#endif

//			destinationCoordsInMeters = sys_convertPixelsToMeters (findOpenWaypoint ());
			destinationCoordsInMeters = (findLocationWithLOS (AI2_PATROL_ACTIONS::AI2_FIND_WAYPOINT));
			if ((destinationCoordsInMeters.x < 0) || (destinationCoordsInMeters.y < 0))
			{
				//
				// TODO: Something clever
				printf ("[ %i ] - paraAI2:: Couldn't find a nearest waypoint to set patrol to.\n", arrayIndex);

//				destinationCoordsInMeters = previousDestinationInMeters;

				modifyAIScore (AI2_MODE_PATROL, -60);
				modifyAIScore (AI2_MODE_FLEE, 80);
				changeAIModeTo (AI2_MODE_FLEE);
				return;
			}
			break;

			//-------------------------- AI_MODE_HEAL ---------------------------

		case AI2_MODE_HEAL:

#ifdef DEBUG_AI2
			std::cout << "[ " << arrayIndex << " ]" << " - Changing to HEAL mode. Requesting aStar to healing tile." << std::endl;
#endif
			tileDestination = findHealingTile ();
			if (tileDestination.x < 0)                // No healing tiles on level - change to flee
			{
				modifyAIScore (AI2_MODE_HEAL, -100);
				modifyAIScore (AI2_MODE_FLEE, 40);
				changeAIModeTo (AI2_MODE_FLEE);
				return;
			}

			runAStarCode (tileDestination);

#ifdef DEBUG_AI2
			std::cout << "[ " << arrayIndex << " ]" << " - Found the destination healing tile." << std::endl;
#endif

#ifdef NO_THREAD
			if (ASTAR_STATUS_TOO_SHORT == returnCode) // TODO: Move straight to healing tile if not blocked by wall ??
			{
				modifyAIScore (AI2_MODE_HEAL, -100);
				modifyAIScore (AI2_MODE_FLEE, 40);
				modifyAIScore (AI2_MODE_PATROL, -70);
			}
			destinationCoordsInMeters = sys_convertPixelsToMeters (aStar.getWaypoint ());
#endif
			break;

			//-------------------------- AI_MODE_FLEE ---------------------------

		case AI2_MODE_FLEE:
#ifdef DEBUG_AI2
			std::cout << "[ " << arrayIndex << " ]" << " - Changing to FLEE mode. Requesting aStar to flee tile." << std::endl;
#endif
			tileDestination = findFleeTile ();
			if (tileDestination.x < 0)                // Couldn't find a suitable tile - change to PATROL
			{
				modifyAIScore (AI2_MODE_FLEE, -100);
				modifyAIScore (AI2_MODE_PATROL, 40);
				return;
			}

#ifdef DEBUG_AI2
			std::cout << "[ " << arrayIndex << " ]" << " - Found the destination flee tile." << std::endl;
#endif

			runAStarCode (tileDestination);
#ifdef NO_THREAD
			if (ASTAR_STATUS_TOO_SHORT == returnCode)
			{
				modifyAIScore (AI2_MODE_FLEE, -100);
				modifyAIScore (AI2_MODE_PATROL, 40);
			}
			destinationCoordsInMeters = sys_convertPixelsToMeters (aStar.getWaypoint ());
#endif
			break;

			//-------------------------- AI_MODE_HUNT ---------------------------

		case AI2_MODE_HUNT:
#ifdef DEBUG_AI2
			std::cout << "[ " << arrayIndex << " ]" << " - Changing to HUNT mode." << std::endl;
#endif
			tileDestination = gam_getLastPlayerTrail ();
			if (tileDestination.x < 0)                // Couldn't get a tile from player trail
			{
				modifyAIScore (AI2_MODE_HUNT, -100);
				modifyAIScore (AI2_MODE_FLEE, 40);
				return;
			}

#ifdef DEBUG_AI2
			std::cout << "[ " << arrayIndex << " ]" << " - Found the player trail tile." << std::endl;
#endif
			runAStarCode (tileDestination);
#ifdef NO_THREAD
			if (ASTAR_STATUS_TOO_SHORT == returnCode)
			{
				modifyAIScore (AI2_MODE_HUNT, -100);
				modifyAIScore (AI2_MODE_ATTACK, -50 + difficultyValue);
				modifyAIScore (AI2_MODE_FLEE, 40);
				modifyAIScore (AI2_MODE_PATROL, 70);
			}
			destinationCoordsInMeters = sys_convertPixelsToMeters (aStar.getWaypoint ());
#endif
			huntCountdownValue = 1.0f;
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
// If droid can not see target for period of time - then stop attack and disregard target
// Decrease attack value and increase hunt value
//
void paraAI2::checkAttackVisibility ()
//-----------------------------------------------------------------------------------------------------------------------
{
	RayCastAnyCallback LOSDroidToDroidCallback;

	DBGVAR(std::cout, arrayIndex);

	//std::cout << sys_getString ("Index %i Function : %s", arrayIndex, __func__) << std::endl;

	if (targetDroid == TARGET_PLAYER)
	{
		if (!g_shipDeckItr->second.droid[arrayIndex].visibleToPlayer)
		{
			g_shipDeckItr->second.droid[arrayIndex].ai2.modifyAIScore (AI2_MODE_HUNT, 10);
			g_shipDeckItr->second.droid[arrayIndex].ai2.modifyAIScore (AI2_MODE_ATTACK, -10);
		}
		else    // Found player again while still in attack mode
		{
			g_shipDeckItr->second.droid[arrayIndex].ai2.modifyAIScore (AI2_MODE_HUNT, -30);
			g_shipDeckItr->second.droid[arrayIndex].ai2.modifyAIScore (AI2_MODE_ATTACK, 40 + difficultyValue);
		}
	}
	else if (targetDroid != NO_ATTACK_TARGET)   // Targeting another droid
	{
		sys_getPhysicsWorld ()->RayCast (&LOSDroidToDroidCallback, sys_convertPixelsToMeters (g_shipDeckItr->second.droid[targetDroid].getWorldPosInPixels ()), worldPositionInMeters);

		if (LOSDroidToDroidCallback.m_hit)
		{
			//
			// Two droids can't see each other
			g_shipDeckItr->second.droid[arrayIndex].ai2.modifyAIScore (AI2_MODE_ATTACK, -100);
			targetDroid = NO_ATTACK_TARGET;
		}
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Process the current AI mode - Pass in the current world position in meters
void paraAI2::process (b2Vec2 newWorldPosInMeters)
//-----------------------------------------------------------------------------------------------------------------------
{
	DBGVAR(std::cout, arrayIndex);

	//std::cout << sys_getString ("Index %i Function : %s", arrayIndex, __func__) << std::endl;

	worldPositionInMeters = newWorldPosInMeters;

	checkAIScores ();

	switch (currentAIMode)
	{
		case AI2_MODE_PATROL:
//			patrol ();
			break;

		case AI2_MODE_ATTACK:
			attack ();
			break;

		case AI2_MODE_FLEE:
//			flee ();
			break;

		case AI2_MODE_HEAL:
//			heal ();
			break;

		case AI2_MODE_HUNT:
			hunt ();
			break;

		default:
			printf ("ERROR: Process invalid AI mode [ %i ]\n", currentAIMode);
			break;
	}
}