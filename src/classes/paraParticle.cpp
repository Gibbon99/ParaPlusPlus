#include "classes/paraRandom.h"
#include "sdl2_gfx/SDL2_gfxPrimitives.h"
#include "system/util.h"
#include "game/particles.h"
#include "classes/paraParticle.h"

paraRandom particleAngleRandom {};

//-----------------------------------------------------------------------------------------------------------
//
// Constructor for a new paraParticle class - newWorldPos is in Physics coords
paraParticle::paraParticle (int newType, b2Vec2 newWorldPos)
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

			printf ("Particle constructor - PARTICLE_TYPE_EXPLOSION.\n");
			break;

		case PARTICLE_TYPE_SPARK:
			m_direction = getCircleAngle ();
			color.r = 220;
			color.g = 190;
			color.b = 200;
			color.a = particleAngleRandom.get (100, 254);

			printf ("Particle constructor - PARTICLE_TYPE_SPARK.\n");

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
		physicObject.bodyDef.type = b2_dynamicBody;
		physicObject.bodyDef.position.Set (m_worldPos.x, m_worldPos.y);
		physicObject.bodyDef.angle = 0;
		physicObject.body          = sys_getPhysicsWorld ()->CreateBody (getBodyDef ());
		if (physicObject.body == nullptr)
			sys_shutdownWithError (sys_getString ("Unable to create physics body for particle."));

//		auto *userData = new _userData;

		physicObject.userData            = new _userData; //reinterpret_cast<_userData *>(sys_malloc (sizeof (_userData), sys_getString ("%i", 1)));; // (new _userData);    // TODO - use para memory routine
		physicObject.userData->userType  = PHYSIC_TYPE_PARTICLE;
		physicObject.userData->dataValue = 0;
		physicObject.body->SetUserData (physicObject.userData);

//		userData->userType  = PHYSIC_TYPE_PARTICLE;
//		userData->dataValue = 0;
//		physicObject.body->SetUserData (userData);

//		delete userData;

//		delete physicObject.userData;

		physicObject.shape.m_radius = static_cast<float>(particleSize / pixelsPerMeter);
		physicObject.shape.m_p.Set (0, 0);

		physicObject.fixtureDef.shape               = &physicObject.shape;
		physicObject.fixtureDef.density             = particleMass;
		physicObject.fixtureDef.friction            = particleFriction;
		physicObject.fixtureDef.restitution         = particleElastic;
		physicObject.fixtureDef.filter.categoryBits = PHYSIC_TYPE_PARTICLE;
		physicObject.fixtureDef.filter.maskBits     = PHYSIC_TYPE_WALL;
		physicObject.body->CreateFixture (&physicObject.fixtureDef);
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Return the location of the body definition
b2BodyDef *paraParticle::getBodyDef ()
//-------------------------------------------------------------------------------------------------------------
{
	return &physicObject.bodyDef;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return if the particle is alive or not
bool paraParticle::isAlive () const
//----------------------------------------------------------------------------------------------------------------------
{
	return m_isAlive;
}

//-----------------------------------------------------------------------------------------------------------
//
// Deconstructor for a particle class
paraParticle::~paraParticle ()
//-----------------------------------------------------------------------------------------------------------
{
#ifdef DEBUG_PARTICLE
	std::cout << "De-construct particle" << std::endl;
#endif

	if (m_particleType != PARTICLE_TYPE_TRAIL)
	{
		stopContactPhysicsBugFlag = true;

		if (physicObject.userData != nullptr)
		{
//			delete physicObject.userData;
			physicObject.userData = nullptr;
		}

		if (physicObject.body != nullptr)
		{
			physicObject.body->DestroyFixture (physicObject.body->GetFixtureList ());
			physicObject.body->SetEnabled (false);
			physicObject.body->SetUserData (nullptr);
			physicObject.body->GetWorld ()->DestroyBody (physicObject.body);
			physicObject.body = nullptr;
		}

		stopContactPhysicsBugFlag = false;
	}
	else
	{
		std::cout << "Particle destructor called - PARTICLE_TYPE_TRAIL." << std::endl;
	}
}

//-----------------------------------------------------------------------------------------------------------
//
// Get a random normalized vector in a circle shape
b2Vec2 paraParticle::getCircleAngle ()
//-----------------------------------------------------------------------------------------------------------
{
	int    angle {}, speed {};
	b2Vec2 newDirection {};

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

#ifdef USE_LOOKUP_TABLE
	newDirection.x = static_cast<float>(speed) * sys_getCosValue (angle);
	newDirection.y = static_cast<float>(speed) * sys_getSinValue (angle);
#else
	direction.x = (speed * cos (angle));
	direction.y = (speed * sin (angle));
#endif

	newDirection.Normalize ();  // Is this needed, negates getting the speed??
	return newDirection;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Animate the particle
void paraParticle::animate ()
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
				physicObject.body->ApplyForce (m_direction, physicObject.body->GetWorldCenter (), true);
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
void paraParticle::render ()
//----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2 renderPosition;

	if (m_isAlive)
	{
		switch (m_particleType)
		{
			case PARTICLE_TYPE_EXPLOSION:
			case PARTICLE_TYPE_SPARK:
				renderPosition = physicObject.body->GetPosition ();
				renderPosition = sys_convertMetersToPixels (renderPosition);
				break;

			case PARTICLE_TYPE_TRAIL:
				renderPosition = m_worldPos;
				break;
		}

		renderPosition = sys_worldToScreen (renderPosition, static_cast<int>(particleSize));

		filledCircleRGBA (renderer.renderer, renderPosition.x, renderPosition.y, particleSize / 2, color.r, color.g, color.b, color.a);
	}
}
