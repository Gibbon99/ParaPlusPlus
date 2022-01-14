#include <utility>
#include "main.h"
#include "classes/paraSprite.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Set the current frame to render
void paraSprite::setCurrentFrame(int newCurrentFrame)
//----------------------------------------------------------------------------------------------------------------------
{
	currentFrame = newCurrentFrame;
}

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
	numFrames      = setNumFrames;
	animateSpeed   = setAnimateSpeed;
	currentFrame   = 0;
	animateCounter = 0;
	textureKeyName = std::move (setTextureKeyname);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a sprite at the passed in location
void paraSprite::render(float posX, float posY, float scale, Uint8 alphaMod)
//----------------------------------------------------------------------------------------------------------------------
{
	SDL_Rect                                            srcRect;
	SDL_FRect                                           destRect;
	static std::map<std::string, paraTexture>::iterator textureItr;

	try
	{
		if (nullptr == texturePtr)
		{
			texturePtr  = textures.at (textureKeyName).getTexture ();
			textureItr  = textures.find (textureKeyName);
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

		if (lowHealth)
		{
			SDL_SetTextureColorMod (textures.at (textureKeyName).getTexture (), lowHealthColor.r, lowHealthColor.g, lowHealthColor.b);
		}
		else
		{
			SDL_SetTextureColorMod (textures.at (textureKeyName).getTexture (), tintColor.r, tintColor.g, tintColor.b);
			SDL_SetTextureAlphaMod (textures.at (textureKeyName).getTexture (), alphaMod);
		}

		SDL_RenderCopyF (renderer.renderer, texturePtr, &srcRect, &destRect);
	}

	catch (std::out_of_range &outOfRange)
	{
		std::cout << "Texture not loaded to render sprite." << std::endl;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render a sprite at the passed in location
void paraSprite::render(float posX, float posY, float scale, double angle)
//----------------------------------------------------------------------------------------------------------------------
{
	SDL_Rect                                            srcRect;
	SDL_FRect                                           destRect;
	static std::map<std::string, paraTexture>::iterator textureItr;

	try
	{
		if (nullptr == texturePtr)
		{
			texturePtr  = textures.at (textureKeyName).getTexture ();
			textureItr  = textures.find (textureKeyName);
			frameWidth  = textureItr->second.getWidth () / numFrames;
			frameHeight = textureItr->second.getHeight ();
		}
		destRect.x = posX - (static_cast<float>(frameWidth) / 2.0f);
		destRect.y = posY - (static_cast<float>(frameHeight) / 2.0f);
		destRect.w = frameWidth * scale;
		destRect.h = frameHeight * scale;

		srcRect.x = frameWidth * currentFrame;
		srcRect.y = 0;
		srcRect.w = frameWidth;
		srcRect.h = frameHeight;

		if (lowHealth)
		{
			SDL_SetTextureColorMod (textures.at (textureKeyName).getTexture (), lowHealthColor.r, lowHealthColor.g, lowHealthColor.b);
		}
		else
		{
			SDL_SetTextureColorMod (textures.at (textureKeyName).getTexture (), tintColor.r, tintColor.g, tintColor.b);
		}

		if (textureItr->second.isLoaded ())
			SDL_RenderCopyExF (renderer.renderer, texturePtr, &srcRect, &destRect, angle, nullptr, SDL_FLIP_NONE);
	}

	catch (std::out_of_range &outOfRange)
	{
		std::cout << "Texture not loaded to render sprite." << std::endl;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Animate the sprite - return TRUE if reached end of the animation
bool paraSprite::animate()
//----------------------------------------------------------------------------------------------------------------------
{
	if (lowHealth)
		animateLowHealth ();

	animateCounter += 1.0 * animateSpeed;
	if (animateCounter > 1.0)
	{
		animateCounter = 0.0;
		currentFrame++;
		if (currentFrame == numFrames)
		{
			currentFrame = 0;
			return true;
		}
	}
	return false;
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
	tintColor.r = r;
	tintColor.g = g;
	tintColor.b = b;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the flag to say this sprite has low health
void paraSprite::setLowHealth(bool newState)
//----------------------------------------------------------------------------------------------------------------------
{
	if (newState != lowHealth)
	{
		lowHealthColor = tintColor;
	}

	lowHealth = newState;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Process the colors to signal low health
void paraSprite::animateLowHealth()
//----------------------------------------------------------------------------------------------------------------------
{
	static int   lowHealthColorFade = 255;
	static float lowHealthDelay     = 1.8f;
	static float lowHealthCounter   = 1.0f;
	static bool  lowHealthDirection = false;

	lowHealthCounter -= 1.0f * lowHealthDelay;
	if (lowHealthCounter < 0.0f)
	{
		lowHealthCounter = 1.0f;
		if (!lowHealthDirection)
		{
			lowHealthColorFade -= 10.0f;
			if (lowHealthColorFade < 100)
			{
				lowHealthColorFade = 0;
				lowHealthDirection = !lowHealthDirection;
			}
		}
		else
		{
			lowHealthColorFade += 10.0f;
			if (lowHealthColorFade > 254)
			{
				lowHealthColorFade = 254;
				lowHealthDirection = !lowHealthDirection;
			}
		}
		//
		// Set the real color
		lowHealthColor.g = static_cast<Uint8>(lowHealthColorFade);
		lowHealthColor.b = static_cast<Uint8>(lowHealthColorFade);
	}
}