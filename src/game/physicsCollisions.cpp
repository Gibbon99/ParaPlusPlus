#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include "game/physicsCollisions.h"

contactListener myContactListenerInstance;

paraDebugDraw g_paraDebugDraw;

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

}

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a sold polygon shape
void paraDebugDraw::DrawSolidPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
//----------------------------------------------------------------------------------------------------------------------
{

}

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a circle outline
void paraDebugDraw::DrawCircle (const b2Vec2 &center, float radius, const b2Color &color)
//----------------------------------------------------------------------------------------------------------------------
{
	circleRGBA (renderer.renderer, center.x, center.y, radius, color.r, color.g, color.b, color.a);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a solid circle shape
void paraDebugDraw::DrawSolidCircle (const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color)
//----------------------------------------------------------------------------------------------------------------------
{
	// TODO - Implement world size and physics in meters
	/*
	b2Vec2 tempPosition;

	tempPosition = center;
	tempPosition *= pixelsPerMeter;
	radius *= pixelsPerMeter;

	tempPosition = sys_worldToScreen (tempPosition, radius);
	*/

	filledCircleRGBA (renderer.renderer, center.x, center.y, radius, color.r, color.g, color.b, color.a);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a line segment
void paraDebugDraw::DrawSegment (const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color)
//----------------------------------------------------------------------------------------------------------------------
{
	lineRGBA (renderer.renderer, p1.x, p1.y, p2.x, p2.y, color.r, color.g, color.b, color.a);
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
	pixelRGBA (renderer.renderer, p.x, p.y, color.r, color.g, color.b, color.a);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Called when a contact is first made
void contactListener::BeginContact (b2Contact *contact)
//----------------------------------------------------------------------------------------------------------------------
{
	_userData *bodyUserData_A;
	_userData *bodyUserData_B;

	bodyUserData_A = static_cast<_userData *>(contact->GetFixtureA ()->GetBody ()->GetUserData ());
	bodyUserData_B = static_cast<_userData *>(contact->GetFixtureB ()->GetBody ()->GetUserData ());
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

}