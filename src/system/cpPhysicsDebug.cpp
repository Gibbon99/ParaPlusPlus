#include "main.h"
#include "SDL2_gfxPrimitives.h"
#include "system/util.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a circle outline
void debugDrawCircle(cpVect pos, [[maybe_unused]]cpFloat angle, cpFloat radius, cpSpaceDebugColor outlineColor, [[maybe_unused]]cpSpaceDebugColor fillColor, [[maybe_unused]]void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	cpVect tempPosition {};

	tempPosition = sys_worldToScreen (pos, static_cast<int>(radius));

	circleRGBA (renderer.renderer, tempPosition.x, tempPosition.y, radius, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a single point
void debugDrawDot(cpFloat size, cpVect pos, cpSpaceDebugColor fillColor, [[maybe_unused]]void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	cpVect point1 {};

	point1.x = pos.x;
	point1.y = pos.y;

	point1 = sys_worldToScreen (point1, static_cast<int>(size));

	pixelRGBA (renderer.renderer, point1.x, point1.y, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
}


//----------------------------------------------------------------------------------------------------------------------
//
// Draw a debug polygon outline shape
void debugDrawPolygon(int vertexCount, const cpVect *vertices, [[maybe_unused]]cpFloat radius, cpSpaceDebugColor outlineColor, [[maybe_unused]]cpSpaceDebugColor fillColor, [[maybe_unused]]void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	std::vector<short> xCoords;
	std::vector<short> yCoords;
	cpVect             tempPosition;

	for (auto i = 0; i != vertexCount; i++)
	{
		tempPosition.x = vertices[i].x;
		tempPosition.y = vertices[i].y;

		tempPosition = sys_worldToScreen (tempPosition, 32);

		xCoords.push_back (tempPosition.x);
		yCoords.push_back (tempPosition.y);
	}
	polygonRGBA (renderer.renderer, &xCoords[0], &yCoords[0], vertexCount, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a debug polygon outline shape
void debugDrawSolidPolygon(int vertexCount, const cpVect *vertices, [[maybe_unused]]cpFloat radius, [[maybe_unused]]cpSpaceDebugColor outlineColor, cpSpaceDebugColor fillColor)
//----------------------------------------------------------------------------------------------------------------------
{
	std::vector<short> xCoords;
	std::vector<short> yCoords;
	cpVect             tempPosition;

	for (auto i = 0; i != vertexCount; i++)
	{
		tempPosition.x = vertices[i].x;
		tempPosition.y = vertices[i].y;

		tempPosition = sys_worldToScreen (tempPosition, 32);

		xCoords.push_back (tempPosition.x);
		yCoords.push_back (tempPosition.y);
	}
	filledPolygonRGBA (renderer.renderer, &xCoords[0], &yCoords[0], vertexCount, fillColor.r, fillColor.g, fillColor.b, fillColor.a);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a line segment
void debugDrawSegment(cpVect p1, cpVect p2, cpSpaceDebugColor color, [[maybe_unused]]void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	cpVect point1 {};
	cpVect point2 {};

	point1.x = p1.x;
	point1.y = p1.y;
	point2.x = p2.x;
	point2.y = p2.y;

	point1 = sys_worldToScreen (point1, static_cast<int>(point1.x - point2.x));
	point2 = sys_worldToScreen (point2, static_cast<int>(point1.x - point2.x));

	lineRGBA (renderer.renderer, point1.x, point1.y, point2.x, point2.y, color.r, color.g, color.b, color.a);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a fat line segment
void debugDrawFatSegment(cpVect p1, cpVect p2, [[maybe_unused]] double radius, [[maybe_unused]]cpSpaceDebugColor color, [[maybe_unused]]cpSpaceDebugColor color2, [[maybe_unused]]void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	cpVect point1 {};
	cpVect point2 {};

	point1.x = p1.x;
	point1.y = p1.y;
	point2.x = p2.x;
	point2.y = p2.y;

	point1 = sys_worldToScreen (point1, static_cast<int>(point1.x - point2.x));
	point2 = sys_worldToScreen (point2, static_cast<int>(point1.x - point2.x));

	lineRGBA (renderer.renderer, point1.x, point1.y, point2.x, point2.y, color.r, color.g, color.b, color.a);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return a color to draw a debug shape in
cpSpaceDebugColor d_colorForShape(cpShape *shape, [[maybe_unused]]cpDataPointer data)
//----------------------------------------------------------------------------------------------------------------------
{
	if (cpShapeGetSensor (shape))
		return cpSpaceDebugColor {255, 0, 255, 255};
	else
	{
		cpBody *body = cpShapeGetBody (shape);

		if (cpBodyIsSleeping (body))
			return cpSpaceDebugColor {88, 110, 117, 255};
		else
			return cpSpaceDebugColor {147, 161, 161, 255};
	}
}