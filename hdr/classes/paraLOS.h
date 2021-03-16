#ifndef PARA_PARALOS_H
#define PARA_PARALOS_H

#include <box2d/b2_world_callbacks.h>
#include <system/physics.h>
#include <game/doors.h>

class RayCastAnyCallback : public b2RayCastCallback {
public:
	RayCastAnyCallback ()
	{
		m_hit = false;
	}

	float ReportFixture (b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float) override
	{

		b2Body *body     = fixture->GetBody ();
		auto   *userData = static_cast<_userData *>(body->GetUserData ());

		switch (userData->userType)
		{
			case PHYSIC_TYPE_WALL:
				m_hit = true;
				return 0.0f;
				break;

			case PHYSIC_TYPE_DOOR_BULLET:
			{
				//
				// Look through open doors, ignoring the bullet sensor
				if ((DOOR_ACROSS_OPENED == doorTriggers[userData->dataValue].currentFrame) ||
				    (DOOR_UP_OPENED == doorTriggers[userData->dataValue].currentFrame))
				{
					m_hit = false;
					return 1.0f;
				}
				else
				{
					m_hit = true;
					// At this point we have a hit, so we know the ray is obstructed.
					// By returning 0, we instruct the calling code to terminate the ray-cast.
					return 0.0f;
				}
			}
				break;
		}

		m_hit            = false;
		return 1.0f;
	}

	bool   m_hit;
	b2Vec2 m_point;
};

#endif //PARA_PARALOS_H
