#include "game/score.h"
#include "game/database.h"
#include "game/audio.h"
#include "game/player.h"
#include "game/lifts.h"
#include "game/shipDecks.h"
#include "system/util.h"
#include "game/healing.h"

double healingAnimSpeed {};
float  healingDelayCounter {};
int    healingAmountPerTick {};

std::vector<__tileSensor> healingTiles;

//----------------------------------------------------------------------------------------------------------------------
//
// Render current healing frames onto map
void gam_renderHealingFrames()
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_Texture *tempTexture;

	tempTexture = SDL_GetRenderTarget (renderer.renderer);

	SDL_SetRenderTarget (renderer.renderer, gam_getPlayfieldTexture ());

	for (const auto &healingItr: g_shipDeckItr->second.healing)
	{
		if (currentMode != MODE_GUI_DECKVIEW)
		{
			if (sys_visibleOnScreen (healingItr.worldPosInPixels, tileSize))
			{
				gam_renderSingleTile (healingItr.worldPosInPixels.x, healingItr.worldPosInPixels.y, healingItr.currentFrame);
			}
		}
		else
			gam_renderSingleTile (healingItr.worldPosInPixels.x, healingItr.worldPosInPixels.y, healingItr.currentFrame);
	}

	SDL_SetRenderTarget (renderer.renderer, tempTexture);
}

