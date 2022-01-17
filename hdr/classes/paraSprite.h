#pragma once

#include <vector>
#include <string>
#include "wrapper.h"

class paraSprite
{

public:
	void render(float posX, float posY, float scale, Uint8 alphaMod);

	void render(float posX, float posY, float scale, double angle);

	bool animate();

	void setAnimateSpeed(double newSpeed);

	void setCurrentFrame(int newCurrentFrame);

	void setTintColor(Uint8 r, Uint8 g, Uint8 b);

	void create(std::string setTextureKeyname, int setNumFrames, double setAnimateSpeed);

	int getFrameWidth();

	int getFrameHeight();

	void setLowHealth(bool newState);

	void animateLowHealth();

	// Update the frame dimension information
	void updateFrameInfo();

private:
	int         numFrames {-1};
	int         currentFrame {-1};
	float       frameWidth {-1};
	float       frameHeight {-1};
	bool        lowHealth {false};
	double      animateSpeed {-1.0};
	double      animateCounter {-1.0};
	std::string textureKeyName {};
	PARA_Texture *texturePtr {nullptr};
	PARA_Color   tintColor {};
	PARA_Color   lowHealthColor {};
};