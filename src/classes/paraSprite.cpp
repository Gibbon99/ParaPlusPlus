#include <main.h>

#include <utility>
#include "classes/paraSprite.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Return width of a frame
int paraSprite::getFrameWidth()
//----------------------------------------------------------------------------------------------------------------------
{
	return frameWidth;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return height of a frame
int paraSprite::getFrameHeight()
//----------------------------------------------------------------------------------------------------------------------
{
	return frameHeight;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create a sprite
void paraSprite::create(std::string setTextureKeyname, int setNumFrames, double setAnimateSpeed)
//----------------------------------------------------------------------------------------------------------------------
{
	numFrames = setNumFrames;
	animateSpeed = setAnimateSpeed;
	currentFrame = 0;
	animateCounter = 0;
	textureKeyName = std::move(setTextureKeyname);

	frameWidth = textures.at(textureKeyName).getWidth() / numFrames;
	frameHeight = textures.at(textureKeyName).getHeight();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a sprite at the passed in location
void paraSprite::render (double posX, double posY, double scale)
//----------------------------------------------------------------------------------------------------------------------
{
	SDL_Rect    srcRect;
	SDL_Rect    destRect;

	destRect.x = static_cast<int>(posX);
	destRect.y = static_cast<int>(posY);
	destRect.w = static_cast<int>(frameWidth * scale);
	destRect.h = static_cast<int>(textures.at(textureKeyName).getHeight() * scale);

	srcRect.x = frameWidth * currentFrame;
	srcRect.y = 0;
	srcRect.w = frameWidth;
	srcRect.h = textures.at(textureKeyName).getHeight();

	SDL_RenderCopy(renderer.renderer, textures.at(textureKeyName).getTexture(), &srcRect, &destRect);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Animate the sprite
void paraSprite::animate()
//----------------------------------------------------------------------------------------------------------------------
{
	animateCounter += 1.0 * animateSpeed;
	if (animateCounter > 1.0)
	{
		animateCounter = 0.0;
		currentFrame++;
		if (currentFrame == numFrames)
			currentFrame = 0;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the animate speed
void paraSprite::setAnimateSpeed(double newSpeed)
//----------------------------------------------------------------------------------------------------------------------
{
	animateSpeed = newSpeed;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the tint color to render the sprite in
void paraSprite::setTintColor(Uint8 r, Uint8 g, Uint8 b)
//----------------------------------------------------------------------------------------------------------------------
{
	SDL_SetTextureColorMod(textures.at(paraSprite::textureKeyName).getTexture(), r, g, b);
}
