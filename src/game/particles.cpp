#include <classes/paraEmitter.h>
#include "game/particles.h"

std::vector<paraEmitter> particleEmitters;

//----------------------------------------------------------------------------------------------------------------------
//
// Reset all the emitters ready for a new deck
void gam_clearEmitters()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &emitterItr: particleEmitters)
	{
//		delete emitterItr;
	}

	particleEmitters.clear ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Add a new emitter to the array - newWorldPos is in meters
void gam_addEmitter(cpVect newWorldPos, int newType, Uint32 newBulletID)
//----------------------------------------------------------------------------------------------------------------------
{
	particleEmitters.emplace_back (newWorldPos, newType, newBulletID);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Remove dead emitters
void gam_removeDeadEmitters()
//----------------------------------------------------------------------------------------------------------------------
{
	auto removeIndex = 0;

	for (auto &partItr: particleEmitters)
	{
		if (partItr.getCanBeRemoved ())
		{
			particleEmitters.erase (particleEmitters.begin () + removeIndex);
			removeIndex++;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Remove an emitter from an attached bullet
void gam_removeEmitter(Uint32 whichBulletID)
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &partItr: particleEmitters)
	{
		if (partItr.getBulletID () == whichBulletID)
		{
			partItr.setAttachedToBullet (false);
			return;
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
	if (0 == particleEmitters.size ())
		return;

	for (auto &emitterItr: particleEmitters)
	{
		emitterItr.render ();
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Animate the particles
void gam_animateParticles()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &emitterItr: particleEmitters)
	{
		emitterItr.process ();
	}
}