#pragma once

#include "main.h"

//-----------------------------------------------------------------------------
//
// sideview of ship stuff - used for lifts and database view
//
//-----------------------------------------------------------------------------

#define MAX_LEVELS            29
#define NUM_OF_TUNNELS        8

//-----------------------------------------------------------------------------
//
// Structure for sideview of ship
//
//-----------------------------------------------------------------------------

struct _basicTunnel
{
	int top;
	int bottom;
	int current;
	int current_deck;
	int decks[6];
};

extern _basicTunnel tunnel[NUM_OF_TUNNELS];

struct _sideviewBasicLevel
{
	float x1;
	float y1;
	float x2;
	float y2;
	int   up;
	int   down;
};

struct _sideviewColors
{
	__PARA_COLOR color;
};

struct _star
{
	SDL_Point   pos;
	int         depth;
};

extern float sideviewDrawScale;

// Create a color into the sideview array
void gui_createSideViewColor (int index, int red, int green, int blue, int alpha);

// load the sideview data from the external file
bool gui_loadSideViewData (std::string sideviewFileName);

// Show the ship in its sideview on the screen
void gui_renderSideView ();

// Render the starfield
void gui_renderStarfield();

// Prepare the starfield
void gui_prepareStarfield (int numStars, int numDepth);

// Animate the starfield
void gui_animateStarfield();