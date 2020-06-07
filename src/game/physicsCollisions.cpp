#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include <game/shipDecks.h>
#include <system/util.h>
#include <game/player.h>
#include <system/gameEvents.h>
#include <game/particles.h>
#include "game/physicsCollisions.h"

#include "game/bullet.h"

bool doWallCollisions = true;

contactListener myContactListenerInstance;

paraDebugDraw g_paraDebugDraw;

//----------------------------------------------------------------------------------------------------------------------
//
// Set player vs wall collisions
void gam_setWallCollisions (bool newState)
//----------------------------------------------------------------------------------------------------------------------
{
	doWallCollisions = newState;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Default constructor
paraDebugDraw::paraDebugDraw () = default;
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
//
// Default deconstructor
paraDebugDraw::~paraDebugDraw () = default;
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a debug polygon outline shape
void paraDebugDraw::DrawPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
//----------------------------------------------------------------------------------------------------------------------
{
	std::vector<short> xCoords;
	std::vector<short> yCoords;
	b2Vec2             tempPosition;

	for (auto i = 0; i != vertexCount; i++)
	{
		tempPosition.x = vertices[i].x;
		tempPosition.y = vertices[i].y;
		tempPosition *= pixelsPerMeter;

		tempPosition = sys_worldToScreen (tempPosition, 32);

		xCoords.push_back (tempPosition.x);
		yCoords.push_back (tempPosition.y);
	}


	polygonRGBA (renderer.renderer, &xCoords[0], &yCoords[0], vertexCount, color.r * 255, color.g * 0, color.b * 255, color.a * 255);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a sold polygon shape
void paraDebugDraw::DrawSolidPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
//----------------------------------------------------------------------------------------------------------------------
{
	std::vector<short> xCoords;
	std::vector<short> yCoords;
	b2Vec2             tempPosition;

	for (auto i = 0; i != vertexCount; i++)
	{
		tempPosition.x = vertices[i].x;
		tempPosition.y = vertices[i].y;
		tempPosition *= pixelsPerMeter;

		tempPosition = sys_worldToScreen (tempPosition, 32);

		xCoords.push_back (tempPosition.x);
		yCoords.push_back (tempPosition.y);
	}

	filledPolygonRGBA (renderer.renderer, &xCoords[0], &yCoords[0], vertexCount, color.r * 0, color.g * 255, color.b * 255, color.a * 255);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a circle outline
void paraDebugDraw::DrawCircle (const b2Vec2 &center, float radius, const b2Color &color)
//----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2 tempPosition{};

	tempPosition.x = center.x;
	tempPosition.y = center.y;
	tempPosition *= pixelsPerMeter;
	radius *= static_cast<float>(pixelsPerMeter);

	tempPosition = sys_worldToScreen (tempPosition, static_cast<int>(radius));

	circleRGBA (renderer.renderer, tempPosition.x, tempPosition.y, radius, color.r * 255, color.g * 255, color.b * 255, color.a * 255);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a solid circle shape
void paraDebugDraw::DrawSolidCircle (const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color)
//----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2 tempPosition{};

	tempPosition.x = center.x;
	tempPosition.y = center.y;
	tempPosition *= pixelsPerMeter;
	radius *= static_cast<float>(pixelsPerMeter);

	tempPosition = sys_worldToScreen (tempPosition, static_cast<int>(radius));

	filledCircleRGBA (renderer.renderer, tempPosition.x, tempPosition.y, radius, color.r * 255, color.g * 255, color.b * 255, color.a * 255);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a line segment
void paraDebugDraw::DrawSegment (const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color)
//----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2 point1{};
	b2Vec2 point2{};

	point1.x = p1.x;
	point1.y = p1.y;
	point2.x = p2.x;
	point2.y = p2.y;

	point1 *= pixelsPerMeter;
	point2 *= pixelsPerMeter;

	point1 = sys_worldToScreen (point1, static_cast<int>(point1.x - point2.x));
	point2 = sys_worldToScreen (point2, static_cast<int>(point1.x - point2.x));

	lineRGBA (renderer.renderer, point1.x, point1.y, point2.x, point2.y, color.r * 255, color.g * 255, color.b * 255, color.a * 255);
}

//----------------------------------------------------------------------------------------------------------------------
//
//
void paraDebugDraw::DrawTransform (const b2Transform &xf)
//----------------------------------------------------------------------------------------------------------------------
{

}

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a single point
void paraDebugDraw::DrawPoint (const b2Vec2 &p, float size, const b2Color &color)
//----------------------------------------------------------------------------------------------------------------------
{
	b2Vec2 point1{};

	point1.x = p.x;
	point1.y = p.y;

	point1 *= pixelsPerMeter;

	point1 = sys_worldToScreen (point1, static_cast<int>(size));

	pixelRGBA (renderer.renderer, point1.x, point1.y, color.r * 255, color.g * 255, color.b * 255, color.a * 255);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Called when a contact is first made
void contactListener::BeginContact (b2Contact *contact)
//----------------------------------------------------------------------------------------------------------------------
{
	_userData *bodyUserData_A;
	_userData *bodyUserData_B;
	b2Vec2    renderPosition;

	bodyUserData_A = static_cast<_userData *>(contact->GetFixtureA ()->GetBody ()->GetUserData ());
	bodyUserData_B = static_cast<_userData *>(contact->GetFixtureB ()->GetBody ()->GetUserData ());

	switch (bodyUserData_A->userType)
	{
		case PHYSIC_TYPE_WALL:
		{
			if ((bodyUserData_B->userType == PHYSIC_TYPE_BULLET_ENEMY) || (bodyUserData_B->userType == PHYSIC_TYPE_BULLET_PLAYER))
			{
				renderPosition = bullets[gam_getArrayIndex (bodyUserData_B->ID)].worldPosInMeters;
				gam_addEvent (EVENT_ACTION_ADD_EMITTER, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, PARTICLE_TYPE_SPARK));
				gam_addEvent (EVENT_ACTION_ADD_LIGHTMAP, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, LIGHTMAP_TYPE_SPARK));
				gam_addEvent (EVENT_ACTION_REMOVE_BULLET, 0, sys_getString ("%i|", bodyUserData_B->ID));
				return;
			}
		}
			break;

		case PHYSIC_TYPE_BULLET_PLAYER:
		{
			if (bodyUserData_B->userType == PHYSIC_TYPE_ENEMY)
			{
				gam_addEvent (EVENT_ACTION_DAMAGE_TO_DROID, 0, sys_getString ("%i|%i|%i", bodyUserData_B->dataValue, PHYSIC_DAMAGE_BULLET, bodyUserData_A->dataValue));
				gam_addEvent (EVENT_ACTION_REMOVE_BULLET, 0, sys_getString ("%i|", bodyUserData_A->ID));
				return;
			}

			if (bodyUserData_B->userType == PHYSIC_TYPE_BULLET_ENEMY)
			{
				renderPosition = bullets[gam_getArrayIndex (bodyUserData_B->ID)].worldPosInMeters;
				gam_addEvent (EVENT_ACTION_ADD_EMITTER, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, PARTICLE_TYPE_SPARK));
				gam_addEvent (EVENT_ACTION_ADD_LIGHTMAP, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, LIGHTMAP_TYPE_SPARK));
				gam_addEvent (EVENT_ACTION_REMOVE_BULLET, 0, sys_getString ("%i|", bodyUserData_A->ID));
				gam_addEvent (EVENT_ACTION_REMOVE_BULLET, 0, sys_getString ("%i|", bodyUserData_B->ID));
				return;
			}

			if (bodyUserData_B->userType == PHYSIC_TYPE_WALL)
			{
				renderPosition = bullets[gam_getArrayIndex (bodyUserData_A->ID)].worldPosInMeters;
				gam_addEvent (EVENT_ACTION_ADD_EMITTER, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, PARTICLE_TYPE_SPARK));
				gam_addEvent (EVENT_ACTION_ADD_LIGHTMAP, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, LIGHTMAP_TYPE_SPARK));
				gam_addEvent (EVENT_ACTION_REMOVE_BULLET, 0, sys_getString ("%i|", bodyUserData_A->ID));
				return;
			}
		}
			break;


		case PHYSIC_TYPE_DOOR_BULLET:
			if ((bodyUserData_B->userType == PHYSIC_TYPE_BULLET_PLAYER) || (bodyUserData_B->userType == PHYSIC_TYPE_BULLET_ENEMY))
			{
				renderPosition = bullets[gam_getArrayIndex (bodyUserData_B->ID)].worldPosInMeters;
				gam_addEvent (EVENT_ACTION_ADD_EMITTER, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, PARTICLE_TYPE_SPARK));
				gam_addEvent (EVENT_ACTION_ADD_LIGHTMAP, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, LIGHTMAP_TYPE_SPARK));
				gam_addEvent (EVENT_ACTION_REMOVE_BULLET, 0, sys_getString ("%i|", bodyUserData_B->ID));
				return;
			}
			break;

		case PHYSIC_TYPE_TERMINAL:
			if (bodyUserData_B->userType == PHYSIC_TYPE_PLAYER)
			{
				playerDroid.overTerminalTile = true;
				return;
			}
			break;

		case PHYSIC_TYPE_LIFT:
			if (bodyUserData_B->userType == PHYSIC_TYPE_PLAYER)
			{
				playerDroid.overLiftTile = true;
				playerDroid.liftIndex    = bodyUserData_A->dataValue;
				return;
			}
			break;

		case PHYSIC_TYPE_PLAYER:
			if (!playerDroid.inTransferMode)
			{
				if (bodyUserData_B->userType == PHYSIC_TYPE_ENEMY)
				{
					// int targetDroid, int damageSource, int sourceDroid,
					gam_addEvent (EVENT_ACTION_DAMAGE_TO_DROID, 0, sys_getString ("%i|%i|%i", bodyUserData_B->dataValue, PHYSIC_DAMAGE_BUMP, -1));
					// TODO - do damage to player
				}
			}
			else
			{
				if (bodyUserData_B->userType == PHYSIC_TYPE_ENEMY)
				{
					gam_addEvent (EVENT_ACTION_GO_TRANSFER_MODE, 0, sys_getString ("%i|", bodyUserData_B->dataValue));
				}
			}
			break;

		case PHYSIC_TYPE_ENEMY:
			if (bodyUserData_B->userType == PHYSIC_TYPE_ENEMY)
			{
				if (!bodyUserData_B->ignoreCollision)
					gam_addEvent (EVENT_ACTION_DROID_COLLISION, 0, sys_getString ("%i|", bodyUserData_B->dataValue));
			}
			break;
	}

	//--------------------------------------------------------
	//
	// Test for second collision type
	//
	//--------------------------------------------------------
	switch (bodyUserData_B->userType)
	{
		case PHYSIC_TYPE_WALL:
		{
			if ((bodyUserData_A->userType == PHYSIC_TYPE_BULLET_ENEMY) || (bodyUserData_A->userType == PHYSIC_TYPE_BULLET_PLAYER))
			{
				renderPosition = bullets[gam_getArrayIndex (bodyUserData_A->ID)].worldPosInMeters;
				gam_addEvent (EVENT_ACTION_ADD_EMITTER, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, PARTICLE_TYPE_SPARK));
				gam_addEvent (EVENT_ACTION_ADD_LIGHTMAP, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, LIGHTMAP_TYPE_SPARK));
				gam_addEvent (EVENT_ACTION_REMOVE_BULLET, 0, sys_getString ("%i|", bodyUserData_A->ID));
				return;
			}
		}
			break;

		case PHYSIC_TYPE_BULLET_PLAYER:
		{
			if (bodyUserData_A->userType == PHYSIC_TYPE_ENEMY)
			{
				gam_addEvent (EVENT_ACTION_DAMAGE_TO_DROID, 0, sys_getString ("%i|%i|%i", bodyUserData_A->dataValue, PHYSIC_DAMAGE_BULLET, -1));
				gam_addEvent (EVENT_ACTION_REMOVE_BULLET, 0, sys_getString ("%i|", bodyUserData_B->ID));
				return;
			}

			if (bodyUserData_A->userType == PHYSIC_TYPE_BULLET_ENEMY)
			{
				renderPosition = bullets[gam_getArrayIndex (bodyUserData_A->ID)].worldPosInMeters;
				gam_addEvent (EVENT_ACTION_ADD_EMITTER, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, PARTICLE_TYPE_SPARK));
				gam_addEvent (EVENT_ACTION_ADD_LIGHTMAP, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, LIGHTMAP_TYPE_SPARK));
				gam_addEvent (EVENT_ACTION_REMOVE_BULLET, 0, sys_getString ("%i|", bodyUserData_B->ID));
				gam_addEvent (EVENT_ACTION_REMOVE_BULLET, 0, sys_getString ("%i|", bodyUserData_A->ID));
				return;
			}

			if (bodyUserData_A->userType == PHYSIC_TYPE_WALL)
			{
				gam_addEvent (EVENT_ACTION_REMOVE_BULLET, 0, sys_getString ("%i|", bodyUserData_B->ID));
				return;
			}
		}
			break;

		case PHYSIC_TYPE_DOOR_BULLET:
			if ((bodyUserData_A->userType == PHYSIC_TYPE_BULLET_PLAYER) || (bodyUserData_A->userType == PHYSIC_TYPE_BULLET_ENEMY))
			{
				renderPosition = bullets[gam_getArrayIndex (bodyUserData_A->ID)].worldPosInMeters;
				gam_addEvent (EVENT_ACTION_ADD_EMITTER, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, PARTICLE_TYPE_SPARK));
				gam_addEvent (EVENT_ACTION_ADD_LIGHTMAP, 0, sys_getString ("%f|%f|%i", renderPosition.x, renderPosition.y, LIGHTMAP_TYPE_SPARK));
				gam_addEvent (EVENT_ACTION_REMOVE_BULLET, 0, sys_getString ("%i|", bodyUserData_A->ID));
				return;
			}
			break;

		case PHYSIC_TYPE_TERMINAL:
			if (bodyUserData_A->userType == PHYSIC_TYPE_TERMINAL)
			{
				playerDroid.overTerminalTile = true;
				return;
			}
			break;

		case PHYSIC_TYPE_LIFT:
			if (bodyUserData_A->userType == PHYSIC_TYPE_PLAYER)
			{
				playerDroid.overLiftTile = true;
				playerDroid.liftIndex    = bodyUserData_B->dataValue;
				return;
			}
			break;

		case PHYSIC_TYPE_PLAYER:
			if (!playerDroid.inTransferMode)
			{
				if (bodyUserData_A->userType == PHYSIC_TYPE_ENEMY)
				{
					// int targetDroid, int damageSource, int sourceDroid,
					gam_addEvent (EVENT_ACTION_DAMAGE_TO_DROID, 0, sys_getString ("%i|%i|%i", bodyUserData_A->dataValue, PHYSIC_DAMAGE_BUMP, -1));
					// TODO - do damage to player
				}
			}
			else
			{
				if (bodyUserData_A->userType == PHYSIC_TYPE_ENEMY)
				{
					gam_addEvent (EVENT_ACTION_GO_TRANSFER_MODE, 0, sys_getString ("%i|", bodyUserData_A->dataValue));
				}
			}
			break;

		case PHYSIC_TYPE_ENEMY:
			if (bodyUserData_A->userType == PHYSIC_TYPE_ENEMY)
			{
				if (!bodyUserData_A->ignoreCollision)
					gam_addEvent (EVENT_ACTION_DROID_COLLISION, 0, sys_getString ("%i|", bodyUserData_A->dataValue));
			}
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Called when a contact is broken - no contact anymore
void contactListener::EndContact (b2Contact *contact)
//----------------------------------------------------------------------------------------------------------------------
{
	_userData *bodyUserData_A;
	_userData *bodyUserData_B;

	bodyUserData_A = static_cast<_userData *>(contact->GetFixtureA ()->GetBody ()->GetUserData ());
	bodyUserData_B = static_cast<_userData *>(contact->GetFixtureB ()->GetBody ()->GetUserData ());

	switch (bodyUserData_A->userType)
	{
		case PHYSIC_TYPE_TERMINAL:
			if (bodyUserData_B->userType == PHYSIC_TYPE_PLAYER)
			{
				playerDroid.overTerminalTile = false;
				return;
			}
			break;

		case PHYSIC_TYPE_LIFT:
			if (bodyUserData_B->userType == PHYSIC_TYPE_PLAYER)
			{
				playerDroid.overLiftTile = false;
				playerDroid.liftIndex    = -1;
				return;
			}
			break;
	}

	switch (bodyUserData_B->userType)
	{
		case PHYSIC_TYPE_TERMINAL:
			if (bodyUserData_A->userType == PHYSIC_TYPE_PLAYER)
			{
				playerDroid.overTerminalTile = false;
				return;
			}
			break;

		case PHYSIC_TYPE_LIFT:
			if (bodyUserData_A->userType == PHYSIC_TYPE_PLAYER)
			{
				playerDroid.overLiftTile = false;
				playerDroid.liftIndex    = -1;
				return;
			}
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Called before the collision resolution is run
//
// Sst contact->SetEnabled (false); to ignore collision
void contactListener::PreSolve (b2Contact *contact, const b2Manifold *manifold)
//----------------------------------------------------------------------------------------------------------------------
{
	_userData *bodyUserData_A;
	_userData *bodyUserData_B;

	bodyUserData_A = static_cast<_userData *>(contact->GetFixtureA ()->GetBody ()->GetUserData ());
	bodyUserData_B = static_cast<_userData *>(contact->GetFixtureB ()->GetBody ()->GetUserData ());

	switch (bodyUserData_A->userType)
	{
		case PHYSIC_TYPE_PLAYER:
			if (bodyUserData_B->userType == PHYSIC_TYPE_WALL)
			{
				contact->SetEnabled (doWallCollisions);
				return;
			}
			break;

		case PHYSIC_TYPE_ENEMY:
			if (bodyUserData_B->userType == PHYSIC_TYPE_ENEMY)
			{
				if (bodyUserData_A->ignoreCollision)
					contact->SetEnabled (false);
				return;
			}
			break;

		case PHYSIC_TYPE_BULLET_PLAYER:
		{
			if (bodyUserData_B->userType == PHYSIC_TYPE_ENEMY)
			{
//				gam_addEvent(EVENT_ACTION_DAMAGE_TO_DROID, 0, sys_getString("%i|%i|%i", bodyUserData_B->dataValue, PHYSIC_DAMAGE_BULLET, bodyUserData_A->dataValue));
//				gam_addEvent(EVENT_ACTION_REMOVE_BULLET, 0, sys_getString("%i|", bodyUserData_A->dataValue));

//				bullets[bodyUserData_A->dataValue].inUse = false;
//				contact->SetEnabled(false);

				return;
			}

			if (bodyUserData_B->userType == PHYSIC_TYPE_BULLET_ENEMY)
			{
				//				par_addEmitter (bullets[bodyUserData_B->dataValue].worldPos, PARTICLE_TYPE_SPARK, bodyUserData_B->dataValue);
//				gam_addEvent(EVENT_ACTION_REMOVE_BULLET, 0, sys_getString("%i|", bodyUserData_A->dataValue));
//				gam_addEvent(EVENT_ACTION_REMOVE_BULLET, 0, sys_getString("%i|", bodyUserData_B->dataValue));
//				contact->SetEnabled(false);
//				return;
			}

			if (bodyUserData_B->userType == PHYSIC_TYPE_WALL)
			{
//				gam_addEvent(EVENT_ACTION_REMOVE_BULLET, 0, sys_getString("%i|", bodyUserData_A->dataValue));

//				bullets[bodyUserData_A->dataValue].inUse = false;
//				contact->SetEnabled(false);
//				return;
			}
		}
			break;
	}

	switch (bodyUserData_B->userType)
	{
		case PHYSIC_TYPE_PLAYER:
			if (bodyUserData_A->userType == PHYSIC_TYPE_WALL)
			{
				contact->SetEnabled (doWallCollisions);
				return;
			}
			break;

		case PHYSIC_TYPE_ENEMY:
			if (bodyUserData_A->userType == PHYSIC_TYPE_ENEMY)
			{
				if (bodyUserData_B->ignoreCollision)
					contact->SetEnabled (false);
				return;
			}
			break;

		case PHYSIC_TYPE_BULLET_PLAYER:
		{
			if (bodyUserData_A->userType == PHYSIC_TYPE_ENEMY)
			{
//				gam_addEvent(EVENT_ACTION_DAMAGE_TO_DROID, 0, sys_getString("%i|%i|%i", bodyUserData_A->dataValue, PHYSIC_DAMAGE_BULLET, -1));
//				gam_addEvent(EVENT_ACTION_REMOVE_BULLET, 0, sys_getString("%i|", bodyUserData_B->dataValue));

//				bullets[bodyUserData_B->dataValue].inUse = false;
//				contact->SetEnabled(false);

//				return;
			}

			if (bodyUserData_A->userType == PHYSIC_TYPE_BULLET_ENEMY)
			{
				//				par_addEmitter (bullets[bodyUserData_B->dataValue].worldPos, PARTICLE_TYPE_SPARK, bodyUserData_B->dataValue);
//				gam_addEvent(EVENT_ACTION_REMOVE_BULLET, 0, sys_getString("%i|", bodyUserData_B->dataValue));
//				gam_addEvent(EVENT_ACTION_REMOVE_BULLET, 0, sys_getString("%i|", bodyUserData_A->dataValue));
//				contact->SetEnabled(false);
//				return;
			}

			if (bodyUserData_A->userType == PHYSIC_TYPE_WALL)
			{
//				gam_addEvent(EVENT_ACTION_REMOVE_BULLET, 0, sys_getString("%i|", bodyUserData_B->dataValue));

//				bullets[bodyUserData_B->dataValue].inUse = false;
//				contact->SetEnabled(false);

				return;
			}
		}
			break;
	}
}