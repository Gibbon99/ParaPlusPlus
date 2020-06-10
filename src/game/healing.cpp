#include "game/score.h"
#include "game/database.h"
#include "game/audio.h"
#include "game/player.h"
#include "game/lifts.h"
#include "game/shipDecks.h"
#include "system/util.h"
#include "game/healing.h"

double healingAnimSpeed;
float  healingDelayCounter;
int    healingAmountPerTick;

std::vector<__tileSensor> healingTiles;

//----------------------------------------------------------------------------------------------------------------------
//
// Render current healing frames onto map
void gam_renderHealingFrames (const std::string &deckName)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_Texture *tempTexture;

	tempTexture = SDL_GetRenderTarget (renderer.renderer);

	SDL_SetRenderTarget (renderer.renderer, gam_getPlayfieldTexture ());

	// TODO : Put in try / catch

	for (const auto &healingItr : shipdecks.at (deckName).healing)
	{
		if (sys_visibleOnScreen (healingItr.worldPosInPixels, tileSize))
		{
			gam_renderSingleTile (healingItr.worldPosInPixels.x, healingItr.worldPosInPixels.y, healingItr.currentFrame);
		}
	}

	SDL_SetRenderTarget (renderer.renderer, tempTexture);
}

// ----------------------------------------------------------------------------
//
// Animate healing tiles on current level
void gam_animateHealing (const std::string &deckName)
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

//----------------------------------------------------------------------------------------------------------------------
//
// Create a lift sensor
void gam_createHealingSensor (unsigned long whichHealingTile, int index)
//----------------------------------------------------------------------------------------------------------------------
{
	healingTiles[whichHealingTile].bodyDef.type = b2_staticBody;
	healingTiles[whichHealingTile].bodyDef.position.Set (healingTiles[whichHealingTile].worldPosition.x / pixelsPerMeter, healingTiles[whichHealingTile].worldPosition.y / pixelsPerMeter);
	healingTiles[whichHealingTile].body = sys_getPhysicsWorld ()->CreateBody (&healingTiles[whichHealingTile].bodyDef);

	healingTiles[whichHealingTile].userData            = new _userData;
	healingTiles[whichHealingTile].userData->userType  = PHYSIC_TYPE_HEALING;
	healingTiles[whichHealingTile].userData->dataValue = (int) index;
	healingTiles[whichHealingTile].body->SetUserData (healingTiles[whichHealingTile].userData);

	healingTiles[whichHealingTile].shape.SetAsBox ((healingTiles[whichHealingTile].height) / pixelsPerMeter, (healingTiles[whichHealingTile].width) / pixelsPerMeter);
	healingTiles[whichHealingTile].fixtureDef.shape    = &healingTiles[whichHealingTile].shape;
	healingTiles[whichHealingTile].fixtureDef.isSensor = true;
	healingTiles[whichHealingTile].body->CreateFixture (&healingTiles[whichHealingTile].fixtureDef);
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
	__tileSensor  tempHealingPhysics;

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

				tempHealingPhysics.worldPosition.x = (countX * tileSize) + (tileSize * 0.5f);
				tempHealingPhysics.worldPosition.y = (countY * tileSize) + (tileSize * 0.5f);

				tempHealingPhysics.width  = tileSize / 8;
				tempHealingPhysics.height = tileSize / 8;

				healingTiles.push_back (tempHealingPhysics);

				gam_createHealingSensor (healingTiles.size () - 1, countHealing);

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

//----------------------------------------------------------------------------------------------------------------------
//
// Check if the player is over a healing tile
void gam_processHealingTile ()
//----------------------------------------------------------------------------------------------------------------------
{
	static float healingDelay = 1.0f;

	if (!playerDroid.overHealingTile)
		return;

	healingDelay -= 1.0f * healingDelayCounter;
	if (healingDelay < 0.0f)
	{
		healingDelay = 1.0f;
		playerDroid.currentHealth += 1;
		gam_modifyScore (-1);
		if (playerDroid.currentHealth > dataBaseEntry[playerDroid.droidType].maxHealth)
		{
			playerDroid.currentHealth = dataBaseEntry[playerDroid.droidType].maxHealth;
			gam_addAudioEvent (EVENT_ACTION_AUDIO_STOP, true, 0, 127, "energyHeal");
			playerDroid.overHealingTile = false;
		}
	}
}