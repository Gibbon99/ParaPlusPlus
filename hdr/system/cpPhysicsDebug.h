#pragma once

#include <chipmunk/chipmunk_structs.h>

// Return a color to draw a debug shape in
cpSpaceDebugColor d_colorForShape(cpShape *shape, cpDataPointer data);

// Draw a line segment
void debugDrawSegment(cpVect p1, cpVect p2, cpSpaceDebugColor color, void *data);

void debugDrawFatSegment(cpVect p1, cpVect p2, double radius, cpSpaceDebugColor color, cpSpaceDebugColor color2, void *data);

// Draw a debug polygon outline shape
void debugDrawSolidPolygon(int vertexCount, const cpVect *vertices, cpFloat radius, cpSpaceDebugColor outlineColor, cpSpaceDebugColor fillColor);

// Draw a debug polygon outline shape
void debugDrawPolygon(int vertexCount, const cpVect *vertices, cpFloat radius, cpSpaceDebugColor outlineColor, cpSpaceDebugColor fillColor, void *data);

// Draw a single point
void debugDrawDot(cpFloat size, cpVect pos, cpSpaceDebugColor fillColor, void *data);

// Draw a circle outline
void debugDrawCircle(cpVect pos, cpFloat angle, cpFloat radius, cpSpaceDebugColor outlineColor, cpSpaceDebugColor fillColor, void *data);
