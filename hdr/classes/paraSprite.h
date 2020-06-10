#ifndef PARA_PARASPRITE_H
#define PARA_PARASPRITE_H

#include <wrapper.h>
#include <vector>
#include <string>

class paraSprite
{

public:
	void render (double posX, double posY, double scale, Uint8 alphaMod);

	void render (double posX, double posY, double scale, double angle);

	bool animate ();

	void setAnimateSpeed (double newSpeed);

	void setCurrentFrame (int newCurrentFrame);

	void setTintColor (Uint8 r, Uint8 g, Uint8 b);

	void create (std::string setTextureKeyname, int setNumFrames, double setAnimateSpeed);

	int getFrameWidth ();

	int getFrameHeight ();

	void setLowHealth (bool newState);

	void animateLowHealth();

private:
	int         numFrames      = -1;
	int         currentFrame   = -1;
	int         frameWidth     = -1;
	int         frameHeight    = -1;
	bool        lowHealth      = false;
	double      animateSpeed   = -1.0;
	double      animateCounter = -1.0;
	std::string textureKeyName = "";
	PARA_Texture *texturePtr   = nullptr;
	PARA_Color   tintColor;
	PARA_Color   lowHealthColor;
};


#endif //PARA_PARASPRITE_H
