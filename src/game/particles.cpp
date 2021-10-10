#include <box2d/b2_math.h>
#include <classes/paraEmitter.h>
#include <system/util.h>
#include "game/particles.h"

std::vector<paraEmitter> particleEmitters;

//----------------------------------------------------------------------------------------------------------------------
//
// Reset all the emitters ready for a new deck
void gam_clearEmitters ()
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
void gam_addEmitter (b2Vec2 newWorldPos, int newType, Uint32 newBulletID)
//----------------------------------------------------------------------------------------------------------------------
{
	printf ("Number of emitters before emplace_back [ %zu ]\n", particleEmitters.size ());

	if (PARTICLE_TYPE_TRAIL == newType)
		particleEmitters.emplace_back (sys_convertMetersToPixels (newWorldPos), newType, newBulletID);
	else
		particleEmitters.emplace_back (newWorldPos, newType, newBulletID);

	printf ("Number of emitters after emplace_back [ %zu ]\n", particleEmitters.size ());

	printf ("Number of particles in array after addEmitter [ %zu ]\n", particleEmitters[0].getNumberParticles ());
}

//----------------------------------------------------------------------------------------------------------------------
//
// Remove dead emitters
void gam_removeDeadEmitters ()
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
void gam_removeEmitter (Uint32 whichBulletID)
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
void gam_renderParticles ()
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
void gam_animateParticles ()
//----------------------------------------------------------------------------------------------------------------------
{
	if (particleEmitters.size () > 0)
		printf ("BEFORE Processing emitter with [ %zu ] number of particles.\n", particleEmitters[0].getNumberParticles ());

	for (auto &emitterItr: particleEmitters)
	{
		emitterItr.process ();

		printf ("AFTER Processing emitter with [ %zu ] number of particles.\n", emitterItr.getNumberParticles ());
	}
}