#include <classes/paraRandom.h>
#include <system/util.h>
#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include <game/bullet.h>
#include "game/particles.h"
#include "classes/paraParticle.h"

// #define DEBUG_PARTICLE 1

paraRandom angleRandom;

//-----------------------------------------------------------------------------------------------------------
//
// Return if the emitter is dead and can be removed
bool paraParticle::isDead (bool b)
//-----------------------------------------------------------------------------------------------------------
{
	return isDeadFlag;
}

//-----------------------------------------------------------------------------------------------------------
//
// Is the emitter currently attached to a bullet
bool paraParticle::isAttached ()
//-----------------------------------------------------------------------------------------------------------
{
	return attachedToBullet;
}

//-----------------------------------------------------------------------------------------------------------
//
// Is the attached bullet still alive.  If not, set to false - keep processing until all particles are gone
void paraParticle::setIsAttached (bool newState)
//-----------------------------------------------------------------------------------------------------------
{
	attachedToBullet = newState;
}

//-----------------------------------------------------------------------------------------------------------
//
// Return the bullet this emitter may be linked to - return -1 if not attached
Uint32 paraParticle::getAttachedBullet ()
//-----------------------------------------------------------------------------------------------------------
{
	if (!attachedToBullet)
		return 0;

	return bulletLink;
}

//-----------------------------------------------------------------------------------------------------------
//
// Set if the particle is in use or not
void paraParticle::setInUse(bool newState)
//-----------------------------------------------------------------------------------------------------------
{
	isInUse = newState;
}

//-----------------------------------------------------------------------------------------------------------
//
// Return inUse status for this emitter
bool paraParticle::inUse ()
//-----------------------------------------------------------------------------------------------------------
{
	return isInUse;
}

//-----------------------------------------------------------------------------------------------------------
//
// Constructor for a new paraParticle class - newWorldPos is in Physics coords
paraParticle::paraParticle (b2Vec2 newWorldPos, int newType, Uint32 newBulletID)
//-----------------------------------------------------------------------------------------------------------
{
#ifdef DEBUG_PARTICLE
	std::cout << "Construct new particle. Connect with bullet index : " << newBulletID << std::endl;
#endif

	__PARTICLE tempParticle;
	int        trailColor;

	particleSpeedExplosionMin      = 1;
	particleSpeedExplosionMax      = 5;
	particleSpeedSparkMin          = 1;
	particleSpeedSparkMax          = 3;
	particleExplosionInitialNumMin = 30;
	particleExplosionInitialNumMax = 90;
	particleSparkInitialNumMin     = 15;
	particleSparkInitialNumMax     = 35;
	particleTrailInitialNumMin     = 30;
	particleTrailInitialNumMax     = 90;
	particleElastic                = 0.9f;
	particleFriction               = 0.2f;
	particleMass                   = 0.05f;
	particleSize                   = 4.0f;
	particleTrailReduceRate        = 40;
	particleExplosionReduceRate    = 10;
	particleSparkReduceRate        = 12;

	worldPos   = newWorldPos;
	type       = newType;
	bulletLink = newBulletID;

	switch (type)
	{
		case PARTICLE_TYPE_EXPLOSION:
			numInitialParticles = angleRandom.get (particleExplosionInitialNumMin, particleExplosionInitialNumMax);
			usePhysics          = true;
			attachedToBullet    = false;
			break;

		case PARTICLE_TYPE_SPARK:
			numInitialParticles = angleRandom.get (particleSparkInitialNumMin, particleSparkInitialNumMax);
			usePhysics          = true;
			attachedToBullet    = false;
			break;

		case PARTICLE_TYPE_TRAIL:
			numInitialParticles = angleRandom.get (particleTrailInitialNumMin, particleTrailInitialNumMax);
			usePhysics          = false;
			attachedToBullet    = true;
			break;
	}

	if (particle.size () == 0)
	{
		particle.reserve (numInitialParticles);
	}
	else
	{
		particle.clear ();
		particle.reserve (numInitialParticles);
	}

	for (auto i = 0; i != numInitialParticles; i++)
	{
		tempParticle.worldPos = newWorldPos;
		tempParticle.isAlive  = true;

		switch (type)
		{
			case PARTICLE_TYPE_EXPLOSION:
				tempParticle.direction = getCircleAngle ();
				tempParticle.color.r   = 200;
				tempParticle.color.g   = 100;
				tempParticle.color.b   = 100;
				tempParticle.color.a   = angleRandom.get (100, 254);;
				break;

			case PARTICLE_TYPE_SPARK:
				tempParticle.direction = getCircleAngle ();
				tempParticle.color.r   = 220;
				tempParticle.color.g   = 190;
				tempParticle.color.b   = 200;
				tempParticle.color.a   = angleRandom.get (100, 254);;
				break;

			case PARTICLE_TYPE_TRAIL:
				tempParticle.worldPos = sys_convertMetersToPixels (tempParticle.worldPos);
				tempParticle.worldPos.x -= (6 - angleRandom.get (0, 12));
				tempParticle.worldPos.y -= (6 - angleRandom.get (0, 12));
				tempParticle.worldPos = sys_convertPixelsToMeters (tempParticle.worldPos);
				trailColor = angleRandom.get (100, 254);
				tempParticle.color.r = trailColor;
				tempParticle.color.g = trailColor;
				tempParticle.color.b = trailColor;
				tempParticle.color.a = trailColor;
				break;
		}

		if (usePhysics)
		{
			tempParticle.physicObject.bodyDef.type = b2_dynamicBody;
			tempParticle.physicObject.bodyDef.position.Set (tempParticle.worldPos.x, tempParticle.worldPos.y);
			tempParticle.physicObject.bodyDef.angle = 0;
			tempParticle.physicObject.body          = sys_getPhysicsWorld ()->CreateBody (&tempParticle.physicObject.bodyDef);
			if (tempParticle.physicObject.body == nullptr)
				sys_shutdownWithError (sys_getString ("Unable to create physics body for particle."));

			tempParticle.physicObject.userData            = new _userData;
			tempParticle.physicObject.userData->userType  = PHYSIC_TYPE_PARTICLE;
			tempParticle.physicObject.userData->dataValue = -1;
			tempParticle.physicObject.body->SetUserData (tempParticle.physicObject.userData);

			tempParticle.physicObject.shape.m_radius = static_cast<float>(particleSize / pixelsPerMeter);
			tempParticle.physicObject.shape.m_p.Set (0, 0);

			tempParticle.physicObject.fixtureDef.shape               = &tempParticle.physicObject.shape;
			tempParticle.physicObject.fixtureDef.density             = particleMass;
			tempParticle.physicObject.fixtureDef.friction            = particleFriction;
			tempParticle.physicObject.fixtureDef.restitution         = particleElastic;
			tempParticle.physicObject.fixtureDef.filter.categoryBits = PHYSIC_TYPE_PARTICLE;
			tempParticle.physicObject.fixtureDef.filter.maskBits     = PHYSIC_TYPE_WALL;
			tempParticle.physicObject.body->CreateFixture (&tempParticle.physicObject.fixtureDef);
		}
		particle.push_back (tempParticle);
	}
	isInUse = true;
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

	stopContactPhysicsBugFlag = true;

	if (usePhysics)
	{
		for (auto &partItr : particle)
		{
			if (partItr.physicObject.userData != nullptr)
			{
//				delete partItr.physicObject.userData;
				partItr.physicObject.userData = nullptr;
			}

			if (partItr.physicObject.body != nullptr)
			{
				/*
				partItr.physicObject.body->DestroyFixture (partItr.physicObject.body->GetFixtureList());
				partItr.physicObject.body->SetEnabled (false);
				partItr.physicObject.body->SetUserData (nullptr);
				partItr.physicObject.body->GetWorld()->DestroyBody (partItr.physicObject.body);
				partItr.physicObject.body = nullptr;
				*/
			}
		}
	}
	particle.clear ();

	stopContactPhysicsBugFlag = false;
}

