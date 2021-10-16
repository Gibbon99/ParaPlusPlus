#include <cassert>
#include <game/player.h>
#include <classes/paraBullet.h>
#include <system/util.h>
#include <system/gameEvents.h>
#include "game/physicsCollisions.h"

cpBool doWallCollisions = cpTrue;

cpCollisionHandler *handlerWallPlayer;
cpCollisionHandler *handlerDroidToPlayer;
cpCollisionHandler *handlerDroidToDroid;
cpCollisionHandler *handlerPlayerToLiftSensorBegin;
cpCollisionHandler *handlerPlayerToLiftSensorEnd;
cpCollisionHandler *handlerPlayerToHealingSensorBegin;
cpCollisionHandler *handlerPlayerToHealingSensorEnd;
cpCollisionHandler *handlerPlayerToTerminalSensorBegin;
cpCollisionHandler *handlerPlayerToTerminalSensorEnd;
cpCollisionHandler *handlerPlayerBulletToWall;
cpCollisionHandler *handlerDroidBulletToWall;
cpCollisionHandler *handlerPlayerBulletToDroid;
cpCollisionHandler *handlerDroidBulletToDroid;
cpCollisionHandler *handlerDroidBulletToPlayer;
cpCollisionHandler *handlerEnemyBulletToPlayerBullet;
cpCollisionHandler *handlerEnemyBulletToEnemyBullet;
cpCollisionHandler *handlerBulletToDoor;

cpCollisionHandler *handlerParticleToDroid;

