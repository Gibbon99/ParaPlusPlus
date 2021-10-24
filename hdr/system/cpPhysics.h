#pragma once

#include "main.h"
#include "game/doors.h"

enum physicObjects
{
	PHYSIC_TYPE_PLAYER        = 1,
	PHYSIC_TYPE_ENEMY         = 2,
	PHYSIC_TYPE_BULLET_PLAYER = 4,
	PHYSIC_TYPE_BULLET_ENEMY  = 8,
	PHYSIC_TYPE_WALL          = 16,
	PHYSIC_TYPE_PARTICLE      = 32,

	PHYSIC_TYPE_DOOR_OPEN   = 64,
	PHYSIC_TYPE_DOOR_CLOSED = 128,
	PHYSIC_TYPE_LIFT        = 256,
	PHYSIC_TYPE_DOOR_BULLET = 512,
	PHYSIC_TYPE_HEALING     = 1024,
	PHYSIC_TYPE_TERMINAL    = 2048
};

extern cpShapeFilter FILTER_CAT_PLAYER;
extern cpShapeFilter FILTER_CAT_ENEMY;
extern cpShapeFilter FILTER_CAT_PLAYER_BULLET;
extern cpShapeFilter FILTER_CAT_ENEMY_BULLET;
extern cpShapeFilter FILTER_CAT_WALL;
extern cpShapeFilter FILTER_CAT_PARTICLE;

extern cpShapeFilter FILTER_PARTICLE_WALL;

extern double gravity;         // Set from script
extern bool   d_showPhysics;

// Create the world physics
void sys_createWorldPhysics();

// Get the state of the physics world
bool sys_getWorldSpaceState();

// Step the world
void sys_stepPhysicsWorld(float stepAmount);

// Setup client player droid physics information
void sys_setupPlayerPhysics();

// Set the player physics position in the world
void sys_setPlayerPhysicsPosition(cpVect newPosition);

// Create the solid walls for this level
void sys_setupSolidWalls(const std::string &levelName);

// Free memory for solid walls and free bodies
void sys_clearSolidWalls();

// Create the physics bodies and shapes for the enemy droids
void sys_setupDroidPhysics(const std::string &levelName);

// Clear previous level - remove physics objects from droid before changing level name
void sys_clearDroidPhysics(const std::string &levelName);

// Process a frame of physics - called from Fixed Update
void sys_processPhysics();

// Cleanup Physics engine - run once
void sys_freePhysicsEngine();

// Setup the colors and debug drawing functions
void d_setupDebugDrawing();

// Call the debugDrawing function
void d_drawPhysicsDebug();

// Create a door bullet sensor
void sys_createDoorBulletSensor(unsigned long whichDoor);

// Change the collision filters for a door
// Used by bullet to see if it passes through an open door
void gam_changeDoorFilters(int doorState, int whichDoor);

// Clear out memory for door triggers
void sys_clearAllDoors();

// Add a new door sensor to the array - return the new array size
unsigned long sys_addNewDoorSensor(const _doorTrigger &newDoorTrigger);

// Return the physics worldSpace
cpSpace *sys_returnPhysicsWorld();