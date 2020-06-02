#include <game/shipDecks.h>
#include <system/util.h>
#include "game/healing.h"
#include "game/shipDecks.h"

double healingAnimSpeed;

//----------------------------------------------------------------------------------------------------------------------
//
// Render current healing frames onto map
void gam_renderHealingFrames (const std::string& deckName)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_Texture    *tempTexture;

	tempTexture = SDL_GetRenderTarget(renderer.renderer);

	SDL_SetRenderTarget(renderer.renderer, gam_getPlayfieldTexture());

	// TODO : Put in try / catch

	for (const auto &healingItr : shipdecks.at (deckName).healing)
	{
		if (sys_visibleOnScreen (healingItr.worldPosInPixels, tileSize))
		{
			gam_renderSingleTile (healingItr.worldPosInPixels.x, healingItr.worldPosInPixels.y, healingItr.currentFrame);
		}
	}

	SDL_SetRenderTarget(renderer.renderer, tempTexture);
}

// ----------------------------------------------------------------------------
//
// Animate healing tiles on current level
void gam_animateHealing (const std::string& deckName)
// ----------------------------------------------------------------------------
{
	if (!shipdecks.at (deckName).healing.empty ())
	{
		for (auto &healingItr: shipdecks.at (deckName).healing)
		{
			healingItr.frameDelay -= 1.0 * healingAnimSpeed;
			if (healingItr.frameDelay < 0.0f)
			{
				healingItr.frameDelay = 1.0f;
				healingItr.currentFrame++;
				if (healingItr.currentFrame > HEALING_TILE + 3)
					healingItr.currentFrame = HEALING_TILE;

				shipdecks.at (deckName).tiles[healingItr.pos] = healingItr.currentFrame;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------
//
// Find out where on the map the healing tiles are
// Store in vector for this deck for animation
void gam_findHealingTiles (std::string deckName)
// --------------------------------------------------------------------------------------------------------------------------
{
	int           index;
	int           currentTile;
	int           countX, countY;
	int           countHealing;
	_basicHealing tempHealing{};

	for (index = 0; index < shipdecks.at (deckName).levelDimensions.x * shipdecks.at (deckName).levelDimensions.y; index++)
	{
		switch (shipdecks.at (deckName).tiles[index])
		{
			case HEALING_TILE:
			case HEALING_TILE + 1:
			case HEALING_TILE + 2:
			case HEALING_TILE + 3:
				tempHealing.pos          = index;
				tempHealing.currentFrame = HEALING_TILE;
				tempHealing.frameDelay   = 0.0f;
				shipdecks.at (deckName).healing.push_back (tempHealing);
				break;

			default:
				break;
		}
	}

	countHealing = 0;
	countX       = 0;
	countY       = 0;
	for (index   = 0; index < (int) shipdecks.at (deckName).levelDimensions.x * (int) shipdecks.at (deckName).levelDimensions.y; index++)
	{
		currentTile = shipdecks.at (deckName).tiles[((countY * ((int) shipdecks.at (deckName).levelDimensions.x)) + countX)];

		switch (currentTile)
		{
			case HEALING_TILE:
			case HEALING_TILE + 1:
			case HEALING_TILE + 2:
			case HEALING_TILE + 3:
				shipdecks.at (deckName).healing[countHealing].worldPosInPixels.x = countX * tileSize;
				shipdecks.at (deckName).healing[countHealing].worldPosInPixels.y = countY * tileSize;

				shipdecks.at (deckName).healing[countHealing].worldPosInMeters.x = (countX * tileSize) / pixelsPerMeter;
				shipdecks.at (deckName).healing[countHealing].worldPosInMeters.y = (countY * tileSize) / pixelsPerMeter;

				countHealing++;
				break;

			default:
				break;
		}

		countX++;

		if (countX == (int) shipdecks.at (deckName).levelDimensions.x)
		{
			countX = 0;
			countY++;
		}
	}
}


