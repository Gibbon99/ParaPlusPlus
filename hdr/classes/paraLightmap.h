#ifndef PARA_PARALIGHTMAP_H
#define PARA_PARALIGHTMAP_H

#include <box2d/b2_math.h>
#include <SDL_quit.h>
#include <wrapper.h>
#include <vector>

#define NUM_BULLET_COLORS 8

class paraLightmap
{

public:

	paraLightmap (b2Vec2 newWorldPos, int newType, int newWhichBullet);

	bool inUse ();

	void setInUseState (bool newState);

	Uint32 getAttachedBullet ();

	void animate ();

	void render ();

	int getType ();

	void setColor (int newColorType);

private:

	bool   isInUse               = false;
	bool   attachedToBullet      = false;
	int    type                  = -1;
	int    colorIndex            = -1;
	int    radius                = -1;
	int    alphaValueBullet      = 0;
	int    textureWidth          = 0;
	int    textureHeight         = 0;
	float  colorIndexChangeSpeed = 0.0f;
	float  colorIndexCounter     = 0.0f;
	Uint32 bulletLink            = 0;
	b2Vec2 worldPosInPixels;
	PARA_Texture *textureCache   = nullptr;
	PARA_Color   color;
};

// Populate the table holding the bullet lightmap colors
void setupBulletLightmapColors ();

#endif //PARA_PARALIGHTMAP_H
