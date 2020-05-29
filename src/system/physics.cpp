#include <game/player.h>
#include <game/shipDecks.h>
#include <system/util.h>
#include "system/physics.h"

struct __physicWall
{
	b2BodyDef    bodyDef;
	b2Body       *body = nullptr;
	b2EdgeShape  shape;
	b2FixtureDef fixture;
	_userData    *userData;
};

std::vector<__physicWall> solidWalls;
b2World                   *physicsWorld;
bool                      d_showPhysics      = false;
bool                      physicsStarted     = true;
int32                     velocityIterations = 6;   //how strongly to correct velocity
int32                     positionIterations = 3;   //how strongly to correct position
double                    gravity;         // Set from script

//----------------------------------------------------------------------------------------------------------------
//
// Return a pointer to the current physics world
b2World *sys_getPhysicsWorld ()
//----------------------------------------------------------------------------------------------------------------
{
	return physicsWorld;
}

//----------------------------------------------------------------------------------------------------------------
//
// Process a frame of physics - called from Fixed Update
void sys_processPhysics (double tickTime)
//----------------------------------------------------------------------------------------------------------------
{
//	bul_applyPhysics (tickTime);
	playerDroid.body->ApplyForce (playerDroid.velocity, playerDroid.body->GetWorldCenter (), true); // TODO do this way - bulletItr.velocity.operator*= (bulletMoveSpeed);

	sys_stepPhysicsWorld (1.0f / TICKS_PER_SECOND);

	playerDroid.previousWorldPosInPixels = playerDroid.worldPosInPixels;
	playerDroid.worldPosInPixels         = playerDroid.body->GetPosition ();        // GetPosition is in meters

	playerDroid.worldPosInPixels.x *= static_cast<float>(pixelsPerMeter);           // Change to pixels
	playerDroid.worldPosInPixels.y *= static_cast<float>(pixelsPerMeter);

	if (g_shipDeckItr->second.droidPhysicsCreated)
	{
		for (auto &droidItr : g_shipDeckItr->second.droid)
		{
			droidItr.previousWorldPosInPixels = droidItr.worldPosInPixels;
			droidItr.worldPosInPixels = droidItr.body->GetPosition();       // In Meters
			droidItr.worldPosInPixels.x *= static_cast<float>(pixelsPerMeter);  // Change to pixels for rendering
			droidItr.worldPosInPixels.y *= static_cast<float>(pixelsPerMeter);
			droidItr.body->SetLinearVelocity({ 0, 0 });
		}
		//	gam_processPhysicActions ();
		playerDroid.body->SetLinearVelocity({ 0, 0 });
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return if the physics engine is ready to use
bool sys_isPhysicsReady ()
//----------------------------------------------------------------------------------------------------------------------
{
	return physicsStarted;
}

//-------------------------------------------------------------------
//
// Set the player physics position in the world
void sys_setPlayerPhysicsPosition (b2Vec2 newPosition)
//-------------------------------------------------------------------
{
	if (playerDroid.body == nullptr)
	{
		return;
	}
	newPosition.x /= pixelsPerMeter;
	newPosition.y /= pixelsPerMeter;
	playerDroid.body->SetTransform (newPosition, playerDroid.body->GetAngle ());
}

//----------------------------------------------------------------------------------------------------------------------
//
// Step the world
void sys_stepPhysicsWorld (float stepAmount)
//----------------------------------------------------------------------------------------------------------------------
{
	physicsWorld->Step (stepAmount, velocityIterations, positionIterations);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup Physics engine - run once
bool sys_setupPhysicsEngine ()
//----------------------------------------------------------------------------------------------------------------------
{
	// Define the gravityVector vector.
	b2Vec2 gravityVector (0.0f, 0.0f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	physicsWorld = new b2World (gravityVector);

	physicsWorld->SetContactListener (&myContactListenerInstance);
	if (d_showPhysics)
	{
		g_paraDebugDraw.SetFlags (b2Draw::e_shapeBit);  // Draw shapes
		physicsWorld->SetDebugDraw (&g_paraDebugDraw);
	}

	physicsStarted = true;

	return true;
}

//-------------------------------------------------------------------
//
// Setup client player droid physics information
void sys_setupPlayerPhysics ()
//-------------------------------------------------------------------
{
	if (!physicsStarted)
		sys_shutdownWithError ("Attempting to configure player droid physics, but engine not started.");

	playerDroid.bodyDef.type = b2_dynamicBody;
	playerDroid.bodyDef.position.Set (2, 3);
	playerDroid.bodyDef.angle = 0;
	playerDroid.body          = physicsWorld->CreateBody (&playerDroid.bodyDef);

	playerDroid.userData                  = new _userData;
	playerDroid.userData->userType        = PHYSIC_TYPE_PLAYER;
	playerDroid.userData->dataValue       = -1;
	playerDroid.userData->ignoreCollision = false;
	playerDroid.body->SetUserData (playerDroid.userData);

	playerDroid.shape.m_radius = (SPRITE_SIZE / 2) / pixelsPerMeter;
	playerDroid.shape.m_p.Set (0, 0);

	playerDroid.fixtureDef.shape               = &playerDroid.shape;
	playerDroid.fixtureDef.density             = 1.0f;
	playerDroid.fixtureDef.friction            = static_cast<float>(playerFriction);
	playerDroid.fixtureDef.restitution         = 1.0f;
	playerDroid.fixtureDef.filter.categoryBits = PHYSIC_TYPE_PLAYER;
	playerDroid.fixtureDef.filter.maskBits     = PHYSIC_TYPE_WALL | PHYSIC_TYPE_BULLET_ENEMY | PHYSIC_TYPE_DOOR_CLOSED | PHYSIC_TYPE_ENEMY;
	playerDroid.body->CreateFixture (&playerDroid.fixtureDef);

	playerDroid.velocity.x = 0.0f;
	playerDroid.velocity.y = 0.0f;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Free memory for solid walls and free bodies
void sys_clearSolidWalls ()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &wallItr : solidWalls)
	{
		if (wallItr.userData != nullptr)
		{
			delete (wallItr.userData);
			wallItr.userData = nullptr;
		}
		
		if (wallItr.body != nullptr)
		{
			wallItr.body->GetWorld()->DestroyBody(wallItr.body);
			wallItr.body = nullptr;
		}
	}
	solidWalls.clear ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the solid walls for this level
void sys_setupSolidWalls (const std::string levelName)
//----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2              wallStart, wallFinish;
	std::vector<b2Vec2> wallPositions;
	__physicWall        tempWall;

	if (0 == shipdecks.at (levelName).numLineSegments)
		return;

	if (!solidWalls.empty ())
		sys_clearSolidWalls ();

	for (auto i = 0; i != shipdecks.at (levelName).numLineSegments; i++)
	{
		wallStart.x = (shipdecks.at (levelName).lineSegments[i].start.x - (tileSize / 2)) / pixelsPerMeter;
		wallStart.y = (shipdecks.at (levelName).lineSegments[i].start.y - (tileSize / 2)) / pixelsPerMeter;

		wallFinish.x = (shipdecks.at (levelName).lineSegments[i].finish.x - (tileSize / 2)) / pixelsPerMeter;
		wallFinish.y = (shipdecks.at (levelName).lineSegments[i].finish.y - (tileSize / 2)) / pixelsPerMeter;

		tempWall.bodyDef.type = b2_staticBody;
		tempWall.bodyDef.position.Set (0, 0);
		tempWall.body = physicsWorld->CreateBody (&tempWall.bodyDef);

		tempWall.userData                 = new _userData;
		tempWall.userData->userType       = PHYSIC_TYPE_WALL;
		tempWall.userData->dataValue      = -1;
		tempWall.userData->wallIndexValue = i;
		tempWall.body->SetUserData (tempWall.userData);

		tempWall.shape.Set (wallStart, wallFinish);
		tempWall.fixture.shape               = &tempWall.shape;
		tempWall.fixture.filter.categoryBits = PHYSIC_TYPE_WALL;
		tempWall.fixture.filter.maskBits     = PHYSIC_TYPE_PLAYER | PHYSIC_TYPE_ENEMY | PHYSIC_TYPE_BULLET_ENEMY | PHYSIC_TYPE_BULLET_PLAYER | PHYSIC_TYPE_PARTICLE;
		tempWall.body->CreateFixture (&tempWall.fixture);

		solidWalls.push_back (tempWall);
		wallPositions.push_back (wallStart);
	}
	shipdecks.at (levelName).wallPhysicsCreated = true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Cleanup Physics engine - run once
void sys_freePhysicsEngine ()
//----------------------------------------------------------------------------------------------------------------------
{
//	gam_clearHealing ();
//	gam_clearLifts ();
	sys_clearSolidWalls ();
//	sys_clearDroids ();
//	bul_clearAllBullets ();
//	gam_clearAllDoors ();
	delete physicsWorld;
}

//-------------------------------------------------------------------------------------------------------------
//
// Clear previous level - remove physics objects from droid before changing level name
void gam_clearDroidPhysics(std::string levelName)
//-------------------------------------------------------------------------------------------------------------
{
	if (levelName.empty())      // First run - no level set as yet
		return;

	for (auto& droidItr : shipdecks.at(levelName).droid)
	{
		if (droidItr.userData != nullptr)
		{
			delete (droidItr.userData);
			droidItr.userData = nullptr;
		}

		if (droidItr.body != nullptr)
		{
			droidItr.body->GetWorld()->DestroyBody(droidItr.body);
			//			sys_getPhysicsWorld()->DestroyBody(droidItr.body);
			droidItr.body = nullptr;
		}
	}
	shipdecks.at(levelName).droidPhysicsCreated = false;
}

//-------------------------------------------------------------------
//
// Create the physics bodies and shapes for the enemy droids
void sys_setupEnemyPhysics (std::string levelName)
//-------------------------------------------------------------------
{
	std::cout << "Setting up enemy physics for level : " << levelName << std::endl;
	
	if (!physicsStarted)
		sys_shutdownWithError (sys_getString ("Attempting to setup droid physics with no engine."));

	if (g_shipDeckItr->second.droidPhysicsCreated)
	{
		std::cout << "Droid physics already created for level : " << levelName << std::endl;
		return;
	}
	
	for (auto &droidItr : shipdecks.at (levelName).droid)
	{
		if (droidItr.currentMode == DROID_MODE_NORMAL)
		{
			droidItr.bodyDef.type = b2_dynamicBody;
			droidItr.bodyDef.position.Set (droidItr.worldPosInPixels.x / pixelsPerMeter, droidItr.worldPosInPixels.y / pixelsPerMeter);
			droidItr.bodyDef.angle = 0;
			droidItr.body          = physicsWorld->CreateBody (&droidItr.bodyDef);

			droidItr.userData                  = new _userData;
			droidItr.userData->userType        = PHYSIC_TYPE_ENEMY;
			droidItr.userData->dataValue	   = droidItr.ai.getArrayIndex();
			droidItr.userData->wallIndexValue  = -1;
			droidItr.userData->ignoreCollision = false;
			droidItr.body->SetUserData (droidItr.userData);

			droidItr.shape.m_radius = static_cast<float>((SPRITE_SIZE * 0.5) / pixelsPerMeter);
			droidItr.shape.m_p.Set (0, 0);

			droidItr.fixtureDef.shape               = &droidItr.shape;
			droidItr.fixtureDef.density             = 1;
			droidItr.fixtureDef.friction            = 0.3f;
			droidItr.fixtureDef.restitution         = 1.0f;
			droidItr.fixtureDef.filter.categoryBits = PHYSIC_TYPE_ENEMY;
			droidItr.fixtureDef.filter.maskBits     = PHYSIC_TYPE_WALL | PHYSIC_TYPE_BULLET_PLAYER | PHYSIC_TYPE_BULLET_ENEMY | PHYSIC_TYPE_PLAYER | PHYSIC_TYPE_ENEMY | PHYSIC_TYPE_DOOR_CLOSED;
			droidItr.body->CreateFixture (&droidItr.fixtureDef);
		}
	}


	for (auto droidItr : shipdecks.at(levelName).droid)
	{
		std::cout << "[ " << droidItr.ai.getArrayIndex() << " ] Position : " << droidItr.body->GetPosition().x << " " << droidItr.body->GetPosition().y << std::endl;
	}
	
	g_shipDeckItr->second.droidPhysicsCreated = true;
}
