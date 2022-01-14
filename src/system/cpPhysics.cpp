#include <memory>
#include "system/shutdown.h"
#include "system/cpPhysics.h"
#include "system/util.h"
#include "system/cpPhysicsDebug.h"
#include "game/player.h"
#include "game/shipDecks.h"
#include "game/physicsCollisions.h"
#include "io/logFile.h"

double  gravity;         // Set from script
cpSpace *worldSpace;
bool    physicsStarted = false;
bool    d_showPhysics  = false;

cpFloat playerMass {0.1f};
cpFloat playerRadius {12};
cpFloat playerElastic (0.0);
cpVect  shapeOffset {};

cpFloat droidMass {0.1f};
cpFloat droidRadius {12};
cpFloat droidElastic (0.0);
cpFloat droidFriction {};

cpFloat wallFricton {};
cpFloat wallRadius {3.0f};

struct _physicWall
{
	cpBody                     *body {nullptr};
	cpShape                    *shape {nullptr};
	std::shared_ptr<_userData> userData {};
};

std::vector<_physicWall>  solidWalls;
std::vector<doorTrigger_> doorBulletSensor;

cpSpaceDebugDrawOptions drawOptions;

cpShapeFilter FILTER_CAT_PLAYER;
cpShapeFilter FILTER_CAT_ENEMY;
cpShapeFilter FILTER_CAT_PLAYER_BULLET;
cpShapeFilter FILTER_CAT_ENEMY_BULLET;
cpShapeFilter FILTER_CAT_WALL;
cpShapeFilter FILTER_CAT_PARTICLE;
cpShapeFilter FILTER_CAT_DOOR_OPEN;
cpShapeFilter FILTER_CAT_DOOR_CLOSED;

