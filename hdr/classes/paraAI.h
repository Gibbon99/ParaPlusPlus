#ifndef PARA_PARAAI_H
#define PARA_PARAAI_H

#include <vector>
#include <box2d/b2_math.h>

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

	void checkScores ();

	void process (b2Vec2 newWorldPosInMeters);

	void patrol ();

	void changeModeTo (int newAIMode);

	void setAcceleration (double newAcceleration);

	void setMaxSpeed (double newMaxSpeed);

	void setWaypointIndex (int newIndex);

	double getAcceleration ();

	double getMaxSpeed ();

	void getWaypointDestination ();

	b2Vec2 getVelocity();

private:
	double           acceleration;
	double           maxSpeed;
	double           currentSpeed;
	b2Vec2           currentVelocity;
	b2Vec2           destination;
	int              wayPointIndex;
	int              wayPointDirection;
	int              currentAIMode;
	float            wayPointDistanceInMeters;
	b2Vec2           destinationCoordsInMeters;
	b2Vec2           worldPositionInMeters;
	b2Vec2           destDirection;
	std::vector<int> ai;
};

#endif //PARA_PARAAI_H
