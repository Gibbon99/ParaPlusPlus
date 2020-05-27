#include <main.h>

#include <utility>
#include "classes/paraSprite.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Return width of a frame
int paraSprite::getFrameWidth ()
//----------------------------------------------------------------------------------------------------------------------
{
	return frameWidth;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return height of a frame
int paraSprite::getFrameHeight ()
//----------------------------------------------------------------------------------------------------------------------
{
	return frameHeight;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create a sprite
void paraSprite::create (std::string setTextureKeyname, int setNumFrames, double setAnimateSpeed)
//----------------------------------------------------------------------------------------------------------------------
{
	numFrames      = setNumFrames;
	animateSpeed   = setAnimateSpeed;
	currentFrame   = 0;
	animateCounter = 0;
	textureKeyName = std::move (setTextureKeyname);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a sprite at the passed in location
void paraSprite::render (double posX, double posY, double scale)
//----------------------------------------------------------------------------------------------------------------------
{
	SDL_Rect  srcRect;
	SDL_FRect destRect;
	static std::map<std::string, paraTexture>::iterator textureItr;

	if (nullptr == texturePtr)
	{
		texturePtr = textures.at(textureKeyName).getTexture();
		textureItr = textures.find(textureKeyName);
		frameWidth  = textureItr->second.getWidth () / numFrames;
		frameHeight = textureItr->second.getHeight ();
	}

	destRect.x = posX - (frameWidth / 2);
	destRect.y = posY - (frameHeight / 2);
	destRect.w = frameWidth * scale;
	destRect.h = frameHeight * scale;

	srcRect.x = frameWidth * currentFrame;
	srcRect.y = 0;
	srcRect.w = frameWidth;
	srcRect.h = frameHeight;

	SDL_SetTextureColorMod (textures.at (textureKeyName).getTexture (), tintColor.r, tintColor.g, tintColor.b);

	SDL_RenderCopyF (renderer.renderer, texturePtr, &srcRect, &destRect);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a sprite at the passed in location
void paraSprite::render (double posX, double posY, double scale, double angle)
//----------------------------------------------------------------------------------------------------------------------
{
	SDL_Rect  srcRect;
	SDL_FRect destRect;
	static std::map<std::string, paraTexture>::iterator textureItr;

	if (nullptr == texturePtr)
	{
		texturePtr = textures.at(textureKeyName).getTexture();
		textureItr = textures.find(textureKeyName);
		frameWidth  = textureItr->second.getWidth () / numFrames;
		frameHeight = textureItr->second.getHeight ();
	}

	destRect.x = posX - (frameWidth / 2);
	destRect.y = posY - (frameHeight / 2);
	destRect.w = frameWidth * scale;
	destRect.h = frameHeight * scale;

	srcRect.x = frameWidth * currentFrame;
	srcRect.y = 0;
	srcRect.w = frameWidth;
	srcRect.h = frameHeight;

	SDL_SetTextureColorMod (textures.at (textureKeyName).getTexture (), tintColor.r, tintColor.g, tintColor.b);

	SDL_RenderCopyExF(renderer.renderer, texturePtr, &srcRect, &destRect, angle, nullptr, SDL_FLIP_NONE);
}


//----------------------------------------------------------------------------------------------------------------------
//
// Animate the sprite
void paraSprite::animate ()
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
void paraSprite::setAnimateSpeed (double newSpeed)
//----------------------------------------------------------------------------------------------------------------------
{
	animateSpeed = newSpeed;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the tint color to render the sprite in
void paraSprite::setTintColor (Uint8 r, Uint8 g, Uint8 b)
//----------------------------------------------------------------------------------------------------------------------
{
	tintColor.r = r;
	tintColor.g = g;
	tintColor.b = b;

	//SDL_SetTextureColorMod (textures.at (paraSprite::textureKeyName).getTexture (), r, g, b);
}
