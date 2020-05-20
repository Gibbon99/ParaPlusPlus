#pragma once

#include "../main.h"
#include "box2d.h"
#include "game/physicsCollisions.h"

//#define DEBUG_PHYSICS    1

enum physicObjects
{
	PHYSIC_TYPE_WALL          = 1,
	PHYSIC_TYPE_ENEMY         = 2,
	PHYSIC_TYPE_PLAYER        = 4,
	PHYSIC_TYPE_BULLET_PLAYER = 8,
	PHYSIC_TYPE_BULLET_ENEMY  = 16,
	PHYSIC_TYPE_DOOR_OPEN     = 32,
	PHYSIC_TYPE_DOOR_CLOSED   = 64,
	PHYSIC_TYPE_PARTICLE      = 128,
	PHYSIC_TYPE_LIFT          = 256,
	PHYSIC_TYPE_DOOR_BULLET   = 512,
	PHYSIC_TYPE_HEALING       = 1024,
	PHYSIC_TYPE_TERMINAL      = 2048
};

struct _userData
{
	int userType;
	int dataValue;
	int wallIndexValue;
};

extern bool   d_showPhysics;
extern double gravity;         // Set from script

// Setup Physics engine - run once
bool sys_setupPhysicsEngine ();

// Return if the physics engine is ready to use
bool sys_isPhysicsReady ();

// Setup client player droid physics information
void sys_setupPlayerPhysics ();

// Step the world
void sys_stepPhysicsWorld (float stepAmount);

// Set the player physics position in the world
void sys_setPlayerPhysicsPosition (b2Vec2 newPosition);

// Process a frame of physics - called from Fixed Update
void sys_processPhysics (double tickTime);

// Return a pointer to the current physics world
b2World *sys_getPhysicsWorld();

// Create the solid walls for this level
void sys_setupSolidWalls (const std::string levelName);

void sys_freePhysicsEngine ();

// Create the physics bodies and shapes for the enemy droids
void sys_setupEnemyPhysics (std::string levelName);