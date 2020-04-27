#pragma once

#include "../main.h"
#include "box2d.h"
#include "game/physicsCollisions.h"

struct _userData
{
	int userType;
	int dataValue;
	int wallIndexValue;
};

extern bool d_showPhysics;

// Setup Physics engine - run once
bool sys_setupPhysicsEngine ();

// Return if the physics engine is ready to use
bool sys_isPhysicsReady ();