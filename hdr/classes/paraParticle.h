#ifndef PARA_PARAPARTICLE_H
#define PARA_PARAPARTICLE_H

#include <string>
#include <vector>

#include <box2d/b2_math.h>
#include <box2d/b2_body.h>
#include <system/physics.h>

#define USE_LOOKUP_TABLE 1

class paraParticle
{
public:

	paraParticle (b2Vec2 newWorldPos, int newType, Uint32 newBulletID);

	~paraParticle ();

	b2Vec2 getCircleAngle ();

	bool inUse ();

	void setInUse(bool newState);

	bool isDead (bool b);

	Uint32 getAttachedBullet ();

	bool isAttached();

	void setIsAttached (bool newState);

	void animate ();

	void render ();

private:
	struct __PARTICLE
	{
		bool           isAlive = false;
		b2Vec2         direction;
		b2Vec2         worldPos;
		_PHYSIC_OBJECT physicObject;
		PARA_Color color;
	};

	int   particleRenderType             = PARTICLE_RENDER_PRIMITIVE;
	int   numInitialParticles            = 0;
	int   particleSpeedExplosionMin      = 0;
	int   particleSpeedExplosionMax      = 0;
	int   particleSpeedSparkMin          = 0;
	int   particleSpeedSparkMax          = 0;
	int   particleExplosionInitialNumMin = 0;
	int   particleExplosionInitialNumMax = 0;
	int   particleSparkInitialNumMin     = 0;
	int   particleSparkInitialNumMax     = 0;
	int   particleTrailInitialNumMin     = 0;
	int   particleTrailInitialNumMax     = 0;
	float particleElastic                = 0.0f;
	float particleFriction               = 0.0f;
	float particleMass                   = 0.0f;
	float particleSize                   = 0.0f;
	int   particleExplosionReduceRate    = 0;
	int   particleTrailReduceRate        = 0;
	int   particleSparkReduceRate        = 0;

	bool                    isDeadFlag        = false;
	bool                    isInUse           = false;
	bool                    usePhysics        = true;
//	bool                    useDirections     = true;
	Uint32                  bulletLink        = 0;
	bool                    attachedToBullet  = true;       // Use this to keep emitter alive until finished, as bullet is gone
//	unsigned int            numParticlesAlive = 0;
	unsigned int            type              = -1;
	std::string             image             = "";
	b2Vec2                  worldPos          = {0, 0};
	std::vector<__PARTICLE> particle;
};

#endif //PARA_PARAPARTICLE_H
