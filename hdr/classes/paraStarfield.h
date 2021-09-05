#ifndef PARA_PARASTARFIELD_H
#define PARA_PARASTARFIELD_H

#include <SDL_rect.h>
#include <vector>
#include "paraRandom.h"
#include "paraRenderer.h"

struct backgroundStar
{
	SDL_Point pos;
	int       depth;
};

class paraStarfield
{

public:
	paraStarfield ();

	~paraStarfield ();

	void init (paraRenderer starRenderer, int numOfStars, int starDepth, int starTop, int starBottom, int starScreenWidth);

	void animate ();

	void render ();

private:
	int                         m_boundaryBottomY{};
	int                         m_boundaryTopY{};
	int                         m_depthSpread{};
	int                         m_starScreenWidth{};
	int                         m_depthRender{};
	paraRenderer                m_starRenderer{};
	std::vector<backgroundStar> stars;
	paraRandom                  randomBackgroundStar;
};

#endif //PARA_PARASTARFIELD_H
