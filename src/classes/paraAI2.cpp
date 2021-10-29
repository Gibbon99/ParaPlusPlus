#include <SDL2_gfxPrimitives.h>
#include <thread>
#include "io/logFile.h"
#include "classes/paraRandom.h"
#include "classes/paraAI2.h"
#include "system/util.h"
#include "game/player.h"
#include "game/pathFind.h"
#include "game/database.h"
#include "classes/paraBullet.h"

#define NO_THREAD 1

#define DESTINATION_ARRIVE_DISTANCE 8.0f
#define DESTINATION_SLOWDOWN_DISTANCE 24.0f
#define DESTINATION_SLOWDOWN_FACTOR 0.4f

#define COLLIDE_WITH_PLAYER -1
#define COLLIDE_WITH_NOTHING -2

float LOOK_AHEAD_DISTANCE {36.0f};

//-----------------------------------------------------------------------------------------------------------------------
//
// Render the destination vector
void paraAI2::debugShowDestination()
//-----------------------------------------------------------------------------------------------------------------------
{
	cpVect drawStart;
	cpVect drawFinish;

	drawStart  = worldPosInPixels;
	drawFinish = destinationPosInPixels;

	drawStart  = sys_worldToScreen (drawStart, 100);
	drawFinish = sys_worldToScreen (drawFinish, 100);

	thickLineRGBA (renderer.renderer, drawStart.x, drawStart.y, drawFinish.x, drawFinish.y, 2, 0, 200, 200, 255);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Show the current AI array values
void paraAI2::debugShowValues()
//-----------------------------------------------------------------------------------------------------------------------
{
	int counter = 0;

	con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("%s %s %s %s %s %s", "        ATK", "   HNT", "   FLE", "   HEL", "   PAT", "   WIT"));

	for (auto droidItr: g_shipDeckItr->second.droid)
	{
		if (droidItr.getCurrentMode () == DROID_MODE_NORMAL)
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("%i   --  %2i --   %2i --   %2i --  %2i  --  %2i  -- %2i -- %s - ", counter, droidItr.ai2.ai[0], droidItr.ai2.ai[1], droidItr.ai2.ai[2], droidItr.ai2.ai[3], droidItr.ai2.ai[4], droidItr.ai2.ai[5], getAIActionString (droidItr.ai2.getCurrentAIMode ()).c_str ()));
		else
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("%i %s", counter, "Dead"));

		counter++;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Get the current mode
