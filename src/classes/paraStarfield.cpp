#include "classes/paraStarfield.h"

#include <utility>
#include <SDL2_gfxPrimitives.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Constructor
paraStarfield::paraStarfield ()
//----------------------------------------------------------------------------------------------------------------------
= default;

//----------------------------------------------------------------------------------------------------------------------
//
// Destroy the starField object - free vector memory
paraStarfield::~paraStarfield ()
//----------------------------------------------------------------------------------------------------------------------
{
	stars.clear ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Init a starfield - requires number of stars, required depth and screen size to contain stars within
void paraStarfield::init (paraRenderer starRenderer, int numOfStars, int starDepth, int starTop, int starBottom, int starScreenWidth)
//----------------------------------------------------------------------------------------------------------------------
{
	backgroundStar tempStar{};

	m_boundaryBottomY = starBottom;
	m_boundaryTopY    = starTop; // Get height of HUD here
	m_depthSpread     = starDepth;
	m_starScreenWidth = starScreenWidth;
	m_starRenderer    = std::move (starRenderer);
	m_depthRender     = 255 / starDepth;

	for (auto counter = 0; counter != numOfStars; counter++)
	{
		tempStar.pos.x = randomBackgroundStar.get (0, starScreenWidth);
		tempStar.pos.y = randomBackgroundStar.get (m_boundaryTopY, m_boundaryBottomY);
		tempStar.depth = randomBackgroundStar.get (1, m_depthSpread);
		stars.push_back (tempStar);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Animate the starfield
void paraStarfield::animate ()
//----------------------------------------------------------------------------------------------------------------------
{
	double speedPercent;
	double moveSpeed;
	double topSpeed = 4.0;

	for (auto &starItr: stars)
	{
		speedPercent = (double) starItr.depth / (double) m_depthSpread;
		moveSpeed    = topSpeed * speedPercent;
		starItr.pos.x -= moveSpeed;
		if (starItr.pos.x <= 0)
		{
			starItr.pos.x = m_starScreenWidth;
			starItr.pos.y = randomBackgroundStar.get (m_boundaryTopY, m_boundaryBottomY);
			starItr.depth = randomBackgroundStar.get (1, m_depthSpread);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render the starfield
void paraStarfield::render ()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto starItr: stars)
	{
		filledCircleRGBA (m_starRenderer.renderer, starItr.pos.x, starItr.pos.y, 2, starItr.depth * m_depthRender, starItr.depth * m_depthRender, starItr.depth * m_depthRender, SDL_ALPHA_OPAQUE);
	}
}
