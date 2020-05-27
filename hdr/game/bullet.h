#pragma once

#include <system/physics.h>
#include "main.h"

#define MY_PI 3.1415

class paraBullet
{
public:

	bool           inUse     = false;
	float          angle     = 0.0;
	b2Vec2         worldPosInMeters;
	b2Vec2         worldDestInMeters;
	b2Vec2         velocity;
	int            type;
	b2BodyDef      bodyDef;                      // Used for physics and collisions
	b2CircleShape  shape;
	b2PolygonShape boxShape;
	b2FixtureDef   fixtureDef;
	b2Body         *body     = nullptr;
	_userData      *userData = nullptr;
	paraSprite     sprite;
	float          disrupterFadeAmount;
	float          disrupterFade;
private:


};

extern float                   bulletDensity;
extern float                   bulletFriction;
extern float                   bulletAnimationSpeed;
extern float                   numDisrupterFrames;
extern float                   disrupterFadeAmount;
extern float                   bulletMoveSpeed;
extern std::vector<paraBullet> bullets;

// Add a newly created bullet to the array
void gam_addBullet (int bulletSourceIndex);

// Remove a bullet - called from Game Event loop - outside of Physics world step
void gam_removeBullet (int bulletIndex);

// Remove all the bullets
void gam_resetBullets ();

// Process physics and animation for any active bullets
void gam_processBullets ();

// Render any visible bullets to screen
void gam_renderBullets();