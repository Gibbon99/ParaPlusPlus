#ifndef PARA_PARASTARFIELD_H
#define PARA_PARASTARFIELD_H

#include <vector>
#include <random>
#include "paraRenderer.h"

struct starPoint
{
	int x;
	int y;
};

struct backgroundStar
{
	starPoint pos;
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
	std::random_device          randomDevice;
};

#endif //PARA_PARASTARFIELD_H
