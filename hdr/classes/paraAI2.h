#pragma once

#include "paraAStar.h"

#define IGNORE_SWAP_DIRECTION_LIMIT 5

/*
#define DBGVAR(os, var) \
  (os) << "DBG: " << __func__ << "(" << __LINE__ << ") "\
       << #var << " = [" << (var) << "]" << std::endl
*/

#define DBGVAR(os, var)

//
// Use strongly typed enumeration type
enum class AI2_PATROL_ACTIONS
{
	AI2_FIND_WAYPOINT = 0
};

enum _AI2_MODES
{
	AI2_MODE_ATTACK = 0,
	AI2_MODE_HUNT,
	AI2_MODE_FLEE,
	AI2_MODE_HEAL,
	AI2_MODE_PATROL,
	AI2_MODE_WITNESS,
	AI2_MODE_NUMBER
};

enum class PATROL_WAYPOINT_DIRECTION
{
	UP = 0,
	DOWN
};

class paraAI2
{
public:

	void process(cpVect newWorldPosInPixels);

	void modifyAIScore(int whichScore, int modifyAmount);

	void checkAIScores();

	void changeAIModeTo(int newAIMode);

	void debugShowDestination();

	std::string getAIActionString(int whichMode);

	bool isDestinationValid();

	void attack();

	void hunt();

	cpVect findFleeTile();

	cpVect findHealingTile();

	cpVect findOpenWaypoint();

	cpVect findLocationWithLOS(AI2_PATROL_ACTIONS locationType);

	void reachedDestination();

	void initAI();

	void doMovement(cpVect newWorldPosInPixels);

	int checkPotentialCollision();

	void getNextDestination();

	int runAStarCode(cpVect destinationTile);

	void processVelocity();

	void setArrayIndex(int newIndex);

	int getCurrentAIMode();

	int getArrayIndex();

	int getTargetDroid();

	void setTargetDroid(int newTargetDroid);

	void setHealValue(float newHealthPercent);

	void setAcceleration(double newAcceleration);

	void setMaxSpeed(double newMaxSpeed);

	void setWaypointIndex(int newIndex);

	void setWaypointDirection(PATROL_WAYPOINT_DIRECTION newDirection);

	void switchWaypointDirection();

	void setDestinationInPixels(cpVect newDestination);

	void setDestinationInMeters(cpVect newDestination);

	cpVect debugGetDestinationCoordsInPixels();

	void debugShowValues();

	double getAcceleration();

	double getMaxSpeed();

	int getPatrolWaypointIndex();

	void checkAttackVisibility();

	cpVect getWorldPosInPixels();

	cpVect getVelocity();

	void setVelocity(cpVect newVelocity);

	void setWorldPosInPixels(cpVect newWorldPosition);

	void setPreviousPosInPixels(cpVect newPreviousPosition);

	cpVect getPreviousWorldPosInPixels();

	void setWitnessDroid(int newWitnessDroid);

	float     desiredAttackDistance2 {90};    // pixels
	float     paddingSize2 {12};
	paraAStar aStar;

	int   collisionCounterDroid {0};
	int   collisionCounterPlayer {0};
	float collisionCounterDelayDroid {5.0};
	float collisionCounterDelayPlayer {5.0};

private:

	int                       currentAIMode {AI2_MODE_PATROL};
	int                       previousAIMode {AI2_MODE_PATROL};
	int                       patrolWayPointIndex {};
	int                       arrayIndex {-1};
	int                       targetDroid {NO_ATTACK_TARGET};
	int                       swapDirectionCounter {};
	int                       witnessDroid {NO_ATTACK_TARGET};
	float                     currentSpeed {};
	float                     acceleration {};
	float                     maxSpeed {};
	float                     currentAttackDistance {};
	float                     huntCountdownValue {};
	float                     huntCountdownModifier {0.2f};
	cpVect                    velocity               = {0, 0};
	cpVect                    directionAttackVector  = {0, 0};
	cpVect                    destinationPosInPixels = {0, 0};
	cpVect                    previousPosInPixels    = {0, 0};
	cpVect                    worldPosInPixels       = {0, 0};
	cpVect                    lookAheadVelocity      = {0, 0};                 // How far ahead does the droid look for a player collision
	//	cpVect                    previousDestinationInPixels = {0, 0};
	int                       ai[AI2_MODE_NUMBER] {};
	PATROL_WAYPOINT_DIRECTION patrolWaypointDirection;
};