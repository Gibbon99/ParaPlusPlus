#ifndef PARA_PARAAI_H
#define PARA_PARAAI_H

#include <vector>
#include <string>
#include <box2d/b2_math.h>
#include <box2d/b2_fixture.h>

#define LOOK_AHEAD_DISTANCE     3.5     // In meters

enum __PATROL_ACTIONS
{
	NORMAL_PATROL = 0,
	FIND_WAYPOINT,
	FOLLOW_ASTAR,
	MOVE_DIRECT_TO_LOCATION
};

enum _AI_MODES
{
	AI_MODE_PATROL = 0,
	AI_MODE_HELP,
	AI_MODE_FLEE,
	AI_MODE_HEAL,
	AI_MODE_ATTACK,
	AI_MODE_NUMBER
};

enum __WAYPOINT_DIRECTION
{
	WAYPOINT_UP = 0,
	WAYPOINT_DOWN
};

class paraAI
{
public:

	paraAI ();

	~paraAI ();

	int getHealScore ();

	void showValues ();

	void initAI ();

	int getCurrentMode ();

	std::string getPatrolAction ();

	int getAStarIndex ();

	void setAStarIndex (int newIndex);

	int getArrayIndex ();

	int getTargetDroid ();

	void sanityCheck ();

	std::string getString (int whichMode);

	void modifyScore (int whichScore, int modifyAmount);

	void checkHealth ();

	void checkScores ();

	void process (b2Vec2 newWorldPosInMeters);

	void patrol ();

	void heal ();

	void changeVelocity ();

	b2Vec2 findLocationWithLOS (int locationType);

	void renderVelocity ();

	void swapWaypointDirection ();

	void changeModeTo (int newAIMode);

	void setAcceleration (double newAcceleration);

	void setMaxSpeed (double newMaxSpeed);

	void setWaypointIndex (int newIndex);

	void setHealthPercent (float newHealthPercent);

	void setTargetDroid (int newTargetDroid);

	void setArrayIndex (int newIndex);

	void followAStar ();

	double getAcceleration ();

	double getMaxSpeed ();

	void setWaypointDirection(int newDirection);

	void getWaypointDestination ();

	void getNextAStarDestination ();

	b2Vec2 getVelocity ();

	b2Vec2 findHealingTile ();

	b2Vec2 findFleeTile ();

	void flee ();

	void attack ();

	void processLOSTimeout();

	bool isFleeTile (int tileIndex);

private:
	int       arrayIndex{};
	bool      haveAStarDestination{};
	int       aStarIndex                = -1;
	int       aStarWaypointIndex{};
	float     acceleration{};
	float     maxSpeed{};
	float     currentSpeed{};
	float     healthPercent{};
	float     aiActionCounter{};
	float     LOSTimeoutCounter;
	float     LOSTimeoutDelayValue      = 35.0f;
	b2Vec2    currentVelocity           = {0, 0};
	int       wayPointIndex             = 0;
	int       wayPointDirection         = WAYPOINT_UP;
	int       currentAIMode             = 0;
	int       patrolAction              = 0;
	int       targetDroid               = -2;   // Target nobody yet
	float     wayPointDistanceInMeters  = 0.0f;
	b2Vec2    destinationCoordsInMeters = {0, 0};
	b2Vec2    worldPositionInMeters     = {0, 0};
	b2Vec2    lookAheadVelocity         = {0, 0};                // How far ahead does the droid look for a player collision
	b2Fixture *droidFixture             = nullptr;    // Used to determine if the droid will run into the player on current velocity
	int       ai[AI_MODE_NUMBER]{};
};

#endif //PARA_PARAAI_H