int paraAI2::getCurrentAIMode()
//-----------------------------------------------------------------------------------------------------------------------
{
	return currentAIMode;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Get a string from the current AI Mode
std::string paraAI2::getAIActionString(int whichMode)
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
void paraAI2::attack()
//-----------------------------------------------------------------------------------------------------------------------
{
	DBGVAR(std::cout, arrayIndex);

//	std::cout << sys_getString ("Index %i Function : %s", arrayIndex, __func__) << std::endl;

	if (targetDroid == NO_ATTACK_TARGET)
	{
		modifyAIScore (AI2_MODE_ATTACK, -(50 + difficultyValue));
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
void paraAI2::hunt()
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
		log_addEvent (sys_getString ("[ %i ] - Within attack range of player while in HUNT mode.", arrayIndex));
#endif

		modifyAIScore (AI2_MODE_HUNT, -60);
		modifyAIScore (AI2_MODE_ATTACK, 60 + difficultyValue);
		return;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Find the waypoint furthest away from the player - return coordinates in tile coords
cpVect paraAI2::findFleeTile()
//-----------------------------------------------------------------------------------------------------------------------
{
	int   newWaypointIndex {};
	float longestDistance = 1;
	float currentDistance = 0;

	for (newWaypointIndex = 0; newWaypointIndex != static_cast<int>(g_shipDeckItr->second.wayPoints.size () - 1); newWaypointIndex++)
	{
		if (!cpveql (playerDroid.getWorldPosInPixels (), g_shipDeckItr->second.wayPoints[newWaypointIndex]))
		{
			currentDistance = cpvdist (playerDroid.getWorldPosInPixels (), g_shipDeckItr->second.wayPoints[newWaypointIndex]);
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
// Find a healing tile - if none on this level then flee - return coordinates in tile coords
cpVect paraAI2::findHealingTile()
//-----------------------------------------------------------------------------------------------------------------------
{
	float  shortestDistance = 1000;
	float  currentDistance;
	cpVect healingDestination;

	if (g_shipDeckItr->second.healing.empty ())
	{
		healingDestination.x = -1;
		healingDestination.y = -1;
		return healingDestination;
	}

	for (auto healItr: g_shipDeckItr->second.healing)
	{
		currentDistance = cpvdist (worldPosInPixels, healItr.worldPosInPixels);
		if (currentDistance > 24 * 2)      // Needs to be far enough away to create a aStar path
		{
			if (currentDistance < shortestDistance)
			{
				shortestDistance   = currentDistance;
				healingDestination = healItr.worldPosInPixels;
			}
		}
	}
	return sys_convertPixelsToTiles (healingDestination);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Find the first available waypoint that is not blocked by a wall or door
cpVect paraAI2::findOpenWaypoint()
//-----------------------------------------------------------------------------------------------------------------------
{
	cpVect invalidWaypoint {-1, -1};

	for (int newWaypointIndex = 0; newWaypointIndex != static_cast<int>(g_shipDeckItr->second.wayPoints.size () - 1); newWaypointIndex++)
	{
		if (!cpveql (worldPosInPixels, g_shipDeckItr->second.wayPoints[newWaypointIndex]))
		{
			if (patrolWayPointIndex != newWaypointIndex)    // Don't check current waypoint
			{
				auto raycastResult = cpSpaceSegmentQueryFirst (sys_returnPhysicsWorld (), worldPosInPixels, g_shipDeckItr->second.wayPoints[newWaypointIndex], 2.0f, CP_SHAPE_FILTER_ALL, nullptr);
				if (!raycastResult)    // Found a waypoint not crossing a wall
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

#ifdef DEBUG_AI2
	log_addEvent (sys_getString ("Break - [ %s ] couldn't find an open waypoint.", __func__));
#endif

	return invalidWaypoint;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Find the nearest location taking into account solid walls
cpVect paraAI2::findLocationWithLOS(AI2_PATROL_ACTIONS locationType)
//-----------------------------------------------------------------------------------------------------------------------
{
	cpVect             newDestinationCoordsInPixels {};
	int                newWaypointIndex {};
	int                shortestDistance {};
	int                currentDistance {};
	cpShapeFilter      shapeFilter {};
	cpSegmentQueryInfo segmentQueryResult {};

	shapeFilter.group      = CP_NO_GROUP;
	shapeFilter.mask       = PHYSIC_TYPE_WALL;      // Check for walls
	shapeFilter.categories = PHYSIC_TYPE_ENEMY;


#ifdef DEBUG_AI2
	log_addEvent (sys_getString ("[ %i ] - [ %s ] Search for a visible waypoint.", arrayIndex, __func__));
#endif

	switch (locationType)
	{
		case AI2_PATROL_ACTIONS::AI2_FIND_WAYPOINT:
			//
			// Find nearest waypoint that isn't blocked by a wall
			newDestinationCoordsInPixels.x = -1;
			shortestDistance = 1000;

			for (newWaypointIndex = 0; newWaypointIndex != static_cast<int>(g_shipDeckItr->second.wayPoints.size () - 1); newWaypointIndex++)
			{
				if (!cpveql (worldPosInPixels, g_shipDeckItr->second.wayPoints[newWaypointIndex]))
				{
					auto raycastResult = cpSpaceSegmentQueryFirst (sys_returnPhysicsWorld (), worldPosInPixels, g_shipDeckItr->second.wayPoints[newWaypointIndex], 2.0f, shapeFilter, &segmentQueryResult);

					if (nullptr != raycastResult)
					{
						currentDistance = cpvdist (worldPosInPixels, g_shipDeckItr->second.wayPoints[newWaypointIndex]);
						if (currentDistance < shortestDistance)
						{
//							if (shortestDistance > 0)
							{
								shortestDistance             = currentDistance;
								newDestinationCoordsInPixels = g_shipDeckItr->second.wayPoints[newWaypointIndex];
								switch (currentAIMode)
								{
									case AI2_MODE_PATROL:
										patrolWayPointIndex = newWaypointIndex;
										break;
								}
							}
#ifdef DEBUG_AI2
							log_addEvent (sys_getString ("[ %i ] - Found a closer waypoint [ %i ]", patrolWayPointIndex, shortestDistance));
#endif
						}
					}
				}
			}
			return newDestinationCoordsInPixels;
			break;
	}
	return cpVect {0, 0};
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Sanity check that droid is not attempting to get to a destination behind a wall
// Return false if there is a wall between droid and destination
bool paraAI2::isDestinationValid()
//-----------------------------------------------------------------------------------------------------------------------
{
	if ((destinationPosInPixels.x <= 0) || (destinationPosInPixels.y <= 0))
		return false;

	if (!cpveql (worldPosInPixels, destinationPosInPixels))
	{
		auto raycastResult = cpSpaceSegmentQueryFirst (sys_returnPhysicsWorld (), worldPosInPixels, destinationPosInPixels, 2.0f, CP_SHAPE_FILTER_ALL, nullptr);
		if (raycastResult)
			return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Insert a value for each action score
void paraAI2::initAI()
//-----------------------------------------------------------------------------------------------------------------------
{
	paraRandom randomDirection;

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
cpVect paraAI2::debugGetDestinationCoordsInPixels()
//-----------------------------------------------------------------------------------------------------------------------
{
	return destinationPosInPixels;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Get the next destination coordinates
void paraAI2::getNextDestination()
//-----------------------------------------------------------------------------------------------------------------------
{
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
			destinationPosInPixels = g_shipDeckItr->second.wayPoints[patrolWayPointIndex];
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
				case ASTAR_DIRECTION::ASTAR_ERROR:
					break;

				case ASTAR_DIRECTION::DESTINATION:
					reachedDestination ();
					break;

				case ASTAR_DIRECTION::START:
					aStar.switchTravelDirection ();
					break;

				default:    // Move UP or DOWN array
					destinationPosInPixels = aStar.getWaypoint ();
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
void paraAI2::doMovement(cpVect newWorldPosInPixels)
//-----------------------------------------------------------------------------------------------------------------------
{
	DBGVAR(std::cout, arrayIndex);

//	std::cout << sys_getString ("Index %i Function : %s", arrayIndex, __func__) << std::endl;

	worldPosInPixels = newWorldPosInPixels;

	if (destinationPosInPixels.x < 0)
	{
		log_addEvent (sys_getString ("[ %i ] - [ %s ]  - Have an invalid destination.", arrayIndex, __func__));
		return;
	}

	switch (currentAIMode)
	{
		case AI2_MODE_HUNT:
			currentAttackDistance = cpvdist (playerDroid.getWorldPosInPixels (), worldPosInPixels);
#ifdef NO_THREAD
			if (cpvdist (destinationPosInPixels, worldPosInPixels) < DESTINATION_ARRIVE_DISTANCE)    // Conditional jump or move on uninitialised value
				getNextDestination ();

			checkPotentialCollision ();
#else
			if (ASTAR_STATUS_READY == aStar.getPathStatus ())
			{
				if (cpvdist (destinationCoordsInMeters, worldPositionInMeters) < DESTINATION_ARRIVE_DISTANCE)    // Conditional jump or move on uninitialised value
					getNextDestination ();

				checkPotentialCollision ();
			}
#endif
			break;

		case AI2_MODE_ATTACK:
			if (targetDroid == TARGET_PLAYER)      // Get distance and direction to the player
			{
				currentAttackDistance = cpvdist (playerDroid.getWorldPosInPixels (), worldPosInPixels);
				directionAttackVector = cpvsub (playerDroid.getWorldPosInPixels (), worldPosInPixels);
			}
			else if (targetDroid != NO_ATTACK_TARGET)                        // Get distance and direction to the other droid
			{
				currentAttackDistance = cpvdist (g_shipDeckItr->second.droid[targetDroid].getWorldPosInPixels (), worldPosInPixels);
				directionAttackVector = cpvsub (g_shipDeckItr->second.droid[targetDroid].getWorldPosInPixels (), worldPosInPixels);
			}

			if (currentAttackDistance > desiredAttackDistance2)    // Droid is too far away - move closer
			{
				directionAttackVector = cpvnormalize (directionAttackVector);
				directionAttackVector = cpvmult (directionAttackVector, currentSpeed);
				velocity              = directionAttackVector;
				return;
			}

			if (currentAttackDistance < desiredAttackDistance2)     // Too close - move away - but check that path is clear ?
			{
				directionAttackVector = -directionAttackVector;
				directionAttackVector = cpvnormalize (directionAttackVector);
				directionAttackVector = cpvmult (directionAttackVector, currentSpeed);
				velocity              = directionAttackVector;
				return;
			}

			break;

		case AI2_MODE_HEAL:
		case AI2_MODE_FLEE:
#ifdef NO_THREAD
			if (cpvdist (destinationPosInPixels, worldPosInPixels) < DESTINATION_ARRIVE_DISTANCE)
				getNextDestination ();

			checkPotentialCollision ();
#else
			if (ASTAR_STATUS_READY == aStar.getPathStatus ())
			{
				if (cpvdist (destinationCoordsInMeters, worldPositionInMeters) < DESTINATION_ARRIVE_DISTANCE)    // Conditional jump or move on uninitialised value
					getNextDestination ();

				checkPotentialCollision ();
			}
#endif
			break;

		default:
			if (cpvdist (destinationPosInPixels, worldPosInPixels) < DESTINATION_ARRIVE_DISTANCE)
				getNextDestination ();

			checkPotentialCollision ();
			break;
	}
	processVelocity ();
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the waypoint index
void paraAI2::setWaypointIndex(int newIndex)
//-----------------------------------------------------------------------------------------------------------------------
{
	patrolWayPointIndex = newIndex;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the current velocity
cpVect paraAI2::getVelocity()
//-----------------------------------------------------------------------------------------------------------------------
{
	return velocity;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the current velocity
void paraAI2::setVelocity(cpVect newVelocity)
//-----------------------------------------------------------------------------------------------------------------------
{
	velocity = newVelocity;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the current position
cpVect paraAI2::getWorldPosInPixels()
//-----------------------------------------------------------------------------------------------------------------------
{
	return worldPosInPixels;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Get the previous world position
cpVect paraAI2::getPreviousWorldPosInPixels()
//-----------------------------------------------------------------------------------------------------------------------
{
	return previousPosInPixels;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the previous world position
void paraAI2::setPreviousPosInPixels(cpVect newPreviousPosition)
//-----------------------------------------------------------------------------------------------------------------------
{
	previousPosInPixels = newPreviousPosition;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the current world position
void paraAI2::setWorldPosInPixels(cpVect newWorldPosition)
//-----------------------------------------------------------------------------------------------------------------------
{
	worldPosInPixels = newWorldPosition;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Check if droid is heading for a collision with player or another droid
// Return TRUE if going to collide
int paraAI2::checkPotentialCollision()
//-----------------------------------------------------------------------------------------------------------------------
{
	DBGVAR(std::cout, arrayIndex);
	//
	// Change direction if going to run into player
	if (cpvdist (lookAheadVelocity, playerDroid.ai2.getWorldPosInPixels ()) < LOOK_AHEAD_DISTANCE)
	{
		currentSpeed = maxSpeed * 0.5; // 0.0;
		velocity     = {0.0, 0.0};
		collisionCounterPlayer++;
		modifyAIScore (AI2_MODE_ATTACK, 5);     // Get more agro the more collisions occur
		if (collisionCounterPlayer < IGNORE_SWAP_DIRECTION_LIMIT)
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
				if (cpvdist (droidItr.getWorldPosInPixels (), lookAheadVelocity) < LOOK_AHEAD_DISTANCE)
				{
					currentSpeed = maxSpeed * 0.5;
					velocity     = {0.0, 0.0};
					collisionCounterDroid++;
					if (collisionCounterDroid < IGNORE_SWAP_DIRECTION_LIMIT)
					{
						switchWaypointDirection ();
						collisionCounterDroid++;
					}

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
void paraAI2::switchWaypointDirection()
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
void paraAI2::setWaypointDirection(PATROL_WAYPOINT_DIRECTION newDirection)
//-----------------------------------------------------------------------------------------------------------------------
{
	patrolWaypointDirection = newDirection;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Do any acceleration and apply current speed to velocity ( direction ) vector
// Update lookAheadVelocity vector with new direction vector
void paraAI2::processVelocity()
//-----------------------------------------------------------------------------------------------------------------------
{
	DBGVAR(std::cout, arrayIndex);

	//std::cout << sys_getString ("Index %i Function : %s", arrayIndex, __func__) << std::endl;

	//
	// Reduce speed as approaching destination
	//
	// TODO Do as a percentage of distance from destination
	if (cpvdist (destinationPosInPixels, worldPosInPixels) < DESTINATION_SLOWDOWN_DISTANCE)  // Conditional jump on unitialised value
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

	velocity          = cpvsub (destinationPosInPixels, worldPosInPixels);
	velocity          = cpvnormalize (velocity);
	lookAheadVelocity = worldPosInPixels + cpvmult (velocity, LOOK_AHEAD_DISTANCE + (g_shipDeckItr->second.droid[arrayIndex].getDroidType () / 5));
	velocity          = cpvmult (velocity, static_cast<float>(currentSpeed));
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the acceleration for this droid
void paraAI2::setAcceleration(double newAcceleration)
//-----------------------------------------------------------------------------------------------------------------------
{
	acceleration = newAcceleration;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the top speed for this droid
void paraAI2::setMaxSpeed(double newMaxSpeed)
//-----------------------------------------------------------------------------------------------------------------------
{
	maxSpeed = newMaxSpeed;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the destination location - pass in destination in pixel ( world ) coordinates
void paraAI2::setDestinationInPixels(cpVect newDestination)
//-----------------------------------------------------------------------------------------------------------------------
{
	destinationPosInPixels = newDestination;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the index into the vector array for this droid
void paraAI2::setArrayIndex(int newIndex)
//-----------------------------------------------------------------------------------------------------------------------
{
	arrayIndex = newIndex;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the current target
int paraAI2::getTargetDroid()
//-----------------------------------------------------------------------------------------------------------------------
{
	return targetDroid;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the current acceleration for this droid
double paraAI2::getAcceleration()
//-----------------------------------------------------------------------------------------------------------------------
{
	return acceleration;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the current maxSpeed for this droid
double paraAI2::getMaxSpeed()
//-----------------------------------------------------------------------------------------------------------------------
{
	return maxSpeed;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Someone shot this droid - set them as the target for an attack
void paraAI2::setTargetDroid(int newTargetDroid)
//-----------------------------------------------------------------------------------------------------------------------
{
	targetDroid = newTargetDroid;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the index into the vector array for this droid
int paraAI2::getArrayIndex()
//-----------------------------------------------------------------------------------------------------------------------
{
	return arrayIndex;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Debug - Return current patrolWaypointIndex
int paraAI2::getPatrolWaypointIndex()
//-----------------------------------------------------------------------------------------------------------------------
{
	return patrolWayPointIndex;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Increase a utility action score - modify can be positive or negative value
void paraAI2::modifyAIScore(int whichScore, int modifyAmount)
//-----------------------------------------------------------------------------------------------------------------------
{
	DBGVAR(std::cout, arrayIndex);

	//std::cout << sys_getString ("Index %i Function : %s", arrayIndex, __func__) << std::endl;

	if ((whichScore < 0) || (whichScore > (AI2_MODE_NUMBER - 1)))
		return;

	ai[whichScore] += modifyAmount;

	ai[whichScore] = ai[whichScore] > 100 ? 100 : ai[whichScore];
	ai[whichScore] = ai[whichScore] < 0 ? 0 : ai[whichScore];
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Check to see if the current mode needs to change
void paraAI2::checkAIScores()
//-----------------------------------------------------------------------------------------------------------------------
{
	DBGVAR(std::cout, arrayIndex);

	int highestScore = 0;
	int newAIAction  = 0;

	//
	// Prioritise healing above others
	if (ai[AI2_MODE_HEAL] >= 80)
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
	for (auto i     = 0; i != AI2_MODE_NUMBER; i++)
	{
		if (ai[i] > highestScore)
		{
			highestScore = ai[i];
			newAIAction  = i;
		}
	}
	//
	// If changing from ATTACK to something else - reset target Droid
	if ((newAIAction != AI2_MODE_ATTACK) && (currentAIMode == AI2_MODE_ATTACK))
		targetDroid = NO_ATTACK_TARGET;

	//
	// Change from witnessing an attack to actually attacking the droid witnessed doing the attacks
	if (newAIAction == AI2_MODE_WITNESS)
	{
		newAIAction = AI2_MODE_ATTACK;
		targetDroid = witnessDroid;
		modifyAIScore (AI2_MODE_WITNESS, -90);
		modifyAIScore (AI2_MODE_ATTACK, 100 + difficultyValue);
		changeAIModeTo (newAIAction);
		return;
	}
	//
	// Already doing this action
	if (currentAIMode == newAIAction)
		return;

#ifdef DEBUG_AI2
	log_addEvent (sys_getString ("[ %i ] Found a new AI mode to change to [ %s ]", arrayIndex, getAIActionString (newAIAction).c_str ()));
#endif

	changeAIModeTo (newAIAction);

#ifdef DEBUG_AI2
	log_addEvent (sys_getString ("[ %i ] AI Scores [ %i %i %i %i %i %i ]", arrayIndex, ai[0], ai[1], ai[2], ai[3], ai[4], ai[5]));
#endif
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the current percentage of health left
void paraAI2::setHealValue(float newHealthPercent)
//-----------------------------------------------------------------------------------------------------------------------
{
	ai[AI2_MODE_HEAL] = 100 - newHealthPercent;

#ifdef DEBUG_AI
	//	std::cout << "[ " << arrayIndex << " ] Health percent is now : " << healthPercent << std::endl;
#endif
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the droid that this droid has witnessed
void paraAI2::setWitnessDroid(int newWitnessDroid)
//-----------------------------------------------------------------------------------------------------------------------
{
	witnessDroid = newWitnessDroid;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Run common aStar functions
int paraAI2::runAStarCode(cpVect destinationTile)
//-----------------------------------------------------------------------------------------------------------------------
{
	aStar.setID (arrayIndex);

	auto returnCode = aStar.requestNewPath (sys_convertPixelsToTiles (worldPosInPixels), destinationTile);

#ifdef NO_THREAD
	aStar.searchThread ();
#else
	aStar.startThread ();
#endif

	return returnCode;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Change to a new mode - run functions required before running the new AI action
void paraAI2::changeAIModeTo(int newAIMode)
//-----------------------------------------------------------------------------------------------------------------------
{
	cpVect tileDestination {};
	int    returnCode {};

	std::thread aStarThread2;

#ifdef DEBUG_AI2
	log_addEvent (sys_getString ("[ %i ] - Running function : %s", arrayIndex, __func__));
#endif

	if (aStar.stillRunning ())
		return;     // Wait for thread to finish before changing mode

	aStar.stopUsingPath ();

	previousAIMode = currentAIMode;
	currentAIMode  = newAIMode;

#ifdef DEBUG_AI2
	log_addEvent (sys_getString ("[ %i ] - Changing to new AI mode - [ %s ]", arrayIndex, getAIActionString (currentAIMode).c_str ()));
#endif

	switch (currentAIMode)
	{
		//-------------------------- AI_MODE_PATROL ---------------------------

		case AI2_MODE_PATROL:    // Find the nearest clear waypoint and move to it

#ifdef DEBUG_AI2
			log_addEvent (sys_getString ("[ %i ] - Changing to PATROL mode. Looking for nearest waypoint to move to.", arrayIndex));
#endif
			destinationPosInPixels = (findLocationWithLOS (AI2_PATROL_ACTIONS::AI2_FIND_WAYPOINT));
			if ((destinationPosInPixels.x < 0) || (destinationPosInPixels.y < 0))
			{
				//
				// TODO: Something clever
				log_addEvent (sys_getString ("[ %i ] - Couldn't find a nearest waypoint to set PATROL to. Increase FLEE amount.", arrayIndex));

				modifyAIScore (AI2_MODE_PATROL, -60);
				modifyAIScore (AI2_MODE_FLEE, 80);
//				changeAIModeTo (AI2_MODE_FLEE);
				return;
			}
			break;

			//-------------------------- AI_MODE_HEAL ---------------------------

		case AI2_MODE_HEAL:

#ifdef DEBUG_AI2
			log_addEvent (sys_getString ("[ %i ] - Changing to HEAL mode. Looking for nearest waypoint to move to.", arrayIndex));
#endif
			tileDestination = findHealingTile ();
			if (tileDestination.x < 0)                // No healing tiles on level - change to flee
			{
#ifdef DEBUG_AI2
				log_addEvent (sys_getString ("[ %i ] - Couldn't find a nearest tile to set HEAL to. Increase FLEE amount.", arrayIndex));
#endif
				modifyAIScore (AI2_MODE_HEAL, -100);
				modifyAIScore (AI2_MODE_FLEE, 40);
//				changeAIModeTo (AI2_MODE_FLEE);
				return;
			}

#ifdef DEBUG_AI2
			log_addEvent (sys_getString ("[ %i ] - Found the destination healing tile [ %4.3f %4.3f ]", arrayIndex, tileDestination.x, tileDestination.y));
#endif
			returnCode = runAStarCode (tileDestination);    // tileDestination is in Tile coords

#ifdef NO_THREAD
			if (ASTAR_STATUS_TOO_SHORT == returnCode) // TODO: Move straight to healing tile if not blocked by wall ??
			{
#ifdef DEBUG_AI2
				log_addEvent (sys_getString ("[ %i ] - Heal tile is too close for aStar. Increase FLEE amount.", arrayIndex));
#endif

				modifyAIScore (AI2_MODE_HEAL, -100);
				modifyAIScore (AI2_MODE_FLEE, 40);
				modifyAIScore (AI2_MODE_PATROL, -70);
			}
			destinationPosInPixels = aStar.getWaypoint ();
#endif
			break;

			//-------------------------- AI_MODE_FLEE ---------------------------

		case AI2_MODE_FLEE:
#ifdef DEBUG_AI2
			log_addEvent (sys_getString ("[ %i ] - Changing to FLEE mode. Looking for nearest waypoint to move to.", arrayIndex));
#endif
			tileDestination = findFleeTile ();
			if (tileDestination.x < 0)                // Couldn't find a suitable tile - change to PATROL
			{
#ifdef DEBUG_AI2
				log_addEvent (sys_getString ("[ %i ] - Couldn't find a suitable FLEE tile. Increase PATROL amount.'.", arrayIndex));
#endif
				modifyAIScore (AI2_MODE_FLEE, -100);
				modifyAIScore (AI2_MODE_PATROL, 40);
				return;
			}

#ifdef DEBUG_AI2
			log_addEvent (sys_getString ("[ %i ] - Found the destination flee tile [ %4.3f %4.3f ]", arrayIndex, tileDestination.x, tileDestination.y));
#endif

			returnCode = runAStarCode (tileDestination);
#ifdef NO_THREAD
			if (ASTAR_STATUS_TOO_SHORT == returnCode)
			{
				modifyAIScore (AI2_MODE_FLEE, -100);
				modifyAIScore (AI2_MODE_PATROL, 40);
			}
			destinationPosInPixels = aStar.getWaypoint ();
#endif
			break;

			//-------------------------- AI_MODE_HUNT ---------------------------

		case AI2_MODE_HUNT:
#ifdef DEBUG_AI2
			log_addEvent (sys_getString ("[ %i ] - Changing to HUNT mode.", arrayIndex));
#endif
			tileDestination = gam_getLastPlayerTrail ();
			if (tileDestination.x < 0)                // Couldn't get a tile from player trail
			{
#ifdef DEBUG_AI2
				log_addEvent (sys_getString ("[ %i ] - Couldn't find player trail tile. Increase FLEE amount.", arrayIndex));
#endif
				modifyAIScore (AI2_MODE_HUNT, -100);
				modifyAIScore (AI2_MODE_FLEE, 40);
				return;
			}

#ifdef DEBUG_AI2
			log_addEvent (sys_getString ("[ %i ] - Found the player trail tile [ %4.3f %4.3f ]", arrayIndex, tileDestination.x, tileDestination.y));
#endif
			returnCode = runAStarCode (tileDestination);
#ifdef NO_THREAD
			if (ASTAR_STATUS_TOO_SHORT == returnCode)
			{
				modifyAIScore (AI2_MODE_HUNT, -100);
				modifyAIScore (AI2_MODE_ATTACK, -(50 + difficultyValue));
				modifyAIScore (AI2_MODE_FLEE, 40);
				modifyAIScore (AI2_MODE_PATROL, 70);
			}
			destinationPosInPixels = aStar.getWaypoint ();
#endif
			huntCountdownValue = 1.0f;
			break;

			//-------------------------- AI_MODE_ATTACK ---------------------------

		case AI2_MODE_ATTACK:

#ifdef DEBUG_AI2
			log_addEvent (sys_getString ("[ %i ] - In ATTACK mode case statement.", arrayIndex));
			debugShowValues ();
#endif
			if (!dataBaseEntry[g_shipDeckItr->second.droid[arrayIndex].getDroidType ()].canShoot)
			{
#ifdef DEBUG_AI2
				log_addEvent (sys_getString ("[ %i ] - Droid can not shoot. Increase FLEE amount.", arrayIndex));
#endif
				modifyAIScore (AI2_MODE_ATTACK, -100);
				modifyAIScore (AI2_MODE_FLEE, 40);
			}
			break;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Reached the destination waypoint
void paraAI2::reachedDestination()
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
void paraAI2::checkAttackVisibility()
//-----------------------------------------------------------------------------------------------------------------------
{
	DBGVAR(std::cout, arrayIndex);

	if (targetDroid == TARGET_PLAYER)
	{
		if (!g_shipDeckItr->second.droid[arrayIndex].visibleToPlayer)
		{
			g_shipDeckItr->second.droid[arrayIndex].ai2.modifyAIScore (AI2_MODE_HUNT, 40 + difficultyValue);
			g_shipDeckItr->second.droid[arrayIndex].ai2.modifyAIScore (AI2_MODE_ATTACK, -10);
			return;
		}
		else    // Found player again while still in attack mode
		{
			g_shipDeckItr->second.droid[arrayIndex].ai2.modifyAIScore (AI2_MODE_HUNT, -30);
			g_shipDeckItr->second.droid[arrayIndex].ai2.modifyAIScore (AI2_MODE_ATTACK, 40 + difficultyValue);
			return;
		}
	}
	else if (targetDroid != NO_ATTACK_TARGET)   // Targeting another droid
	{
		auto returnShape = cpSpaceSegmentQueryFirst (sys_returnPhysicsWorld (), worldPosInPixels, g_shipDeckItr->second.droid[targetDroid].getWorldPosInPixels (), 2.0f, CP_SHAPE_FILTER_NONE, nullptr);

		if (returnShape != nullptr)
		{
			auto shapePointer = cpShapeGetUserData (returnShape);
			if (nullptr != shapePointer)
			{
				auto *userDataA = reinterpret_cast<_userData *>(shapePointer);

				if (userDataA->dataValue == targetDroid)
				{
					modifyAIScore (AI2_MODE_ATTACK, 10 + difficultyValue);   // Still in visible range
				}
				else
				{
					modifyAIScore (AI2_MODE_ATTACK, -10 + difficultyValue);   // Lost sight of target droid
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Process the current AI mode - Pass in the current world position in meters
void paraAI2::process(cpVect newWorldPosInPixels)
//-----------------------------------------------------------------------------------------------------------------------
{
	DBGVAR(std::cout, arrayIndex);

	//std::cout << sys_getString ("Index %i Function : %s", arrayIndex, __func__) << std::endl;

	worldPosInPixels = newWorldPosInPixels;

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