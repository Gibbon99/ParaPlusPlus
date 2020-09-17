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
	bool          overHealingTile       = false;
	bool          overTerminalTile      = false;
	bool          inTransferMode        = false;
	bool          overLiftTile          = false;
	bool          visibleToPlayer       = true;
	bool          weaponCanFire         = true;
	bool          lowInfluenceTimeleft  = false;
	bool          influenceFadeFlag     = false;
	int           index;                      // Used in physics callback
	int           currentMode;                // What is the droid doing; transfer, healing, terminal etc
	int           droidType;
	int           currentHealth;
	int           liftIndex             = 0;
	int           visibleValue          = 255;
	int           visibleState          = VISIBLE_STATE_IS_VISIBLE;
	int           transferTargetDroidIndex;
	int           transferTargetDroidType;
	float         weaponDelay;
	float         influenceTimeLeft;
	float         influenceFade;
	std::string   droidName;
	b2BodyDef     bodyDef;                      // Used for physics and collisions
	b2CircleShape shape;
	b2FixtureDef  fixtureDef;
	b2Body        *body = nullptr;
	_userData     *userData             = nullptr;
	b2Vec2        worldPosInPixels;
	b2Vec2        previousWorldPosInPixels;
	b2Vec2        velocity;
	paraSprite    sprite;
	paraAI        ai;
	//
	// Enemy droid fields
	int           aStarIndex            = -1;
	int           collisionCounter      = 0;
	float         collisionCounterDelay = 0.0f;

private:
	// Current speed
};

#endif //PARA_DROIDCLASS_H
