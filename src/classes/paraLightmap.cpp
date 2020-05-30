#include <main.h>
#include <system/util.h>
#include <game/bullet.h>
#include "classes/paraLightmap.h"

std::vector<PARA_Color> bulletColors;

//-----------------------------------------------------------------------------------------------------------
//
// Populate the table holding the bullet lightmap colors
void setupBulletLightmapColors ()
//-----------------------------------------------------------------------------------------------------------
{
	PARA_Color tempColor;

	tempColor.r = 255;
	tempColor.g = 0;
	tempColor.b = 0;
	tempColor.a = 255;
	bulletColors.push_back (tempColor);
	tempColor.r = 255;
	tempColor.g = 153;
	tempColor.b = 0;
	tempColor.a = 255;
	bulletColors.push_back (tempColor);
	tempColor.r = 255;
	tempColor.g = 102;
	tempColor.b = 0;
	tempColor.a = 255;
	bulletColors.push_back (tempColor);
	tempColor.r = 153;
	tempColor.g = 153;
	tempColor.b = 0;
	tempColor.a = 255;
	bulletColors.push_back (tempColor);
	tempColor.r = 153;
	tempColor.g = 255;
	tempColor.b = 0;
	tempColor.a = 255;
	bulletColors.push_back (tempColor);
	tempColor.r = 0;
	tempColor.g = 153;
	tempColor.b = 51;
	tempColor.a = 255;
	bulletColors.push_back (tempColor);
	tempColor.r = 0;
	tempColor.g = 51;
	tempColor.b = 153;
	tempColor.a = 0;
	bulletColors.push_back (tempColor);
	tempColor.r = 0;
	tempColor.g = 51;
	tempColor.b = 81;
	tempColor.a = 255;
	bulletColors.push_back (tempColor);

}

//-----------------------------------------------------------------------------------------------------------
//
// Return inUse status for this emitter
bool paraLightmap::inUse ()
//-----------------------------------------------------------------------------------------------------------
{
	return isInUse;
}

//-----------------------------------------------------------------------------------------------------------
//
// Set the state of the isInUse flag
void paraLightmap::setInUseState (bool newState)
//-----------------------------------------------------------------------------------------------------------
{
	isInUse = newState;
}

//-----------------------------------------------------------------------------------------------------------
//
// Return the bullet this emitter may be linked to - return -1 if not attached
Uint32 paraLightmap::getAttachedBullet ()
//-----------------------------------------------------------------------------------------------------------
{
	if (!attachedToBullet)
		return 0;

	return bulletLink;
}

//-----------------------------------------------------------------------------------------------------------
//
// Constructor for a new lightmap instance - Pass in newWorldPos in world coords ( meters )
paraLightmap::paraLightmap (b2Vec2 newWorldPos, int newType, int newWhichBullet)
//-----------------------------------------------------------------------------------------------------------
{
	worldPosInPixels = sys_convertToPixels (newWorldPos);
	type             = newType;
	textureWidth     = textures.at ("lightmap").getWidth ();
	textureHeight    = textures.at ("lightmap").getHeight ();
	textureCache     = textures.at ("lightmap").getTexture ();

	switch (type)
	{
		case LIGHTMAP_TYPE_EXPLOSION:
			attachedToBullet = false;
			break;

		case LIGHTMAP_TYPE_SPARK:
			bulletLink       = 0;
			attachedToBullet = false;
			color.r = 220;      // Match particle color in paraParticle setup
			color.g = 190;
			color.b = 200;
			color.a = 254;
			colorIndexCounter     = 0.0f;
			colorIndexChangeSpeed = 19.0f;
			break;

		case LIGHTMAP_TYPE_BULLET:
			bulletLink            = newWhichBullet;
			attachedToBullet      = true;
			colorIndex            = 0;
			colorIndexCounter     = 0.0f;
			colorIndexChangeSpeed = 0.5f;
			alphaValueBullet      = 48;
			break;

		case LIGHTMAP_TYPE_ALERT:
			attachedToBullet = false;
			break;
	}

	isInUse = true;
}


//-----------------------------------------------------------------------------------------------------------
//
// Animate the lightmap
void paraLightmap::animate ()
//-----------------------------------------------------------------------------------------------------------
{
	float tempAlpha;

	switch (type)
	{
		case LIGHTMAP_TYPE_EXPLOSION:
			break;

		case LIGHTMAP_TYPE_SPARK:
			tempAlpha = color.a;
			tempAlpha -= colorIndexChangeSpeed;
			if (tempAlpha < 0)
			{
				isInUse = false;
			}
			color.a = static_cast<Uint8>(tempAlpha);
			break;

		case LIGHTMAP_TYPE_BULLET:
			worldPosInPixels = sys_convertToPixels (bullets[gam_getArrayIndex (bulletLink)].worldPosInMeters);
			colorIndexCounter += 1.0f * colorIndexChangeSpeed;
			if (colorIndexCounter > 1.0f)
			{
				colorIndexCounter = 0.0f;
				colorIndex++;
				if (colorIndex == NUM_BULLET_COLORS)
					colorIndex = 0;
				color          = bulletColors[colorIndex];
			}
			break;

		case LIGHTMAP_TYPE_ALERT:
			break;
	}
}

//-----------------------------------------------------------------------------------------------------------
//
// Render the lightmap
void paraLightmap::render ()
//-----------------------------------------------------------------------------------------------------------
{
	SDL_Rect destRectInPixels;
	b2Vec2   renderPosition;

	switch (type)
	{
		case LIGHTMAP_TYPE_EXPLOSION:
			break;

		case LIGHTMAP_TYPE_SPARK:
			renderPosition.x = worldPosInPixels.x - (static_cast<float>(textureWidth) / 2);
			renderPosition.y = worldPosInPixels.y - (static_cast<float>(textureHeight) / 2);

			renderPosition = sys_worldToScreen (renderPosition, textureWidth);

			destRectInPixels.x = static_cast<int>(renderPosition.x);
			destRectInPixels.y = static_cast<int>(renderPosition.y);
			destRectInPixels.w = textureWidth;
			destRectInPixels.h = textureHeight;

			SDL_SetTextureAlphaMod (textureCache, color.a);
			SDL_SetTextureColorMod (textureCache, color.r, color.g, color.b);
			textures.at ("lightmap").render (&destRectInPixels);
			break;

		case LIGHTMAP_TYPE_BULLET:
			renderPosition.x = worldPosInPixels.x - (static_cast<float>(textureWidth) / 2);
			renderPosition.y = worldPosInPixels.y - (static_cast<float>(textureHeight) / 2);

			renderPosition = sys_worldToScreen (renderPosition, textureWidth);

			destRectInPixels.x = static_cast<int>(renderPosition.x);
			destRectInPixels.y = static_cast<int>(renderPosition.y);
			destRectInPixels.w = textureWidth;
			destRectInPixels.h = textureHeight;

			SDL_SetTextureAlphaMod (textureCache, alphaValueBullet);
			SDL_SetTextureColorMod (textureCache, color.r, color.g, color.b);
			textures.at ("lightmap").render (&destRectInPixels);

			break;

		case LIGHTMAP_TYPE_ALERT:
			break;
	}
}