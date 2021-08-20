#ifndef PARA_PARAPHYSICSDESTRUCTION_H
#define PARA_PARAPHYSICSDESTRUCTION_H


#include <b2_world_callbacks.h>
#include <cstdio>

class paraPhysicsDestruction
{

};

class destructionListener : public b2DestructionListener
{
	void SayGoodbye (b2Joint *joint)
	{
		// remove all references to joint.
	}

	void SayGoodbye (b2Fixture *fixture)
	{
		// remove all references to fixture.
		printf ("Fixture is being destroyed.\n");
	}
};

extern destructionListener myDestructionListener;

#endif //PARA_PARAPHYSICSDESTRUCTION_H
