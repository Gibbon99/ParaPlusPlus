#include <system/cpPhysics.h>
#include <wrapper.h>
#include <classes/paraRandom.h>
#include "paraParticle.h"

class paraEmitter
{
public:

	paraEmitter(cpVect newWorldPos, int newEmitterType, Uint32 newBulletID);

	~paraEmitter();

	void render();

	void animate();

	void process();

	bool getCanBeRemoved() const;

	void setCanBeRemoved(bool newValue);

	Uint32 getBulletID() const;

	size_t getNumberParticles();

	void setAttachedToBullet(bool newValue);

private:

	int particleRenderType = PARTICLE_RENDER_PRIMITIVE;

	int particleExplosionInitialNumMin {30};
	int particleExplosionInitialNumMax {90};
	int particleSparkInitialNumMin {15};
	int particleSparkInitialNumMax {35};
	int particleTrailInitialNumMin {30};
	int particleTrailInitialNumMax {90};

	bool                      m_attachedToBullet {};
	bool                      m_canBeRemoved {false};
	int                       m_emitterType {};
	Uint32                    m_bulletID {};
	cpVect                    m_worldPos {};
	std::vector<paraParticle> particles {};
};