#ifndef PARA_DROIDCLASS_H
#define PARA_DROIDCLASS_H

#include <box2d/b2_body.h>
#include <box2d/box2d.h>
#include "classes/paraAI.h"
#include "system/physics.h"

extern int   collisionLimit;
extern float collisionCount;

class droidClass
{
public:

	int           index;					  // Used in physics callback
	int           currentMode;                // What is the droid doing; transfer, healing, terminal etc
	int           droidType;
	int           currentHealth;
	bool          overTerminalTile;
	bool          inTransferMode;
	bool          overLiftTile;
	int           liftIndex;
	bool          weaponCanFire;
	float         weaponDelay;
	std::string   droidName;
	b2BodyDef     bodyDef;                      // Used for physics and collisions
	b2CircleShape shape;
	b2FixtureDef  fixtureDef;
	b2Body        *body;
	_userData     *userData = nullptr;
	b2Vec2        worldPosInPixels;
	b2Vec2        previousWorldPosInPixels;
	b2Vec2        velocity;
	paraSprite    sprite;
	paraAI        ai;
	//
	// Enemy droid fields
	int           aStarIndex = -1;
	int           collisionCounter = 0;
	float         collisionCounterDelay = 0.0f;

private:
	// Current speed
};

#endif //PARA_DROIDCLASS_H
