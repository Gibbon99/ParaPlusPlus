#pragma once

#include <string>
#include <vector>
#include "system/cpPhysics.h"

#define USE_LOOKUP_TABLE 1

class paraParticle
{
public:

	paraParticle(int newType, cpVect newWorldPos);

	~paraParticle();

	cpVect getCircleAngle() const;

	bool inUse();

	void setInUse(bool newState);

	bool isAlive() const;

	Uint32 getAttachedBullet();

	bool isAttached();

	void setIsAttached(bool newState);

	void animate();

	void render();

	//
	// Overload copy assignment operator
	paraParticle &operator=(const paraParticle &other)
	{
		if (this != &other)     // protect against invalid self-assignment
		{
			m_id                        = other.m_id;
			m_particleType              = other.m_particleType;
			particleSpeedExplosionMin   = other.particleSpeedExplosionMin;
			particleSpeedExplosionMax   = other.particleSpeedExplosionMax;
			particleSpeedSparkMin       = other.particleSpeedSparkMin;
			particleSpeedSparkMax       = other.particleSpeedSparkMax;
			particleTrailReduceRate     = other.particleTrailReduceRate;
			particleExplosionReduceRate = other.particleExplosionReduceRate;
			particleSparkReduceRate     = other.particleSparkReduceRate;
			particleElastic             = other.particleElastic;
			particleFriction            = other.particleFriction;
			particleMass                = other.particleMass;
			particleSize                = other.particleSize;
			m_isAlive                   = other.m_isAlive;
			m_direction                 = other.m_direction;
			m_worldPos                  = other.m_worldPos;
//			physicObject                = other.physicObject;
			color                       = other.color;
//			userData
		}

		return *this;
	}

	//
	// Copy constructor - deep-copy
	paraParticle(const paraParticle &p1)
	{
		m_id                        = p1.m_id;
		m_particleType              = p1.m_particleType;
		particleSpeedExplosionMin   = p1.particleSpeedExplosionMin;
		particleSpeedExplosionMax   = p1.particleSpeedExplosionMax;
		particleSpeedSparkMin       = p1.particleSpeedSparkMin;
		particleSpeedSparkMax       = p1.particleSpeedSparkMax;
		particleTrailReduceRate     = p1.particleTrailReduceRate;
		particleExplosionReduceRate = p1.particleExplosionReduceRate;
		particleSparkReduceRate     = p1.particleSparkReduceRate;
		particleElastic             = p1.particleElastic;
		particleFriction            = p1.particleFriction;
		particleMass                = p1.particleMass;
		particleSize                = p1.particleSize;
		m_isAlive                   = p1.m_isAlive;
		m_direction                 = p1.m_direction;
		m_worldPos                  = p1.m_worldPos;
//		physicObject                = p1.physicObject;
		color                       = p1.color;

//		return paraParticle;
	}

private:

	int                        m_id {};
	int                        m_particleType {};
	int                        particleSpeedExplosionMin {200};
	int                        particleSpeedExplosionMax {300};
	int                        particleSpeedSparkMin {250};
	int                        particleSpeedSparkMax {300};
	int                        particleTrailReduceRate {40};
	int                        particleExplosionReduceRate {10};
	int                        particleSparkReduceRate {8};
	float                      particleElastic {0.9f};
	float                      particleFriction {0.2f};
	float                      particleMass {0.05f};
	float                      particleSize {4.0f};
	bool                       m_isAlive {};
	cpVect                     m_direction {};
	cpVect                     m_worldPos {};
	cpBody                     *body {};
	cpShape                    *shape {};
	std::shared_ptr<_userData> userData {};

	PARA_Color color {};
};