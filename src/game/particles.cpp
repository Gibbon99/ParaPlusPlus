#include <box2d/b2_math.h>
#include "classes/paraParticle.h"
#include "game/particles.h"

std::vector<paraParticle>   particleEmitters;

//----------------------------------------------------------------------------------------------------------------------
//
// Reset all the emitters ready for a new deck
void gam_clearEmitters()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &emitterItr : particleEmitters)
	{
		emitterItr.setIsAttached(false);
		emitterItr.isDead(true);
//		delete emitterItr;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Add a new emitter to the array - newWorldPos is in meters
void gam_addEmitter(b2Vec2 newWorldPos, int newType, Uint32 newBulletID)
//----------------------------------------------------------------------------------------------------------------------
{
	paraParticle    tempEmitter(newWorldPos, newType, newBulletID);

	switch (newType)
	{
		case PARTICLE_TYPE_TRAIL:
			tempEmitter.setIsAttached(true);
			break;

		default:
			tempEmitter.setIsAttached(false);
			break;
	}

	if (particleEmitters.empty())
	{
		particleEmitters.push_back (tempEmitter);
		return;
	}

	for (auto &partItr : particleEmitters)
	{
		if (!partItr.inUse())
		{
			partItr = tempEmitter;
			return;
		}
	}

	particleEmitters.push_back(tempEmitter);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Remove dead emitters
void gam_removeDeadEmitters()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &partItr : particleEmitters)
	{
		if (partItr.isDead (false))
		{
			// TODO - Set inUse to false ?
//			delete partItr;
			return;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Remove an emitter from an attached bullet
void gam_removeEmitter(Uint32 whichBulletID)
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &partItr : particleEmitters)
	{
		if ((partItr.inUse()) && (!partItr.isDead (false)))
		{
 	 		if (partItr.getAttachedBullet () == whichBulletID)
			{
				partItr.setIsAttached (false);  // Emitter will die when all particles are gone
				return;
			}
		}
	}

//	sys_shutdownWithError(sys_getString("Attempting to remove an unknown emitter [ %i ]", whichBulletID));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render the particles
void gam_renderParticles()
 //----------------------------------------------------------------------------------------------------------------------
{
	for (auto emitterItr : particleEmitters)
	{
		if (emitterItr.inUse())
			emitterItr.render();
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Animate the particles
void gam_animateParticles()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &emitterItr : particleEmitters)
	{
		if (emitterItr.inUse())
			emitterItr.animate();
	}
}