#include <game/player.h>
#include <game/shipDecks.h>
#include <system/util.h>
#include <game/lifts.h>
#include "system/physics.h"
#include "classes/paraPhysicsDestruction.h"

struct __physicWall
{
	b2BodyDef    bodyDef;
	b2Body       *body     = nullptr;
	b2EdgeShape  shape;
	b2FixtureDef fixture;
	_userData    *userData = nullptr;
};

std::vector<__physicWall> solidWalls;
b2World                   *physicsWorld;
bool                      d_showPhysics             = false;
bool                      physicsStarted            = true;
int32                     velocityIterations        = 6;   //how strongly to correct velocity
int32                     positionIterations        = 3;   //how strongly to correct position
double                    gravity;         // Set from script
bool                      stopContactPhysicsBugFlag = false;

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
	playerDroid.body->ApplyForce (playerDroid.getVelocity (), playerDroid.body->GetWorldCenter (), true);

	sys_stepPhysicsWorld (1.0f / TICKS_PER_SECOND);

	playerDroid.previousWorldPosInPixels = playerDroid.getWorldPosInPixels ();
	playerDroid.setWorldPosInPixels (sys_convertMetersToPixels (playerDroid.body->GetPosition ()));        // GetPosition is in meters

	playerDroid.body->SetLinearVelocity ({0, 0});

	if ((g_shipDeckItr->second.droidPhysicsCreated) || (!g_shipDeckItr->second.deckIsDead))
	{
		for (auto &droidItr: shipdecks.at (gam_getCurrentDeckName ()).droid)
		{
			if (droidItr.getCurrentMode () == DROID_MODE_NORMAL)
			{
				droidItr.previousWorldPosInPixels = droidItr.getWorldPosInPixels ();
				if (droidItr.body == nullptr)
					sys_shutdownWithError ("Invalid droidItr body pointer. Set to nullptr");

				droidItr.setWorldPosInPixels (sys_convertMetersToPixels (droidItr.body->GetPosition ()));
				droidItr.body->SetLinearVelocity ({0, 0});
			}
		}
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

	physicsWorld->SetDestructionListener (&myDestructionListener);

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

	if (playerDroid.body != nullptr)    // Physics already setup
		return;

	playerDroid.setBodyDefType (b2_dynamicBody);
	playerDroid.setBodyPosition (b2Vec2 {2, 3});
	playerDroid.setBodyAngle (0);
	playerDroid.body = physicsWorld->CreateBody (playerDroid.getBodyDef ());

	//std::unique_ptr<_userData> tempUserData (new _userData);

	playerDroid.userData                       = reinterpret_cast<_userData *>(sys_malloc (sizeof (_userData), "playerPhysics")); //   = (new _userData);
	playerDroid.userData->userType             = PHYSIC_TYPE_PLAYER;
	playerDroid.userData->dataValue            = -1;
	playerDroid.userData->ignoreCollisionDroid = false;
	playerDroid.body->SetUserData (playerDroid.userData);

	playerDroid.setShapeRadius ((SPRITE_SIZE / 2) / pixelsPerMeter);
	playerDroid.setShapePosition (b2Vec2 {0, 0});

	playerDroid.setFixtureDefShape (playerDroid.getShape ());
	playerDroid.setFixtureDefDensity (1.0f);
	playerDroid.setFixtureDefFriction (playerFriction);
	playerDroid.setFixtureRestitution (1.0f);
	playerDroid.setFixtureDefFilterCategoryBits (PHYSIC_TYPE_PLAYER);
	playerDroid.setFixtureDefMaskBits (PHYSIC_TYPE_WALL | PHYSIC_TYPE_BULLET_ENEMY | PHYSIC_TYPE_DOOR_CLOSED | PHYSIC_TYPE_ENEMY);
	playerDroid.body->CreateFixture (playerDroid.getFixtureDef ());

	playerDroid.setVelocity (b2Vec2{0, 0});
}

