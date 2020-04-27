#include "system/physics.h"

b2World *physicsWorld;
bool    d_showPhysics  = false;
bool    physicsStarted = true;

//----------------------------------------------------------------------------------------------------------------------
//
// Return if the physics engine is ready to use
bool sys_isPhysicsReady ()
//----------------------------------------------------------------------------------------------------------------------
{
	return physicsStarted;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup Physics engine - run once
bool sys_setupPhysicsEngine ()
//----------------------------------------------------------------------------------------------------------------------
{
	// Define the gravity vector.
	b2Vec2 gravity (0.0f, 0.0f);

	// Construct a world object, which will hold and simulate the rigid bodies.
	physicsWorld = new b2World (gravity);

	physicsWorld->SetContactListener (&myContactListenerInstance);
	if (d_showPhysics)
	{
		g_paraDebugDraw.SetFlags (b2Draw::e_shapeBit);  // Draw shapes
		physicsWorld->SetDebugDraw (&g_paraDebugDraw);
	}

	physicsStarted = true;

	return true;
}