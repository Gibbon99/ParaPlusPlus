#include <system/util.h>
#include <classes/paraBullet.h>
#include "classes/paraEmitter.h"

#define USE_LOOKUP_TABLE 1

paraRandom angleRandom {};

//----------------------------------------------------------------------------------------------------------------------
//
// Deconstructor for an emitter class
paraEmitter::~paraEmitter()
//----------------------------------------------------------------------------------------------------------------------
{

}

//----------------------------------------------------------------------------------------------------------------------
//
// Constructor for a new paraEmitter - pass in position in worldspace ( pixels )  coordinates
// Position for PARTICLE_TYPE_TRAIL is in pixels
paraEmitter::paraEmitter(const cpVect newWorldPos, const int newEmitterType, const Uint32 newBulletID)
//----------------------------------------------------------------------------------------------------------------------
{
	int numInitialParticles {};

	m_worldPos     = newWorldPos;
	m_emitterType  = newEmitterType;
	m_bulletID     = newBulletID;
	m_canBeRemoved = false;

	switch (m_emitterType)
	{
		case PARTICLE_TYPE_EXPLOSION:
			numInitialParticles = angleRandom.get (particleExplosionInitialNumMin, particleExplosionInitialNumMax);
			m_attachedToBullet  = false;
			break;

		case PARTICLE_TYPE_SPARK:
			numInitialParticles = angleRandom.get (particleSparkInitialNumMin, particleSparkInitialNumMax);
			m_attachedToBullet  = false;
			break;

		case PARTICLE_TYPE_TRAIL:
			numInitialParticles = angleRandom.get (particleTrailInitialNumMin, particleTrailInitialNumMax);
			m_attachedToBullet  = true;
			break;
	}

	particles.reserve (numInitialParticles);

	for (int i = 0; i != numInitialParticles; i++)
	{
		particles.emplace_back (m_emitterType, newWorldPos);
	}
}

//-----------------------------------------------------------------------------------------------------------
//
// Remove the emitter from a bullet
void paraEmitter::setAttachedToBullet(bool newValue)
//-----------------------------------------------------------------------------------------------------------
{
	m_attachedToBullet = newValue;
}

//-----------------------------------------------------------------------------------------------------------
//
// Return size of particle vector
size_t paraEmitter::getNumberParticles()
//-----------------------------------------------------------------------------------------------------------
{
	return particles.size ();
}

//-----------------------------------------------------------------------------------------------------------
//
// Render all the particles for this emitter
void paraEmitter::render()
//-----------------------------------------------------------------------------------------------------------
{
	for (auto &partItr: particles)
	{
		partItr.render ();
	}
}

//-----------------------------------------------------------------------------------------------------------
//
// Check to see if the emitter is no longer in use and remove it
bool paraEmitter::getCanBeRemoved() const
//-----------------------------------------------------------------------------------------------------------
{
	return m_canBeRemoved;
}

//-----------------------------------------------------------------------------------------------------------
//
// Get rid of an emitter
void paraEmitter::setCanBeRemoved(bool newValue)
//-----------------------------------------------------------------------------------------------------------
{
	m_canBeRemoved = newValue;
}

//-----------------------------------------------------------------------------------------------------------
//
// Return the bullet this emitter may be linked too
Uint32 paraEmitter::getBulletID() const
//-----------------------------------------------------------------------------------------------------------
{
	return m_bulletID;
}

//-----------------------------------------------------------------------------------------------------------
//
// Process each of the particles for animation - also remove all dead particles
void paraEmitter::process()
//-----------------------------------------------------------------------------------------------------------
{
	if (m_canBeRemoved)
		return;

	if ((m_attachedToBullet) && (PARTICLE_TYPE_TRAIL == m_emitterType))  // Still connected to a bullet
	{
		auto tempBulletIndex = gam_getArrayIndex (m_bulletID);    // Check that bullet is still valid

		if (tempBulletIndex < 0)
		{
			m_attachedToBullet = false;
		}
		else
		{
			// add new particle to form trail behind bullet
			auto tempWorldPos = bullets[gam_getArrayIndex (m_bulletID)].worldPosInPixels;
			tempWorldPos.x -= ((bullets[gam_getArrayIndex (m_bulletID)].sprite.getFrameWidth () * 0.5) - angleRandom.get (0, bullets[gam_getArrayIndex (m_bulletID)].sprite.getFrameWidth ()));
			tempWorldPos.y -= ((bullets[gam_getArrayIndex (m_bulletID)].sprite.getFrameHeight () * 0.5) - angleRandom.get (0, bullets[gam_getArrayIndex (m_bulletID)].sprite.getFrameHeight ()));
			//
			// Put starting position behind direction of bullet travel
			auto spacingVelocity = bullets[gam_getArrayIndex (m_bulletID)].velocity;
			spacingVelocity = cpvnormalize (spacingVelocity);
			spacingVelocity = cpvmult (spacingVelocity, bullets[gam_getArrayIndex (m_bulletID)].sprite.getFrameWidth () * 0.5f);
			tempWorldPos    = cpvsub (tempWorldPos, spacingVelocity);

			particles.emplace_back (PARTICLE_TYPE_TRAIL, tempWorldPos);
		}
	}

	m_canBeRemoved = true;
	//
	// Check to see if all the particles are dead so the emitter can be removed
	for (auto &partItr: particles)
	{
		if (partItr.isAlive ())
		{
			partItr.animate ();
			m_canBeRemoved = false;
		}
	}
	//
	// All particles are dead - clear the array and mark emitter can be removed
	if (m_canBeRemoved)
		particles.clear ();
}