//----------------------------------------------------------------------------------------------------------------------
//
// Post step remove to remove a object from the world
static void postStepRemove(cpSpace *space, cpShape *shape, void *unused)
//----------------------------------------------------------------------------------------------------------------------
{
	cpSpaceRemoveBody (space, cpShapeGetBody (shape));
	cpSpaceRemoveShape (space, shape);

	cpBodyFree (cpShapeGetBody (shape));
	cpShapeFree (shape);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set player vs wall collisions
void gam_setWallCollisions(bool newState)
//----------------------------------------------------------------------------------------------------------------------
{
	doWallCollisions = newState;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Collision between player and wall
// Used to ignore collisions for debugging
//
unsigned char handleCollisionPlayerWall(cpArbiter *arb, [[maybe_unused]]cpSpace *space, [[maybe_unused]]void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	return doWallCollisions;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Handle two droid colliding - could be DROID to PLAYER, or DROID to DROID
unsigned char handleCollisionDroidToDroid(cpArbiter *arb, [[maybe_unused]]cpSpace *space, [[maybe_unused]]void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	// Get the cpShapes involved in the collision
	//
	// The order is A = ENEMY and B = PLAYER if userData.dataType == -1
	//
	//  Or A = ENEMY and B = ENEMY

	cpShape       *a, *b;
	cpDataPointer dataPointerA, dataPointerB;

	cpArbiterGetShapes (arb, &a, &b);

	dataPointerA = cpShapeGetUserData (a);      // Enemy droid
	dataPointerB = cpShapeGetUserData (b);      // Player droid

	auto *userDataA = reinterpret_cast<_userData *>(dataPointerA);
	auto *userDataB = reinterpret_cast<_userData *>(dataPointerB);

	if (-1 == userDataB->dataValue)     // Player
	{
		if (playerDroid.getInTransferMode ())
		{
			cpBodySetVelocity (playerDroid.body, cpvzero);
			gam_setDroidVelocity (userDataA->dataValue, cpvzero);
			gam_addEvent (EVENT_ACTION_INIT_TRANSFER_MODE, 0, sys_getString ("%i|", userDataA->dataValue));
			return cpFalse;
		}

		if (gam_getDroidCurrentMode (userDataA->dataValue) == DROID_MODE_EXPLODING)      // Damage to player from exploding droid
		{
			gam_addEvent (EVENT_ACTION_DAMAGE_TO_DROID, 0, sys_getString ("%i|%i|%i", userDataB->dataValue, PHYSIC_DAMAGE_EXPLOSION, userDataA->dataValue));
			return cpFalse;
		}
	}

	//
	// Two Droids colliding
	if (gam_getDroidCurrentMode (userDataA->dataValue) == DROID_MODE_EXPLODING)
	{
		gam_addEvent (EVENT_ACTION_DAMAGE_TO_DROID, 0, sys_getString ("%i|%i|%i", userDataB->dataValue, PHYSIC_DAMAGE_EXPLOSION, userDataA->dataValue));
		return cpFalse;
	}

	//
	// Two Droids colliding
	if (gam_getDroidCurrentMode (userDataB->dataValue) == DROID_MODE_EXPLODING)
	{
		gam_addEvent (EVENT_ACTION_DAMAGE_TO_DROID, 0, sys_getString ("%i|%i|%i", userDataA->dataValue, PHYSIC_DAMAGE_EXPLOSION, userDataB->dataValue));
		return cpFalse;
	}

	if ((userDataA->ignoreCollisionDroid) || (userDataB->ignoreCollisionDroid))
		return cpFalse;

	if (userDataB->ignoreCollisionPlayer)
		return cpFalse;

	gam_addEvent (EVENT_ACTION_DAMAGE_TO_DROID, 0, sys_getString ("%i|%i|%i", userDataA->dataValue, PHYSIC_DAMAGE_BUMP, userDataB->dataValue));

	std::cout << "Droid [ " << userDataB->dataValue << " ] Collided with droid [ " << userDataA->dataValue << " ] " << std::endl;

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Player has left a lift sensor
void handleSensorPlayerToLiftEnd(cpArbiter *arb, [[maybe_unused]]cpSpace *space, [[maybe_unused]]void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	playerDroid.setOverLiftTile (false);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Player is over a lift sensor
unsigned char handleSensorPlayerToLiftBegin(cpArbiter *arb, [[maybe_unused]]cpSpace *space, [[maybe_unused]]void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	cpShape       *a, *b;
	cpDataPointer dataPointerB;

	cpArbiterGetShapes (arb, &a, &b);
	dataPointerB = cpShapeGetUserData (b);

	auto *userDataB = reinterpret_cast<_userData *>(dataPointerB);

	playerDroid.setLiftIndex (userDataB->dataValue);
	playerDroid.setOverLiftTile (true);

	return cpTrue;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Player has left a healing tile sensor
void handleSensorPlayerToHealingEnd(cpArbiter *arb, [[maybe_unused]]cpSpace *space, [[maybe_unused]]void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	playerDroid.setOverHealingTile (false);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Player is over a healing tile sensor
unsigned char handleSensorPlayerToHealingBegin(cpArbiter *arb, [[maybe_unused]]cpSpace *space, [[maybe_unused]]void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	playerDroid.setOverHealingTile (true);

	return cpTrue;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Player has left a terminal tile sensor
void handleSensorPlayerToTerminalEnd(cpArbiter *arb, [[maybe_unused]]cpSpace *space, [[maybe_unused]]void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	playerDroid.setOverTerminalTile (cpFalse);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Player is over a terminal tile sensor
unsigned char handleSensorPlayerToTerminalBegin(cpArbiter *arb, [[maybe_unused]]cpSpace *space, [[maybe_unused]]void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	playerDroid.setOverTerminalTile (cpTrue);

	return cpTrue;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Bullet hits a wall
unsigned char handleBulletToWall(cpArbiter *arb, [[maybe_unused]]cpSpace *space, [[maybe_unused]]void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	cpShape       *a, *b;
	cpDataPointer dataPointerA;

	cpArbiterGetShapes (arb, &a, &b);
	dataPointerA = cpShapeGetUserData (a);

	auto *userDataA = reinterpret_cast<_userData *>(dataPointerA);

	gam_addEvent (EVENT_ACTION_REMOVE_BULLET, 0, sys_getString ("%i|", userDataA->bulletID));
	auto renderPosition = bullets[gam_getArrayIndex (userDataA->bulletID)].worldPosInPixels;    // TODO Do this with a function, not a call into the array
	gam_addEvent (EVENT_ACTION_ADD_EMITTER, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, PARTICLE_TYPE_SPARK));
	gam_addEvent (EVENT_ACTION_ADD_LIGHTMAP, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, LIGHTMAP_TYPE_SPARK));

	cpSpaceAddPostStepCallback (space, reinterpret_cast<cpPostStepFunc>(postStepRemove), a, nullptr);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Handle a bullet hitting a bullet
unsigned char handleBulletToBullet(cpArbiter *arb, [[maybe_unused]]cpSpace *space, [[maybe_unused]]void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	cpShape       *a, *b;
	cpDataPointer dataPointerA;
	cpDataPointer dataPointerB;

	cpArbiterGetShapes (arb, &a, &b);
	dataPointerA = cpShapeGetUserData (a);
	dataPointerB = cpShapeGetUserData (b);

	auto *userDataA = reinterpret_cast<_userData *>(dataPointerA);
	auto *userDataB = reinterpret_cast<_userData *>(dataPointerB);

	auto renderPosition = bullets[gam_getArrayIndex (userDataA->bulletID)].worldPosInPixels;    // TODO Do this with a function, not a call into the array
	gam_addEvent (EVENT_ACTION_ADD_EMITTER, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, PARTICLE_TYPE_SPARK));
	gam_addEvent (EVENT_ACTION_ADD_LIGHTMAP, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, LIGHTMAP_TYPE_SPARK));

	gam_addEvent (EVENT_ACTION_REMOVE_BULLET, 0, sys_getString ("%i|", userDataA->bulletID));
	gam_addEvent (EVENT_ACTION_REMOVE_BULLET, 0, sys_getString ("%i|", userDataB->bulletID));

	cpSpaceAddPostStepCallback (space, reinterpret_cast<cpPostStepFunc>(postStepRemove), a, nullptr);
	cpSpaceAddPostStepCallback (space, reinterpret_cast<cpPostStepFunc>(postStepRemove), b, nullptr);

	return cpFalse;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Handle a bullet hitting a droid - either player or droid
unsigned char handleBulletToDroid(cpArbiter *arb, [[maybe_unused]]cpSpace *space, [[maybe_unused]]void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	cpShape       *a, *b;
	cpDataPointer dataPointerA;
	cpDataPointer dataPointerB;

	cpArbiterGetShapes (arb, &a, &b);
	dataPointerA = cpShapeGetUserData (a);  // a is the bullet - either player or another droids
	dataPointerB = cpShapeGetUserData (b);  // b is which droid

	auto *userDataA = reinterpret_cast<_userData *>(dataPointerA);
	auto *userDataB = reinterpret_cast<_userData *>(dataPointerB);

	if (userDataA->dataValue == userDataB->dataValue)
		return cpFalse;

	gam_addEvent (EVENT_ACTION_DAMAGE_TO_DROID, 0, sys_getString ("%i|%i|%i", userDataB->dataValue, PHYSIC_DAMAGE_BULLET, userDataA->dataValue));
	gam_addEvent (EVENT_ACTION_REMOVE_BULLET, 0, sys_getString ("%i|", userDataA->bulletID));
	cpSpaceAddPostStepCallback (space, reinterpret_cast<cpPostStepFunc>(postStepRemove), a, nullptr);

	printf ("Running bullet to droid.\n");

	return cpFalse;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Handle a bullet hitting a door
unsigned char handleBulletToDoor(cpArbiter *arb, [[maybe_unused]]cpSpace *space, [[maybe_unused]]void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	cpShape       *a, *b;
	cpDataPointer dataPointerA;

	cpArbiterGetShapes (arb, &a, &b);
	dataPointerA = cpShapeGetUserData (a);  // a is the bullet - either player or another droids

	auto *userDataA = reinterpret_cast<_userData *>(dataPointerA);

	auto renderPosition = bullets[gam_getArrayIndex (userDataA->bulletID)].worldPosInPixels;    // TODO Do this with a function, not a call into the array
	gam_addEvent (EVENT_ACTION_ADD_EMITTER, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, PARTICLE_TYPE_SPARK));
	gam_addEvent (EVENT_ACTION_ADD_LIGHTMAP, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, LIGHTMAP_TYPE_SPARK));

	gam_addEvent (EVENT_ACTION_REMOVE_BULLET, 0, sys_getString ("%i|", userDataA->bulletID));

	cpSpaceAddPostStepCallback (space, reinterpret_cast<cpPostStepFunc>(postStepRemove), a, nullptr);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup all the collision handlers
void sys_setupCollisionHandlers()
//----------------------------------------------------------------------------------------------------------------------
{
	//
	// Handle PLAYER hitting a WALL
	//
	handlerWallPlayer = cpSpaceAddCollisionHandler (sys_returnPhysicsWorld (), PHYSIC_TYPE_PLAYER, PHYSIC_TYPE_WALL);
	handlerWallPlayer->beginFunc = reinterpret_cast<cpCollisionBeginFunc>(handleCollisionPlayerWall);
	//
	// Collision between player and a droid
	//
	handlerDroidToPlayer = cpSpaceAddCollisionHandler (sys_returnPhysicsWorld (), PHYSIC_TYPE_ENEMY, PHYSIC_TYPE_PLAYER);
	handlerDroidToPlayer->beginFunc = reinterpret_cast<cpCollisionBeginFunc>(handleCollisionDroidToDroid);
	//
	// Collision between droid and droid
	//
	handlerDroidToDroid = cpSpaceAddCollisionHandler (sys_returnPhysicsWorld (), PHYSIC_TYPE_ENEMY, PHYSIC_TYPE_ENEMY);
	handlerDroidToDroid->beginFunc = reinterpret_cast<cpCollisionBeginFunc >(handleCollisionDroidToDroid);
	//
	// Player is over lift sensor
	handlerPlayerToLiftSensorBegin = cpSpaceAddCollisionHandler (sys_returnPhysicsWorld (), PHYSIC_TYPE_PLAYER, PHYSIC_TYPE_LIFT);
	handlerPlayerToLiftSensorBegin->beginFunc = reinterpret_cast<cpCollisionBeginFunc>(handleSensorPlayerToLiftBegin);
	//
	// Player has left a lift sensor
	handlerPlayerToLiftSensorEnd = cpSpaceAddCollisionHandler (sys_returnPhysicsWorld (), PHYSIC_TYPE_PLAYER, PHYSIC_TYPE_LIFT);
	handlerPlayerToLiftSensorEnd->separateFunc = reinterpret_cast<cpCollisionSeparateFunc>(handleSensorPlayerToLiftEnd);
	//
	// Player is over healing sensor
	handlerPlayerToHealingSensorBegin = cpSpaceAddCollisionHandler (sys_returnPhysicsWorld (), PHYSIC_TYPE_PLAYER, PHYSIC_TYPE_HEALING);
	handlerPlayerToHealingSensorBegin->beginFunc = reinterpret_cast<cpCollisionBeginFunc>(handleSensorPlayerToHealingBegin);
	//
	// Player has left a healing sensor
	handlerPlayerToHealingSensorEnd = cpSpaceAddCollisionHandler (sys_returnPhysicsWorld (), PHYSIC_TYPE_PLAYER, PHYSIC_TYPE_HEALING);
	handlerPlayerToHealingSensorEnd->separateFunc = reinterpret_cast<cpCollisionSeparateFunc>(handleSensorPlayerToHealingEnd);
	//
	// Player is over terminal sensor
	handlerPlayerToTerminalSensorBegin = cpSpaceAddCollisionHandler (sys_returnPhysicsWorld (), PHYSIC_TYPE_PLAYER, PHYSIC_TYPE_TERMINAL);
	handlerPlayerToTerminalSensorBegin->beginFunc = reinterpret_cast<cpCollisionBeginFunc>(handleSensorPlayerToTerminalBegin);
	//
	// Player has left a terminal sensor
	handlerPlayerToTerminalSensorEnd = cpSpaceAddCollisionHandler (sys_returnPhysicsWorld (), PHYSIC_TYPE_PLAYER, PHYSIC_TYPE_TERMINAL);
	handlerPlayerToTerminalSensorEnd->separateFunc = reinterpret_cast<cpCollisionSeparateFunc>(handleSensorPlayerToTerminalEnd);
	//
	// Player bullet hits a wall - remove it
	handlerPlayerBulletToWall = cpSpaceAddCollisionHandler (sys_returnPhysicsWorld (), PHYSIC_TYPE_BULLET_PLAYER, PHYSIC_TYPE_WALL);
	handlerPlayerBulletToWall->beginFunc = reinterpret_cast<cpCollisionBeginFunc>(handleBulletToWall);
	//
	// Droid bullet hits a wall - remove it
	handlerDroidBulletToWall = cpSpaceAddCollisionHandler (sys_returnPhysicsWorld (), PHYSIC_TYPE_BULLET_ENEMY, PHYSIC_TYPE_WALL);
	handlerDroidBulletToWall->beginFunc = reinterpret_cast<cpCollisionBeginFunc>(handleBulletToWall);
	//
	// Player bullet hits a droid
	handlerPlayerBulletToDroid = cpSpaceAddCollisionHandler (sys_returnPhysicsWorld (), PHYSIC_TYPE_BULLET_PLAYER, PHYSIC_TYPE_ENEMY);
	handlerPlayerBulletToDroid->beginFunc = reinterpret_cast<cpCollisionBeginFunc>(handleBulletToDroid);
	//
	// Droid bullet hits a droid
	handlerDroidBulletToDroid = cpSpaceAddCollisionHandler (sys_returnPhysicsWorld (), PHYSIC_TYPE_BULLET_ENEMY, PHYSIC_TYPE_ENEMY);
	handlerDroidBulletToDroid->beginFunc = reinterpret_cast<cpCollisionBeginFunc>(handleBulletToDroid);
	//
	// Droid bullet hits the player
	handlerDroidBulletToPlayer = cpSpaceAddCollisionHandler (sys_returnPhysicsWorld (), PHYSIC_TYPE_BULLET_ENEMY, PHYSIC_TYPE_PLAYER);
	handlerDroidBulletToPlayer->beginFunc = reinterpret_cast<cpCollisionBeginFunc>(handleBulletToDroid);
	//
	// Enemy bullet to player bullet collision
	handlerEnemyBulletToPlayerBullet = cpSpaceAddCollisionHandler (sys_returnPhysicsWorld (), PHYSIC_TYPE_BULLET_ENEMY, PHYSIC_TYPE_BULLET_PLAYER);
	handlerEnemyBulletToPlayerBullet->beginFunc = reinterpret_cast<cpCollisionBeginFunc>(handleBulletToBullet);
	//
	// Enemy bullet to enemy bullet collision
	handlerEnemyBulletToEnemyBullet = cpSpaceAddCollisionHandler (sys_returnPhysicsWorld (), PHYSIC_TYPE_BULLET_ENEMY, PHYSIC_TYPE_BULLET_ENEMY);
	handlerEnemyBulletToEnemyBullet->beginFunc = reinterpret_cast<cpCollisionBeginFunc>(handleBulletToBullet);
	//
	// Player bullet hitting a closed door
	handlerBulletToDoor = cpSpaceAddCollisionHandler (sys_returnPhysicsWorld (), PHYSIC_TYPE_BULLET_PLAYER, PHYSIC_TYPE_DOOR_CLOSED);
	handlerBulletToDoor->beginFunc = reinterpret_cast<cpCollisionBeginFunc>(handleBulletToDoor);
	//
	// Enemy bullet hitting a closed door
	handlerBulletToDoor = cpSpaceAddCollisionHandler (sys_returnPhysicsWorld (), PHYSIC_TYPE_BULLET_ENEMY, PHYSIC_TYPE_DOOR_CLOSED);
	handlerBulletToDoor->beginFunc = reinterpret_cast<cpCollisionBeginFunc>(handleBulletToDoor);




	//
	// Stop particles affecting droids
//	handlerParticleToDroid = cpSpaceAddCollisionHandler (sys_returnPhysicsWorld (), PHYSIC_TYPE_PARTICLE, PHYSIC_TYPE_PLAYER);

}