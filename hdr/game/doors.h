#pragma once

#include <box2d/b2_math.h>
#include <box2d/b2_body.h>
#include <system/physics.h>
#include "main.h"

enum doorBulletDirection
{
	DIRECTION_UP = 0,
	DIRECTION_ACROSS
};

//-----------------------------------------------------------------------------
//
// defines used for controlling doors - tile number
//-----------------------------------------------------------------------------
#define DOOR_ACROSS_OPEN_1        15
#define DOOR_ACROSS_OPEN_2        16
#define DOOR_ACROSS_OPENED        NO_PASS_TILE + 10
#define DOOR_ACROSS_CLOSING_1     22
#define DOOR_ACROSS_CLOSING_2     21
#define DOOR_ACROSS_CLOSED        14

#define DOOR_UP_OPEN_1            12
#define DOOR_UP_OPEN_2            13
#define DOOR_UP_OPENED            NO_PASS_TILE + 9
#define DOOR_UP_CLOSING_1         19
#define DOOR_UP_CLOSING_2         18
#define DOOR_UP_CLOSED            11

//-----------------------------------------------------------------------------
//
// structure used for doors
//
//-----------------------------------------------------------------------------

typedef struct
{
	int            tileIndex;
	float          height;
	float          width;
	int            currentFrame;            // which frame are we on
	float          frameDelay;                // speed to animate them at
	int            numberUsing;
	int            direction;

	bool inUse;
	b2Vec2         topLeft;
	b2Vec2         topRight;
	b2Vec2         botLeft;
	b2Vec2         botRight;

	b2Vec2         worldPosition  = {0, 0};
	b2Vec2         renderPosition = {0, 0};
	b2BodyDef      bodyDef;                      // Used for physics and collisions
	b2PolygonShape shape;
	b2FixtureDef   fixtureDef;
	b2Body         *body;
	_userData      *userData;
} _doorTrigger;

extern float distanceForDoorSoundMax;
extern double doorAnimSpeed;
extern std::vector<_doorTrigger> doorTriggers;

// Find the doors for this level and prepare a sensor object
void gam_doorTriggerSetup (const std::string deckName);

// Process all the doors that are currently inUse
void gam_doorProcessActions ();

// Check door trigger areas against sprite positions
void gam_doorCheckTriggerAreas ();

// Render current door frames onto map
void gam_renderDoorFrames ();