//-----------------------------------------------------------------------------------------------------------
//
// Get a random normalized vector in a circle shape
b2Vec2 paraParticle::getCircleAngle ()
//-----------------------------------------------------------------------------------------------------------
{
	int    angle, speed;
	b2Vec2 direction;

	angle = angleRandom.get (0, 359);

	switch (type)
	{
		case PARTICLE_TYPE_EXPLOSION:
			speed = angleRandom.get (particleSpeedExplosionMin, particleSpeedExplosionMax);
			break;

		case PARTICLE_TYPE_SPARK:
			speed = angleRandom.get (particleSpeedSparkMin, particleSpeedSparkMax);
			break;
	}

#ifdef USE_LOOKUP_TABLE
	direction.x = static_cast<float>(speed) * sys_getCosValue(angle);
	direction.y = static_cast<float>(speed) * sys_getSinValue(angle);
#else
	direction.x = (speed * cos (angle));
	direction.y = (speed * sin (angle));
#endif

	direction.Normalize ();  // Is this needed, negates getting the speed??
	return direction;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Animate the particles
void paraParticle::animate ()
//----------------------------------------------------------------------------------------------------------------------
{
	int    particleLifetimeReduce = 0;
	int    tempAlpha;
	int     tempBulletIndex;
	b2Vec2 spacingVelocity;

	switch (type)
	{
		case PARTICLE_TYPE_EXPLOSION:
			particleLifetimeReduce = particleExplosionReduceRate;
			break;

		case PARTICLE_TYPE_SPARK:
			particleLifetimeReduce = particleSparkReduceRate;
			break;

		case PARTICLE_TYPE_TRAIL:
			particleLifetimeReduce = angleRandom.get (1, particleTrailReduceRate);
			break;
	}

	for (auto &particleItr : particle)
	{
		tempAlpha = static_cast<int>(particleItr.color.a);
		tempAlpha -= particleLifetimeReduce;

		if (tempAlpha < 0)
		{
			particleItr.isAlive = false;
			particleItr.color.a = 0;
			if (type != PARTICLE_TYPE_TRAIL)
			{
				if (usePhysics)
				{
					if (particleItr.physicObject.body != nullptr)
					{
						particleItr.physicObject.body->GetWorld ()->DestroyBody (particleItr.physicObject.body);
						particleItr.physicObject.body = nullptr;
					}
					if (particleItr.physicObject.userData != nullptr)
					{
						delete particleItr.physicObject.userData;
						particleItr.physicObject.userData = nullptr;
					}
				}
			}

			if (attachedToBullet)  // Still connected to a bullet
			{
				tempBulletIndex = gam_getArrayIndex(bulletLink);    // Check that bullet is still valid
				if (tempBulletIndex < 0)
				{
					isDead (true);
				}
				else
				{
					particleItr.isAlive  = true;
					particleItr.worldPos = sys_convertMetersToPixels (bullets[gam_getArrayIndex (bulletLink)].worldPosInMeters);
					particleItr.worldPos.x -= ((bullets[gam_getArrayIndex (bulletLink)].sprite.getFrameWidth () * 0.5) - angleRandom.get (0, bullets[gam_getArrayIndex (bulletLink)].sprite.getFrameWidth ()));
					particleItr.worldPos.y -= ((bullets[gam_getArrayIndex (bulletLink)].sprite.getFrameHeight () * 0.5) - angleRandom.get (0, bullets[gam_getArrayIndex (bulletLink)].sprite.getFrameHeight ()));
					//
					// Put starting position behind direction of bullet travel
					spacingVelocity      = bullets[gam_getArrayIndex (bulletLink)].velocity;
					spacingVelocity.Normalize ();
					spacingVelocity *= static_cast<float>(bullets[gam_getArrayIndex (bulletLink)].sprite.getFrameWidth () * 0.5f);
					particleItr.worldPos -= spacingVelocity;
					particleItr.worldPos = sys_convertPixelsToMeters (particleItr.worldPos);

					particleItr.color.a = angleRandom.get (100, 255);
				}
			}
		}
		else    // Particle is still alive
		{
			switch (type)
			{
				case PARTICLE_TYPE_EXPLOSION:
				case PARTICLE_TYPE_SPARK:
					if (usePhysics)
					{
						particleItr.physicObject.body->ApplyForce (particleItr.direction, particleItr.physicObject.body->GetWorldCenter (), true);
						particleItr.color.a = static_cast<Uint8>(tempAlpha);
					}
					else
					{
						particleItr.direction.Normalize ();
						particleItr.direction *= 0.10f;

						particleItr.worldPos += particleItr.direction;
						particleItr.color.a = static_cast<Uint8>(tempAlpha);
					}
					break;

				case PARTICLE_TYPE_TRAIL:
					particleItr.color.a = static_cast<Uint8>(tempAlpha);
					break;
			}
		}       // Particle is still alive
	}       // For each particle loop

	auto            aliveCounter = 0;
	//
	// Check to see if all the particles are dead so the emitter can be removed
	for (const auto &partItr : particle)
	{
		if (partItr.isAlive)
		{
			aliveCounter++;
		}
	}

//	std::cout << "Alive counter : " << aliveCounter << std::endl;

	if (aliveCounter > 0)
		return;

	isDeadFlag = true;
	isInUse    = false;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render the particles
void paraParticle::render ()
//----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2 renderPosition;

	for (auto &particleItr : particle)
	{
		if (particleItr.isAlive)    // Shouldn't be needed?
		{
			switch (type)
			{
				case PARTICLE_TYPE_EXPLOSION:
				case PARTICLE_TYPE_SPARK:
					if (usePhysics)
					{
						if (particleItr.physicObject.body != nullptr)
						{
							renderPosition = particleItr.physicObject.body->GetPosition ();
						}
						else
						{
							std::cout << "Attempting to get position from null body" << std::endl;
						}
					}
					else
						renderPosition = particleItr.worldPos;
					break;

				case PARTICLE_TYPE_TRAIL:
					renderPosition = particleItr.worldPos;
					break;
			}

			renderPosition = sys_convertMetersToPixels (renderPosition);
			renderPosition = sys_worldToScreen (renderPosition, static_cast<int>(particleSize));

			switch (particleRenderType)
			{
				case PARTICLE_RENDER_BITMAP:
					break;

				case PARTICLE_RENDER_PRIMITIVE:
					filledCircleRGBA (renderer.renderer, renderPosition.x, renderPosition.y, particleSize / 2, particleItr.color.r, particleItr.color.g, particleItr.color.b, particleItr.color.a);
					break;
			}
		}
	}
}