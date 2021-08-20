#ifndef PARA_PARAAI2_H
#define PARA_PARAAI2_H

#include <b2_math.h>

extern SDL_Thread  *sdlThreadID;

enum _AI2_PATROL_ACTIONS
{
	AI2_NORMAL_PATROL = 0,
	AI2_FIND_WAYPOINT,
	AI2_FOLLOW_ASTAR,
	AI2_MOVE_DIRECT_TO_LOCATION
};

enum _AI2_MODES
{
	AI2_MODE_ATTACK = 0,
	AI2_MODE_HUNT,
	AI2_MODE_FLEE,
	AI2_MODE_HEAL,
	AI2_MODE_PATROL,
	AI2_MODE_NUMBER
};

enum PATROL_WAYPOINT_DIRECTION
{
	PATROL_WAYPOINT_DIRECTION_UP = 0,
	PATROL_WAYPOINT_DIRECTION_DOWN
};

class paraAI2
{
public:

	void process (b2Vec2 newWorldPosInMeters);

	void modifyAIScore (int whichScore, int modifyAmount);

	void checkAIScores ();

	void changeAIModeTo (int newAIMode);

	void debugShowDestination ();

	std::string getAIActionString (int whichMode);

	bool isDestinationValid ();

	void attack ();

	b2Vec2 findFleeTile ();

	b2Vec2 findHealingTile ();

	b2Vec2 findOpenWaypoint ();

	b2Vec2 findLocationWithLOS (int locationType);

	void reachedDestination ();

	void initAI ();

	void doMovement (b2Vec2 newWorldPosInMeters);

	int checkPotentialCollision ();

	void getNextDestination ();

	void processVelocity ();

	b2Vec2 getVelocity ();

	void setArrayIndex (int newIndex);

	int getCurrentAIMode ();

	int getArrayIndex ();

	int getTargetDroid ();

	void setTargetDroid (int newTargetDroid);

	void setHealValue (float newHealthPercent);

	void setAcceleration (double newAcceleration);

	void setMaxSpeed (double newMaxSpeed);

	void setWaypointIndex (int newIndex);

	void setWaypointDirection (int newDirection);

	void switchWaypointDirection ();

	void setDestinationInPixels (b2Vec2 newDestination);

	void setDestinationInMeters (b2Vec2 newDestination);

	void debugShowValues ();

	double getAcceleration ();

	double getMaxSpeed ();

	float     desiredAttackDistance2 = 6;    // meters
	float     paddingSize2           = 2;
	paraAStar aStar;

private:

	int       currentAIMode               = AI2_MODE_PATROL;
	int       previousAIMode              = AI2_MODE_PATROL;
	int       patrolWaypointDirection     = PATROL_WAYPOINT_DIRECTION_DOWN;
	int       patrolWayPointIndex         = 0;
	int       arrayIndex                  = -1;
	int       targetDroid                 = NO_ATTACK_TARGET;
	float     currentSpeed                = 0.0f;
	float     acceleration                = 0.0f;
	float     maxSpeed                    = 0.0f;
	float     currentAttackDistance       = 0.0f;
	b2Vec2    directionAttackVector       = {0, 0};
	b2Vec2    currentVelocity             = {0, 0};
	b2Vec2    destinationCoordsInMeters   = {0, 0};
	b2Vec2    previousDestinationInMeters = {0, 0};
	b2Vec2    worldPositionInMeters       = {0, 0};
	b2Vec2    lookAheadVelocity           = {0, 0};                 // How far ahead does the droid look for a player collision
	b2Fixture *droidFixture               = nullptr;                        // Used to determine if the droid will run into the player on current velocity
	int       ai[AI2_MODE_NUMBER]{};
};


#endif //PARA_PARAAI2_H
