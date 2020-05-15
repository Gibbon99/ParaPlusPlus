#pragma once

#include "main.h"
#include "shipDecks.h"
#include <box2d/b2_math.h>
#include <box2d/b2_body.h>
#include <system/physics.h>

struct __liftSensor
{
	float          width;
	float          height;
	b2Vec2         worldPosition = {0, 0};
	b2BodyDef      bodyDef;                      // Used for physics and collisions
	b2PolygonShape shape;
	b2FixtureDef   fixtureDef;
	b2Body         *body;
	_userData      *userData;
};

extern int currentTunnel;

// Position the player on the requested lift on the new level
b2Vec2 gam_getLiftWorldPosition (int whichLift, std::string whichLevel);

// Setup lifts
void gam_setupLifts ();

// Get the positions of lifts
void gam_findLiftPositions (const std::string &levelName);

// Move the lift position
void gam_moveLift (int direction);

// Activate the lift
void gam_performLiftAction ();

// Set the deck index
void gam_setCurrentTunnelDeckIndex();

// Return the current tunnel in use
int gam_getCurrentTunnel();