// ----------------------------------------------------------------------------
//
// Animate healing tiles on current level
void gam_animateHealing()
// ----------------------------------------------------------------------------
{
	if (!g_shipDeckItr->second.healing.empty ())
	{
		for (auto &healingItr: g_shipDeckItr->second.healing)
		{
			healingItr.frameDelay -= 1.0 * healingAnimSpeed;
			if (healingItr.frameDelay < 0.0f)
			{
				healingItr.frameDelay = 1.0f;
				healingItr.currentFrame++;
				if (healingItr.currentFrame > HEALING_TILE + 3)
					healingItr.currentFrame = HEALING_TILE;

				g_shipDeckItr->second.tiles[healingItr.pos] = healingItr.currentFrame;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Clear out memory and free bodies
void gam_clearHealing()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &healingItr: healingTiles)
	{
		if (healingItr.shape != nullptr)
		{
			cpSpaceRemoveShape (sys_returnPhysicsWorld (), healingItr.shape);
			cpShapeFree (healingItr.shape);
			healingItr.shape = nullptr;
		}
		if (healingItr.body != nullptr)
		{
			if (cpSpaceContainsBody (sys_returnPhysicsWorld(), healingItr.body))
				cpSpaceRemoveBody (sys_returnPhysicsWorld(), healingItr.body);

			cpBodyFree(healingItr.body);
			healingItr.body = nullptr;
		}
	}
	healingTiles.clear ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create a lift sensor
void gam_createHealingSensor(int whichHealingTile)
//----------------------------------------------------------------------------------------------------------------------
{
	healingTiles[whichHealingTile].body = cpBodyNewStatic ();
	cpBodySetPosition (healingTiles[whichHealingTile].body, healingTiles[whichHealingTile].worldPosition);

	healingTiles[whichHealingTile].shape = cpBoxShapeNew (healingTiles[whichHealingTile].body, healingTiles[whichHealingTile].width, healingTiles[whichHealingTile].height, 1.0f);
	cpSpaceAddShape (sys_returnPhysicsWorld (), healingTiles[whichHealingTile].shape);
	cpShapeSetCollisionType (healingTiles[whichHealingTile].shape, PHYSIC_TYPE_HEALING);
	cpShapeSetSensor (healingTiles[whichHealingTile].shape, cpTrue);

	healingTiles[whichHealingTile].userData            = std::make_shared<userData_> ();
	healingTiles[whichHealingTile].userData->userType  = cpShapeGetCollisionType (healingTiles[whichHealingTile].shape);
	healingTiles[whichHealingTile].userData->dataValue = whichHealingTile;
	cpShapeSetUserData (healingTiles[whichHealingTile].shape, healingTiles[whichHealingTile].userData.get ());
}

//--------------------------------------------------------------------------------------------------------------------------
//
// Find out where on the map the healing tiles are and create a physic sensor for the current level
void gam_findHealingTilesPhysics()
// --------------------------------------------------------------------------------------------------------------------------
{
	int          index;
	int          currentTile;
	int          countX, countY;
	int          countHealing;
	__tileSensor tempHealingPhysics;

	if (!healingTiles.empty ())
	{
		gam_clearHealing ();
	}

	countHealing = 0;
	countX       = 0;
	countY       = 0;
	for (index   = 0; index < (int) g_shipDeckItr->second.levelDimensions.x * (int) g_shipDeckItr->second.levelDimensions.y; index++)
	{
		currentTile = g_shipDeckItr->second.tiles[((countY * ((int) g_shipDeckItr->second.levelDimensions.x)) + countX)];

		switch (currentTile)
		{
			case HEALING_TILE:
			case HEALING_TILE + 1:
			case HEALING_TILE + 2:
			case HEALING_TILE + 3:
				tempHealingPhysics.worldPosition.x = (countX * tileSize) + (tileSize * 0.5f);
				tempHealingPhysics.worldPosition.y = (countY * tileSize) + (tileSize * 0.5f);

				tempHealingPhysics.width  = tileSize / 8;
				tempHealingPhysics.height = tileSize / 8;

				healingTiles.push_back (tempHealingPhysics);

				gam_createHealingSensor (healingTiles.size () - 1);

				countHealing++;
				break;

			default:
				break;
		}

		countX++;

		if (countX == (int) g_shipDeckItr->second.levelDimensions.x)
		{
			countX = 0;
			countY++;
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------
//
// Find out where on the map the healing tiles are
// Store in vector for this deck for animation
void gam_findHealingTiles(std::string deckName)
// --------------------------------------------------------------------------------------------------------------------------
{
	int           index;
	int           currentTile;
	int           countX, countY;
	int           countHealing;
	_basicHealing tempHealing {};
	__tileSensor  tempHealingPhysics;

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
				tempHealing.pos          = index;
				tempHealing.currentFrame = HEALING_TILE;
				tempHealing.frameDelay   = 0.0f;

				tempHealing.worldPosInPixels.x = countX * tileSize;
				tempHealing.worldPosInPixels.y = countY * tileSize;

				tempHealing.worldPosInMeters.x = (countX * tileSize) / pixelsPerMeter;
				tempHealing.worldPosInMeters.y = (countY * tileSize) / pixelsPerMeter;

				shipdecks.at (deckName).healing.push_back (tempHealing);

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
void gam_processHealingTile()
//----------------------------------------------------------------------------------------------------------------------
{
	static float healingDelay = 1.0f;

	if (!playerDroid.getOverHealingTile ())
		return;

	if (playerDroid.getCurrentHealth () < dataBaseEntry[playerDroid.getDroidType ()].maxHealth)
	{
		if (!audio.isPlaying ("energyHeal"))
		{
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, true, 0, 127, "energyHeal");
		}
	}

	healingDelay -= 1.0f * healingDelayCounter;

	if (healingDelay < 0.0f)
	{
		healingDelay = 1.0f;
		playerDroid.setCurrentHealth (playerDroid.getCurrentHealth () + healingAmountPerTick);
		gam_modifyScore (-1);
		if (playerDroid.getCurrentHealth () > dataBaseEntry[playerDroid.getDroidType ()].maxHealth)
		{
			playerDroid.setCurrentHealth (dataBaseEntry[playerDroid.getDroidType ()].maxHealth);
			gam_addAudioEvent (EVENT_ACTION_AUDIO_STOP, true, 0, 127, "energyHeal");
//			playerDroid.setOverHealingTile (false);
		}
		gam_checkPlayerHealth ();
	}
}