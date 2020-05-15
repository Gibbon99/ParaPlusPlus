#ifndef PARA_DROIDCLASS_H
#define PARA_DROIDCLASS_H

#include <box2d/b2_body.h>
#include <box2d/box2d.h>
#include "system/physics.h"

class droidClass
{
public:

	int           currentMode;                // What is the droid doing; transfer, healing, terminal etc
	int           droidType;
	double        currentSpeed;
	double        acceleration;
	double        maxSpeed;
	bool          overTerminalTile;
	bool          inTransferMode;
	bool          overLiftTile;
	int           liftIndex;
	bool          weaponCanFire;
	std::string   droidName;
	b2BodyDef     bodyDef;                      // Used for physics and collisions
	b2CircleShape shape;
	b2FixtureDef  fixtureDef;
	b2Body        *body;
	_userData     *userData = nullptr;

	b2Vec2 worldPosInPixels;
	b2Vec2 previousWorldPosInPixels;

	b2Vec2     destinationCoords;          // This is the line segment end point
	b2Vec2     destDirection;              // Which way is the droid heading
	b2Vec2     velocity;
	paraSprite sprite;
private:
	// Current speed
};

#endif //PARA_DROIDCLASS_H
