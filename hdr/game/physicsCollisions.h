#pragma once

#include "../main.h"
#include "box2d.h"
#include "system/physics.h"

class contactListener : public b2ContactListener
{
	void BeginContact (b2Contact *contact) override;

	void EndContact (b2Contact *contact) override;

	void PreSolve (b2Contact *contact, const b2Manifold *manifold) override;
};

class paraDebugDraw : public b2Draw
{
public:
	paraDebugDraw ();

	~paraDebugDraw () override;

	void DrawPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;

	void DrawSolidPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;

	void DrawCircle (const b2Vec2 &center, float radius, const b2Color &color) override;

	void DrawSolidCircle (const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color) override;

	void DrawSegment (const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override;

	void DrawTransform (const b2Transform &xf) override;

	void DrawPoint (const b2Vec2 &p, float size, const b2Color &color) override;
};

extern paraDebugDraw g_paraDebugDraw;

extern contactListener myContactListenerInstance;

extern bool doWallCollisions;
