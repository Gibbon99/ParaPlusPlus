#include "classes/paraRandom.h"
#include "sdl2_gfx/SDL2_gfxPrimitives.h"
#include "system/util.h"
#include "game/particles.h"
#include "classes/paraParticle.h"

paraRandom particleAngleRandom {};

//-----------------------------------------------------------------------------------------------------------
//
// Constructor for a new paraParticle class - newWorldPos is in Physics coords
paraParticle::paraParticle(int newType, cpVect newWorldPos)
//-----------------------------------------------------------------------------------------------------------
{
#ifdef DEBUG_PARTICLE
	std::cout << "Construct new particle. Connect with bullet index : " << newBulletID << std::endl;
#endif
	int trailColor {};

	m_worldPos     = newWorldPos;
	m_particleType = newType;
	m_isAlive      = true;

	switch (newType)
	{
		case PARTICLE_TYPE_EXPLOSION:
			m_direction = getCircleAngle ();
			color.r = 200;
			color.g = 100;
			color.b = 100;
			color.a = particleAngleRandom.get (100, 254);;
			break;

		case PARTICLE_TYPE_SPARK:
			m_direction = getCircleAngle ();
			color.r = 220;
			color.g = 190;
			color.b = 200;
			color.a = particleAngleRandom.get (100, 254);
			break;

		case PARTICLE_TYPE_TRAIL:
			m_worldPos.x -= (6 - particleAngleRandom.get (0, 12));
			m_worldPos.y -= (6 - particleAngleRandom.get (0, 12));
			trailColor = particleAngleRandom.get (100, 254);
			color.r = trailColor;
			color.g = trailColor;
			color.b = trailColor;
			color.a = trailColor;
			break;
	}

	if (m_particleType != PARTICLE_TYPE_TRAIL)
	{
		body = cpSpaceAddBody (sys_returnPhysicsWorld (), cpBodyNew (particleMass, cpMomentForCircle (particleMass, 0.0f, particleSize, cpvzero)));
		cpBodySetMass (body, particleMass);
		cpBodySetVelocity (body, cpvzero);
		cpBodySetPosition (body, m_worldPos);

		shape = cpSpaceAddShape (sys_returnPhysicsWorld (), cpCircleShapeNew (body, particleSize, cpvzero));
		cpShapeSetFriction (shape, particleFriction);
		cpShapeSetElasticity (shape, particleElastic);
		cpShapeSetCollisionType (shape, PHYSIC_TYPE_PARTICLE);
		cpShapeSetFilter (shape, FILTER_CAT_PARTICLE);

		userData = std::make_shared<_userData> ();
		userData->bulletID              = -1;
		userData->userType              = cpShapeGetCollisionType (shape);
		userData->dataValue             = -1;
//		userData->wallIndexValue        = -1;
		userData->ignoreCollisionDroid  = false;
		userData->ignoreCollisionPlayer = false;
		cpShapeSetUserData (shape, userData.get ());
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return if the particle is alive or not
bool paraParticle::isAlive() const
//----------------------------------------------------------------------------------------------------------------------
{
	return m_isAlive;
}

//-----------------------------------------------------------------------------------------------------------
//
// Deconstructor for a particle class
paraParticle::~paraParticle()
//-----------------------------------------------------------------------------------------------------------
{
#ifdef DEBUG_PARTICLE
	std::cout << "De-construct particle" << std::endl;
#endif

	if (m_particleType != PARTICLE_TYPE_TRAIL)
	{
		if (shape != nullptr)
		{
			cpSpaceRemoveShape (sys_returnPhysicsWorld (), shape);
			shape = nullptr;
		}
		if (body != nullptr)
		{
			cpSpaceRemoveBody (sys_returnPhysicsWorld (), body);
			body = nullptr;
		}
	}
}

//-----------------------------------------------------------------------------------------------------------
//
// Get a random normalized vector in a circle shape
cpVect paraParticle::getCircleAngle() const
//-----------------------------------------------------------------------------------------------------------
{
	int    angle {}, speed {};
	cpVect newDirection {};

	angle = particleAngleRandom.get (0, 359);

	switch (m_particleType)
	{
		case PARTICLE_TYPE_EXPLOSION:
			speed = particleAngleRandom.get (particleSpeedExplosionMin, particleSpeedExplosionMax);
			break;

		case PARTICLE_TYPE_SPARK:
			speed = particleAngleRandom.get (particleSpeedSparkMin, particleSpeedSparkMax);
			break;
	}

	newDirection = cpvnormalize (newDirection);

#ifdef USE_LOOKUP_TABLE
	newDirection.x = static_cast<float>(speed) * sys_getCosValue (angle);
	newDirection.y = static_cast<float>(speed) * sys_getSinValue (angle);
#else
	direction.x = (speed * cos (angle));
	direction.y = (speed * sin (angle));
#endif

	return newDirection;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Animate the particle
void paraParticle::animate()
//----------------------------------------------------------------------------------------------------------------------
{
	int particleLifetimeReduce {};
	int tempAlpha {};

	switch (m_particleType)
	{
		case PARTICLE_TYPE_EXPLOSION:
			particleLifetimeReduce = particleExplosionReduceRate;
			break;

		case PARTICLE_TYPE_SPARK:
			particleLifetimeReduce = particleSparkReduceRate;
			break;

		case PARTICLE_TYPE_TRAIL:
			particleLifetimeReduce = particleAngleRandom.get (1, particleTrailReduceRate);
			break;
	}

	tempAlpha = static_cast<int>(color.a);
	tempAlpha -= particleLifetimeReduce;

	if (tempAlpha < 0)
	{
		m_isAlive = false;
		color.a = 0;
	}
	else    // Particle is still alive
	{
		switch (m_particleType)
		{
			case PARTICLE_TYPE_EXPLOSION:
			case PARTICLE_TYPE_SPARK:
				if (body != nullptr)
					cpBodyApplyForceAtLocalPoint (body, m_direction, cpvzero);
				color.a = static_cast<Uint8>(tempAlpha);
				break;

			case PARTICLE_TYPE_TRAIL:
				color.a = static_cast<Uint8>(tempAlpha);
				break;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render the particle
void paraParticle::render()
//----------------------------------------------------------------------------------------------------------------------
{
	cpVect renderPosition;

	if (m_isAlive)
	{
		switch (m_particleType)
		{
			case PARTICLE_TYPE_EXPLOSION:
			case PARTICLE_TYPE_SPARK:
				if (body != nullptr)
					renderPosition = cpBodyGetPosition (body);
				break;

			case PARTICLE_TYPE_TRAIL:
				renderPosition = m_worldPos;
				break;
		}

		renderPosition = sys_worldToScreen (renderPosition, static_cast<int>(particleSize));

		filledCircleRGBA (renderer.renderer, renderPosition.x, renderPosition.y, particleSize / 2, color.r, color.g, color.b, color.a);
	}
}