//----------------------------------------------------------------------------------------------------------------------
//
// Setup the colors and debug drawing functions
void d_setupDebugDrawing()
//----------------------------------------------------------------------------------------------------------------------
{
	drawOptions.drawCircle     = debugDrawCircle;
	drawOptions.drawSegment    = debugDrawSegment;
	drawOptions.drawFatSegment = debugDrawFatSegment;
	drawOptions.drawPolygon    = debugDrawPolygon;
	drawOptions.drawDot        = debugDrawDot;

	drawOptions.flags = (cpSpaceDebugDrawFlags) (CP_SPACE_DEBUG_DRAW_SHAPES | CP_SPACE_DEBUG_DRAW_CONSTRAINTS | CP_SPACE_DEBUG_DRAW_COLLISION_POINTS);

	drawOptions.shapeOutlineColor   = cpSpaceDebugColor {238, 232, 213, 255};
	drawOptions.constraintColor     = cpSpaceDebugColor {0, 200, 0, 255};
	drawOptions.collisionPointColor = cpSpaceDebugColor {250, 000, 000, 250};
	drawOptions.colorForShape       = d_colorForShape;

	drawOptions.data = nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Call the debugDrawing function
void d_drawPhysicsDebug()
//----------------------------------------------------------------------------------------------------------------------
{
	cpSpaceDebugDraw (worldSpace, &drawOptions);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the physics worldSpace
cpSpace *sys_returnPhysicsWorld()
//----------------------------------------------------------------------------------------------------------------------
{
	return worldSpace;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the world physics
void sys_createWorldPhysics()
//----------------------------------------------------------------------------------------------------------------------
{
	worldSpace = cpSpaceNew ();

	cpSpaceSetGravity (worldSpace, cpVect {gravity, gravity});
	cpSpaceSetDamping (worldSpace, 0.0f);

	sys_setupCollisionHandlers ();

	d_setupDebugDrawing ();

	FILTER_CAT_PLAYER.group      = CP_NO_GROUP;
	FILTER_CAT_PLAYER.mask       = PHYSIC_TYPE_ENEMY | PHYSIC_TYPE_BULLET_ENEMY | PHYSIC_TYPE_WALL | PHYSIC_TYPE_DOOR_CLOSED;
	FILTER_CAT_PLAYER.categories = PHYSIC_TYPE_PLAYER;

	FILTER_CAT_ENEMY.group      = CP_NO_GROUP;
	FILTER_CAT_ENEMY.mask       = PHYSIC_TYPE_PLAYER | PHYSIC_TYPE_BULLET_PLAYER | PHYSIC_TYPE_BULLET_ENEMY | PHYSIC_TYPE_WALL;
	FILTER_CAT_ENEMY.categories = PHYSIC_TYPE_ENEMY;

	FILTER_CAT_PLAYER_BULLET.group      = CP_NO_GROUP;
	FILTER_CAT_PLAYER_BULLET.mask       = PHYSIC_TYPE_PLAYER | PHYSIC_TYPE_ENEMY | PHYSIC_TYPE_BULLET_ENEMY | PHYSIC_TYPE_WALL | PHYSIC_TYPE_DOOR_CLOSED;
	FILTER_CAT_PLAYER_BULLET.categories = PHYSIC_TYPE_BULLET_PLAYER;

	FILTER_CAT_ENEMY_BULLET.group      = CP_NO_GROUP;
	FILTER_CAT_ENEMY_BULLET.mask       = PHYSIC_TYPE_PLAYER | PHYSIC_TYPE_ENEMY | PHYSIC_TYPE_BULLET_PLAYER | PHYSIC_TYPE_BULLET_ENEMY | PHYSIC_TYPE_WALL;
	FILTER_CAT_ENEMY_BULLET.categories = PHYSIC_TYPE_BULLET_ENEMY;

	FILTER_CAT_WALL.group      = CP_NO_GROUP;
	FILTER_CAT_WALL.mask       = PHYSIC_TYPE_PLAYER | PHYSIC_TYPE_ENEMY | PHYSIC_TYPE_BULLET_PLAYER | PHYSIC_TYPE_BULLET_ENEMY | PHYSIC_TYPE_PARTICLE;
	FILTER_CAT_WALL.categories = PHYSIC_TYPE_WALL;

	FILTER_CAT_PARTICLE.group      = CP_NO_GROUP;
	FILTER_CAT_PARTICLE.mask       = PHYSIC_TYPE_WALL;
	FILTER_CAT_PARTICLE.categories = PHYSIC_TYPE_PARTICLE;

	FILTER_CAT_DOOR_OPEN.group      = CP_NO_GROUP;
	FILTER_CAT_DOOR_OPEN.mask       = PHYSIC_TYPE_PLAYER;
	FILTER_CAT_DOOR_OPEN.categories = PHYSIC_TYPE_DOOR_OPEN;

	FILTER_CAT_DOOR_CLOSED.group      = CP_NO_GROUP;
	FILTER_CAT_DOOR_CLOSED.mask       = PHYSIC_TYPE_PLAYER | PHYSIC_TYPE_ENEMY | PHYSIC_TYPE_BULLET_PLAYER | PHYSIC_TYPE_BULLET_ENEMY;
	FILTER_CAT_DOOR_CLOSED.categories = PHYSIC_TYPE_DOOR_CLOSED;

	physicsStarted = true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Cleanup Physics engine - run once
void sys_freePhysicsEngine()
//----------------------------------------------------------------------------------------------------------------------
{
	if (nullptr != worldSpace)
	{
		sys_clearSolidWalls ();
		cpSpaceDestroy (worldSpace);
		worldSpace = nullptr;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the state of the physics world
bool sys_getWorldSpaceState()
//----------------------------------------------------------------------------------------------------------------------
{
	return physicsStarted;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Step the world
void sys_stepPhysicsWorld(float stepAmount)
//----------------------------------------------------------------------------------------------------------------------
{
	cpSpaceStep (worldSpace, stepAmount);
}

//-------------------------------------------------------------------
//
// Setup client player droid physics information
void sys_setupPlayerPhysics()
//-------------------------------------------------------------------
{
	if (!physicsStarted)
		sys_shutdownWithError ("Attempting to configure player droid physics, but engine not started.");

	if (playerDroid.body != nullptr)    // Physics already setup
		return;

	playerDroid.body = cpSpaceAddBody (worldSpace, cpBodyNew (playerMass, cpMomentForCircle (playerMass, 0.0f, playerRadius, shapeOffset)));
	cpBodySetMass (playerDroid.body, playerMass);
	cpBodySetVelocity (playerDroid.body, cpVect {0, 0});

	playerDroid.shape = cpSpaceAddShape (worldSpace, cpCircleShapeNew (playerDroid.body, playerRadius, shapeOffset));
	cpShapeSetFriction (playerDroid.shape, playerFriction);
	cpShapeSetElasticity (playerDroid.shape, playerElastic);
	cpShapeSetCollisionType (playerDroid.shape, PHYSIC_TYPE_PLAYER);
	cpShapeSetFilter (playerDroid.shape, FILTER_CAT_PLAYER);

	playerDroid.userData = std::make_shared<_userData> ();

	playerDroid.userData->bulletID              = -1;
	playerDroid.userData->userType              = cpShapeGetCollisionType (playerDroid.shape);
	playerDroid.userData->dataValue             = TARGET_PLAYER;
	playerDroid.userData->ignoreCollisionDroid  = false;
	playerDroid.userData->ignoreCollisionPlayer = false;

	cpShapeSetUserData (playerDroid.shape, playerDroid.userData.get ());
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the physics bodies and shapes for the enemy droids
void sys_setupDroidPhysics(const std::string &levelName)
//----------------------------------------------------------------------------------------------------------------------
{
#ifdef MY_DEBUG
	log_addEvent (sys_getString ("[ %s ] Setting up enemy physics for level[ %s ].", __func__, levelName.c_str ()));
#endif

	if (!physicsStarted)
		sys_shutdownWithError (sys_getString ("Attempting to setup droid physics with no engine."));

	if (shipdecks.at (levelName).deckIsDead)   // No droids alive
	{
#ifdef MY_DEBUG
		log_addEvent (sys_getString ("[ %s ] Level [ %s ] is dead - no physics to create.", __func__, levelName.c_str ()));
#endif
		return;
	}

	if (shipdecks.at (levelName).droidPhysicsCreated)
	{
#ifdef MY_DEBUG
		log_addEvent (sys_getString ("[ %s ] Droid physics already created for level [ %s ].", __func__, levelName.c_str ()));
#endif
		return;
	}

	for (auto &droidItr: shipdecks.at (levelName).droid)
	{
		if (droidItr.getCurrentMode () == DROID_MODE_NORMAL)
		{
			if (droidItr.body == nullptr)
			{
				droidItr.body = cpSpaceAddBody (worldSpace, cpBodyNew (droidMass, cpMomentForCircle (droidMass, 0.0f, droidRadius, shapeOffset)));
				cpBodySetMass (droidItr.body, droidMass);
				cpBodySetPosition (droidItr.body, droidItr.getWorldPosInPixels ());

				droidItr.shape = cpSpaceAddShape (worldSpace, cpCircleShapeNew (droidItr.body, droidRadius, shapeOffset));
				cpShapeSetFriction (droidItr.shape, droidFriction);
				cpShapeSetElasticity (droidItr.shape, droidElastic);
				cpShapeSetCollisionType (droidItr.shape, PHYSIC_TYPE_ENEMY);
				cpShapeSetFilter (droidItr.shape, FILTER_CAT_ENEMY);

				droidItr.userData                        = std::make_shared<_userData> ();
				droidItr.userData->bulletID              = -1;
				droidItr.userData->userType              = cpShapeGetCollisionType (droidItr.shape);
				droidItr.userData->dataValue             = droidItr.ai2.getArrayIndex ();
				droidItr.userData->ignoreCollisionDroid  = true;
				droidItr.userData->ignoreCollisionPlayer = true;

				cpShapeSetUserData (droidItr.shape, droidItr.userData.get ());
			}
		}
	}
	shipdecks.at (levelName).droidPhysicsCreated = true;
#ifdef MY_DEBUG
	log_addEvent (sys_getString ("[ %s ] Droid physics created for level [ %s ]", __func__, levelName.c_str ()));
#endif
}

//-------------------------------------------------------------------
//
// Set the player physics position in the world
void sys_setPlayerPhysicsPosition(cpVect newPosition)
//-------------------------------------------------------------------
{
	if (playerDroid.body == nullptr)
		return;

	cpBodySetPosition (playerDroid.body, newPosition);
}

//-------------------------------------------------------------------------------------------------------------
//
// Clear previous level - remove physics objects from droid before changing level name
void sys_clearDroidPhysics(const std::string &levelName)
//-------------------------------------------------------------------------------------------------------------
{
	if (levelName.empty ())      // First run - no level set as yet
		return;

	for (auto &droidItr: shipdecks.at (levelName).droid)
	{
		if (nullptr != droidItr.shape)
		{
			cpSpaceRemoveShape (worldSpace, droidItr.shape);
			cpShapeFree (droidItr.shape);
			droidItr.shape = nullptr;
		}

		if (nullptr != droidItr.body)
		{
			cpSpaceRemoveBody (worldSpace, droidItr.body);
			cpBodyFree (droidItr.body);
			droidItr.body = nullptr;
		}
	}
	shipdecks.at (levelName).droidPhysicsCreated = false;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Free memory for solid walls and free bodies
void sys_clearSolidWalls()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &wallItr: solidWalls)
	{
		if (nullptr != wallItr.shape)
		{
			cpSpaceRemoveShape (worldSpace, wallItr.shape);
			cpShapeFree (wallItr.shape);
			wallItr.shape = nullptr;
		}

		if (nullptr != wallItr.body)
		{
			wallItr.body = nullptr;
		}
	}
	solidWalls.clear ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the solid walls for this level
void sys_setupSolidWalls(const std::string &levelName)
//----------------------------------------------------------------------------------------------------------------------
{
	cpVect              wallStart, wallFinish;
	std::vector<cpVect> wallPositions;
	_physicWall         tempWall;

	if (0 == shipdecks.at (levelName).numLineSegments)
		return;

	if (!solidWalls.empty ())
	{
		sys_clearSolidWalls ();
		shipdecks.at (levelName).wallPhysicsCreated = false;
	}

	for (auto i = 0; i != shipdecks.at (levelName).numLineSegments; i++)
	{
		wallStart.x = (shipdecks.at (levelName).lineSegments[i].start.x - (tileSize / 2));
		wallStart.y = (shipdecks.at (levelName).lineSegments[i].start.y - (tileSize / 2));

		wallFinish.x = (shipdecks.at (levelName).lineSegments[i].finish.x - (tileSize / 2));
		wallFinish.y = (shipdecks.at (levelName).lineSegments[i].finish.y - (tileSize / 2));

		tempWall.body = cpBodyNewStatic ();

		tempWall.shape = cpSegmentShapeNew (tempWall.body, wallStart, wallFinish, wallRadius);
		solidWalls.push_back (tempWall);

		cpShapeSetFriction (solidWalls[i].shape, wallFricton);
		cpShapeSetCollisionType (solidWalls[i].shape, PHYSIC_TYPE_WALL);
		cpShapeSetFilter (solidWalls[i].shape, FILTER_CAT_WALL);

		tempWall.userData                        = std::make_shared<_userData> ();
		tempWall.userData->bulletID              = -1;
		tempWall.userData->userType              = cpShapeGetCollisionType (tempWall.shape);
		tempWall.userData->dataValue             = i;
		tempWall.userData->ignoreCollisionDroid  = true;
		tempWall.userData->ignoreCollisionPlayer = true;
		cpShapeSetUserData (tempWall.shape, tempWall.userData.get ());

		cpSpaceAddShape (worldSpace, solidWalls[i].shape);
		wallPositions.push_back (wallStart);
	}
	shipdecks.at (levelName).wallPhysicsCreated = true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Add a new door sensor to the array - return the new array size
unsigned long sys_addNewDoorSensor(const doorTrigger_ &newDoorTrigger)
//----------------------------------------------------------------------------------------------------------------------
{
	doorBulletSensor.push_back (newDoorTrigger);
	return doorBulletSensor.size () - 1;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Clear out memory for door triggers
void sys_clearAllDoors()
//----------------------------------------------------------------------------------------------------------------------
{
	doorTriggers.clear ();

	for (auto &doorBulletItr: doorBulletSensor)
	{
		if (nullptr != doorBulletItr.shape)
		{
			cpSpaceRemoveShape (sys_returnPhysicsWorld (), doorBulletItr.shape);
			cpShapeFree (doorBulletItr.shape);
			doorBulletItr.shape = nullptr;
		}

		if (nullptr != doorBulletItr.body)
		{
			cpSpaceRemoveBody (sys_returnPhysicsWorld (), doorBulletItr.body);
			cpBodyFree (doorBulletItr.body);
			doorBulletItr.body = nullptr;
		}
	}
	doorBulletSensor.clear ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create a door bullet sensor
void sys_createDoorBulletSensor(unsigned long whichDoor)
//----------------------------------------------------------------------------------------------------------------------
{
	doorBulletSensor[whichDoor].body = cpBodyNewStatic ();

	cpSpaceAddBody (worldSpace, doorBulletSensor[whichDoor].body);
	cpBodySetPosition (doorBulletSensor[whichDoor].body, doorBulletSensor[whichDoor].worldPosition);

	doorBulletSensor[whichDoor].shape = cpBoxShapeNew (doorBulletSensor[whichDoor].body, doorBulletSensor[whichDoor].width, doorBulletSensor[whichDoor].height, 3.0f);
	cpShapeSetFriction (doorBulletSensor[whichDoor].shape, 0.5f);
	cpShapeSetCollisionType (doorBulletSensor[whichDoor].shape, PHYSIC_TYPE_DOOR_CLOSED);
	cpShapeSetFilter (doorBulletSensor[whichDoor].shape, FILTER_CAT_DOOR_CLOSED);

	doorBulletSensor[whichDoor].userData            = std::make_shared<_userData> ();
	doorBulletSensor[whichDoor].userData->userType  = cpShapeGetCollisionType (doorBulletSensor[whichDoor].shape);
	doorBulletSensor[whichDoor].userData->dataValue = whichDoor;
	cpShapeSetUserData (doorBulletSensor[whichDoor].shape, doorBulletSensor[whichDoor].userData.get ());

	cpSpaceAddShape (sys_returnPhysicsWorld (), doorBulletSensor[whichDoor].shape);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Change the collision filters for a door
// Used by bullet to see if it passes through an open door
void gam_changeDoorFilters(int doorState, int whichDoor)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (doorState)
	{
		case DOOR_ACROSS_OPENED:
		case DOOR_UP_OPENED:
			cpShapeSetCollisionType (doorBulletSensor[whichDoor].shape, PHYSIC_TYPE_DOOR_OPEN);
			cpShapeSetFilter (doorBulletSensor[whichDoor].shape, FILTER_CAT_DOOR_OPEN);
			break;

		default:
			cpShapeSetCollisionType (doorBulletSensor[whichDoor].shape, PHYSIC_TYPE_DOOR_CLOSED);
			cpShapeSetFilter (doorBulletSensor[whichDoor].shape, FILTER_CAT_DOOR_CLOSED);
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------
//
// Process a frame of physics - called from Fixed Update
void sys_processPhysics()
//----------------------------------------------------------------------------------------------------------------
{
	if (playerDroid.body != nullptr)
	{
		playerDroid.ai2.setPreviousPosInPixels (playerDroid.getWorldPosInPixels ());
		cpBodyApplyForceAtLocalPoint (playerDroid.body, playerDroid.getVelocity (), cpVect {0, 0});
	}

	for (auto &droidItr: shipdecks.at (gam_getCurrentDeckName ()).droid)
	{
		if (droidItr.getCurrentMode () == DROID_MODE_NORMAL)
		{
			droidItr.ai2.setPreviousPosInPixels (droidItr.getWorldPosInPixels ());
			if (droidItr.body != nullptr)
				cpBodyApplyForceAtLocalPoint (droidItr.body, droidItr.ai2.getVelocity (), cpVect {0, 0});
		}
	}

	sys_stepPhysicsWorld (1.0f / TICKS_PER_SECOND);

	playerDroid.setWorldPosInPixels (cpBodyGetPosition (playerDroid.body));
	cpBodySetAngle (playerDroid.body, 0); // Remove rotation

	if ((g_shipDeckItr->second.deckIsDead) || (!g_shipDeckItr->second.droidPhysicsCreated))
		return;

	for (auto &droidItr: shipdecks.at (gam_getCurrentDeckName ()).droid)
	{
		if (droidItr.getCurrentMode () == DROID_MODE_NORMAL)
		{
			droidItr.setWorldPosInPixels (cpBodyGetPosition (droidItr.body));
			cpBodySetAngle (droidItr.body, 0);     // Remove rotation about center
		}
	}
}