//----------------------------------------------------------------------------------------------------------------------
//
// Free memory for solid walls and free bodies
void sys_clearSolidWalls ()
//----------------------------------------------------------------------------------------------------------------------
{
	stopContactPhysicsBugFlag = true;

	for (auto &wallItr: solidWalls)
	{
		if (wallItr.userData != nullptr)
		{
			delete (wallItr.userData);
			wallItr.userData = nullptr;
		}

		if (wallItr.body != nullptr)
		{
//			wallItr.body->DestroyFixture (wallItr.body->GetFixtureList());
//			wallItr.body->SetEnabled (false);
			wallItr.body->SetUserData (nullptr);
			wallItr.body->GetWorld ()->DestroyBody (wallItr.body);
			wallItr.body = nullptr;
		}
	}
	solidWalls.clear ();

	stopContactPhysicsBugFlag = false;
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
	if (nullptr != physicsWorld)
	{
		sys_clearSolidWalls ();
		delete physicsWorld;
		physicsWorld = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Clear previous level - remove physics objects from droid before changing level name
void sys_clearDroidPhysics (std::string levelName)
//-------------------------------------------------------------------------------------------------------------
{
	if (levelName.empty ())      // First run - no level set as yet
		return;

	stopContactPhysicsBugFlag                    = true;

	for (auto &droidItr: shipdecks.at (levelName).droid)
	{
		if (droidItr.userData != nullptr)
		{
			sys_freeMemory (sys_getString ("%s-%i", levelName.c_str (), droidItr.ai2.getArrayIndex ()));
			//free (droidItr.userData);
			droidItr.userData = nullptr;
		}

		if (droidItr.body != nullptr)
		{
			droidItr.body->DestroyFixture (droidItr.body->GetFixtureList ());
			droidItr.body->SetEnabled (false);
			droidItr.body->SetUserData (nullptr);
			droidItr.body->GetWorld ()->DestroyBody (droidItr.body);
			droidItr.body = nullptr;
		}
	}
	shipdecks.at (levelName).droidPhysicsCreated = false;

	stopContactPhysicsBugFlag = false;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the physics bodies and shapes for the enemy droids
void sys_setupDroidPhysics (std::string levelName)
//----------------------------------------------------------------------------------------------------------------------
{
#ifdef MY_DEBUG
	std::cout << "Setting up enemy physics for level : " << levelName << std::endl;
#endif

	if (!physicsStarted)
		sys_shutdownWithError (sys_getString ("Attempting to setup droid physics with no engine."));

	if (shipdecks.at (levelName).deckIsDead)   // No droids alive
	{
#ifdef MY_DEBUG
		std::cout << "Level is dead - no physics to create." << std::endl;
#endif
		return;
	}

	if (shipdecks.at (levelName).droidPhysicsCreated)
	{
#ifdef MY_DEBUG
		std::cout << "Droid physics already created for level : " << levelName << std::endl;
#endif
		return;
	}

	for (auto &droidItr: shipdecks.at (levelName).droid)
	{
		if (droidItr.getCurrentMode () == DROID_MODE_NORMAL)
		{
			if (droidItr.body == nullptr)
			{
				droidItr.setBodyDefType (b2_dynamicBody);
				droidItr.setBodyPosition (sys_convertPixelsToMeters (droidItr.getWorldPosInPixels ()));
				droidItr.setBodyAngle (0);
				droidItr.body = physicsWorld->CreateBody (droidItr.getBodyDef ());

				droidItr.userData                        = reinterpret_cast<_userData *>(sys_malloc (sizeof (_userData), sys_getString ("%s-%i", levelName.c_str (), droidItr.ai2.getArrayIndex ()))); //new _userData;     // TODO Free this on shutdown
				droidItr.userData->userType              = PHYSIC_TYPE_ENEMY;
				droidItr.userData->dataValue             = droidItr.ai2.getArrayIndex ();
				droidItr.userData->ID                    = 0;
				droidItr.userData->wallIndexValue        = -1;
				droidItr.userData->ignoreCollisionDroid  = true;
				droidItr.userData->ignoreCollisionPlayer = true;
				droidItr.body->SetUserData (droidItr.userData);

				droidItr.setShapeRadius (static_cast<float>((SPRITE_SIZE * 0.5) / pixelsPerMeter));
				droidItr.setShapePosition (b2Vec2 {0, 0});

				droidItr.setFixtureDefShape (droidItr.getShape ());
				droidItr.setFixtureDefDensity (1);
				droidItr.setFixtureDefFriction (0.3f);
				droidItr.setFixtureRestitution (1.0f);
				droidItr.setFixtureDefFilterCategoryBits (PHYSIC_TYPE_ENEMY);
				droidItr.setFixtureDefMaskBits (PHYSIC_TYPE_WALL | PHYSIC_TYPE_BULLET_PLAYER | PHYSIC_TYPE_BULLET_ENEMY | PHYSIC_TYPE_PLAYER | PHYSIC_TYPE_ENEMY | PHYSIC_TYPE_DOOR_CLOSED);
				droidItr.body->CreateFixture (droidItr.getFixtureDef ());
			}
		}
	}
	shipdecks.at (levelName).droidPhysicsCreated = true;
#ifdef MY_DEBUG
	std::cout << "Droid physics created for level : " << levelName << std::endl;
#endif
}

//----------------------------------------------------------------------------------------------------------------------
//
// Find out how many physics body are currently in use in the world
void debug_getNumberOfShapes ()
//----------------------------------------------------------------------------------------------------------------------
{
	g_debugDroidCount = 0;

	for (b2Body *b = sys_getPhysicsWorld ()->GetBodyList (); b; b = b->GetNext ())
	{
		g_debugDroidCount++;
	}
}
