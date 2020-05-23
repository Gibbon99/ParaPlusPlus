#ifndef PARA_PARAAI_H
#define PARA_PARAAI_H

#include <vector>
#include <box2d/b2_math.h>
#include <box2d/b2_fixture.h>

#define LOOK_AHEAD_DISTANCE     3.5     // In meters

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

	int getCurrentMode ();

	void modifyScore (int whichScore, int modifyAmount);

	void checkHealth ();

	void checkScores ();

	void process (b2Vec2 newWorldPosInMeters);

	void patrol ();

	void heal ();

	void renderVelocity ();

	void swapWaypointDirection ();

	void changeModeTo (int newAIMode);

	void setAcceleration (double newAcceleration);

	void setMaxSpeed (double newMaxSpeed);

	void setWaypointIndex (int newIndex);

	void setHealthPercent (float newHealthPercent);

	double getAcceleration ();

	double getMaxSpeed ();

	void getWaypointDestination ();

	void getNextAStarDestination ();

	b2Vec2 getVelocity ();

	b2Vec2 findHealingTile ();

private:
	bool             haveAStarDestination;
	int              aStarIndex          = -1;
	int              aStarWaypointIndex;
	bool             haveRequestedAStar  = false;
	double           acceleration;
	double           maxSpeed;
	double           currentSpeed;
	float            healthPercent;
	double           aiActionCounter;
	b2Vec2           currentVelocity;
	b2Vec2           destination;
	int              wayPointIndex;
	int              wayPointDirection;
	int              currentAIMode;
	float            wayPointDistanceInMeters;
	b2Vec2           destinationCoordsInMeters;
	b2Vec2           worldPositionInMeters;
	b2Vec2           destDirection;
	b2Vec2           lookAheadVelocity;                // How far ahead does the droid look for a player collision
	b2Fixture        *playerDroidFixture = nullptr;    // Used to determine if the droid will run into the player on current velocity
	std::vector<int> ai;
};

#endif //PARA_PARAAI_H
