#include "system/util.h"
#include "game/shipDecks.h"
#include "game/audio.h"
#include "game/player.h"
#include "game/doors.h"

double doorAnimSpeed {1.0f};
float  distanceForDoorSoundMax {120};   // In pixels

std::vector<doorTrigger_> doorTriggers {};

// ----------------------------------------------------------------------------
//
// Check door trigger areas against sprite positions
void gam_doorCheckTriggerAreas()
// ----------------------------------------------------------------------------
{
	if (0 == doorTriggers.size ())
		return;        // no doors on this level to process

	for (auto &doorItr: doorTriggers)
	{
		if ((playerDroid.ai2.getPreviousWorldPosInPixels ().x > doorItr.topLeft.x) && (playerDroid.ai2.getPreviousWorldPosInPixels ().y > doorItr.topLeft.y) && (playerDroid.ai2.getPreviousWorldPosInPixels ().x < doorItr.botRight.x) &&
		    (playerDroid.ai2.getPreviousWorldPosInPixels ().y < doorItr.botRight.y))
		{    // player sprite is inside a trigger area
			doorItr.inUse = true;
		}
		else
		{
			doorItr.inUse = false;    // this will reset all the doors the player is not in
		}
	}

	//
	// now check all the enemy sprites against the doors
	//
	for (auto &doorItr: doorTriggers)
	{
		for (auto droidItr: g_shipDeckItr->second.droid)
		{
			if (droidItr.getCurrentMode () == DROID_MODE_NORMAL)
			{
				if ((droidItr.getWorldPosInPixels ().x + (SPRITE_SIZE / 2) > doorItr.topLeft.x) && (droidItr.getWorldPosInPixels ().y + (SPRITE_SIZE / 2) > doorItr.topLeft.y) &&
				    (droidItr.getWorldPosInPixels ().x + (SPRITE_SIZE / 2) < doorItr.botRight.x) && (droidItr.getWorldPosInPixels ().y + (SPRITE_SIZE / 2) < doorItr.botRight.y))
				{
					doorItr.inUse = true;
				}
			}
		}
	}
}

// ----------------------------------------------------------------------------
//
// Play a door sound - taking into account distance and orientation to player
void gam_playDoorSound(int whichTrigger, std::string keyName)
// ----------------------------------------------------------------------------
{
	cpFloat distanceToDoor;
	float   distanceSoundLevel;
	float   distanceOrientation;

	//
	// How far is player from this door - attenuate for distance
	distanceToDoor = cpvdist (doorTriggers[whichTrigger].worldPosition, playerDroid.getWorldPosInPixels ());

	if (distanceToDoor > distanceForDoorSoundMax)
		distanceSoundLevel = 254;
	else
	{
		distanceSoundLevel = 254 * (distanceForDoorSoundMax / 10.0f);
		distanceSoundLevel = 255 - distanceSoundLevel;
	}
	//
	// Which side should the sound come from - attenuate for distance
	if (doorTriggers[whichTrigger].worldPosition.x < playerDroid.getWorldPosInPixels ().x)   // Door is to the left
	{
		if (distanceToDoor > distanceForDoorSoundMax)
			distanceOrientation = 254;  // All the way to the left
		else
		{
			distanceOrientation = 127 * 1.0f - (distanceForDoorSoundMax / 10.0f);
			distanceOrientation = 254 - distanceOrientation;
		}
	}
	else
	{
		if (distanceToDoor > distanceForDoorSoundMax)
			distanceOrientation = 0;        // All the way to the right
		else
		{
			distanceOrientation = 127 * 1.0f - (distanceForDoorSoundMax / 10.0f);
		}
	}

	if (distanceSoundLevel < 0)
		distanceSoundLevel = 0;

	gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, static_cast<int>(distanceSoundLevel), static_cast<int>(distanceOrientation), keyName);
}

// ----------------------------------------------------------------------------
//
// Process all the doors that are currently inUse
void gam_doorProcessActions()
// ----------------------------------------------------------------------------
{
	int i;

	if (doorTriggers.empty ())
		return;

	for (i = 0; i < static_cast<int>(doorTriggers.size ()); i++)
	{
		if (doorTriggers[i].inUse)
		{
			doorTriggers[i].frameDelay -= doorAnimSpeed * (1.0f / TICKS_PER_SECOND);
			if (doorTriggers[i].frameDelay > 0.0f)
			{
				doorTriggers[i].frameDelay = 1.0f;
				switch (doorTriggers[i].currentFrame)
				{
					case DOOR_ACROSS_CLOSED:
						doorTriggers[i].currentFrame = DOOR_ACROSS_OPEN_1;
						gam_playDoorSound (1, "doorOpen");
						break;
					case DOOR_ACROSS_OPEN_1:
						doorTriggers[i].currentFrame = DOOR_ACROSS_OPEN_2;
						break;
					case DOOR_ACROSS_OPEN_2:
						doorTriggers[i].currentFrame = DOOR_ACROSS_OPENED;
						break;
					case DOOR_ACROSS_OPENED:
						doorTriggers[i].currentFrame = DOOR_ACROSS_OPENED;
						break;
					case DOOR_ACROSS_CLOSING_1:
						doorTriggers[i].currentFrame = DOOR_ACROSS_OPENED;
						break;
					case DOOR_ACROSS_CLOSING_2:
						doorTriggers[i].currentFrame = DOOR_ACROSS_CLOSING_1;
						break;

					case DOOR_UP_CLOSED:
						doorTriggers[i].currentFrame = DOOR_UP_OPEN_1;
						gam_playDoorSound (1, "doorOpen");
						break;
					case DOOR_UP_OPEN_1:
						doorTriggers[i].currentFrame = DOOR_UP_OPEN_2;
						break;
					case DOOR_UP_OPEN_2:
						doorTriggers[i].currentFrame = DOOR_UP_OPENED;
						break;
					case DOOR_UP_OPENED:
						doorTriggers[i].currentFrame = DOOR_UP_OPENED;
						break;
					case DOOR_UP_CLOSING_1:
						doorTriggers[i].currentFrame = DOOR_UP_OPENED;
						break;
					case DOOR_UP_CLOSING_2:
						doorTriggers[i].currentFrame = DOOR_UP_CLOSING_1;
						break;

				}    // end of switch statement
				g_shipDeckItr->second.tiles[doorTriggers[i].tileIndex] = doorTriggers[i].currentFrame;
				gam_changeDoorFilters (doorTriggers[i].currentFrame, i);
			}    // end of nextFrame test
		}    // end of inUse is true test
		else
		{    // trigger is not in use
			doorTriggers[i].frameDelay -= doorAnimSpeed * (1.0f / TICKS_PER_SECOND);
			if (doorTriggers[i].frameDelay > 0.0f)
			{
				doorTriggers[i].frameDelay = 1.0f;
				switch (doorTriggers[i].currentFrame)
				{
					case DOOR_ACROSS_OPENED:
						doorTriggers[i].currentFrame = DOOR_ACROSS_CLOSING_1;
						gam_playDoorSound (1, "doorClose");
						break;
					case DOOR_ACROSS_CLOSING_1:
						doorTriggers[i].currentFrame = DOOR_ACROSS_CLOSING_2;
						break;
					case DOOR_ACROSS_CLOSING_2:
						doorTriggers[i].currentFrame = DOOR_ACROSS_CLOSED;
						break;

					case DOOR_ACROSS_OPEN_1:
						doorTriggers[i].currentFrame = DOOR_ACROSS_CLOSING_1;
						break;
					case DOOR_ACROSS_OPEN_2:
						doorTriggers[i].currentFrame = DOOR_ACROSS_CLOSING_2;
						break;

					case DOOR_UP_OPENED:
						doorTriggers[i].currentFrame = DOOR_UP_CLOSING_1;
						gam_playDoorSound (1, "doorClose");
						break;
					case DOOR_UP_CLOSING_1:
						doorTriggers[i].currentFrame = DOOR_UP_CLOSING_2;
						break;
					case DOOR_UP_CLOSING_2:
						doorTriggers[i].currentFrame = DOOR_UP_CLOSED;
						break;

					case DOOR_UP_OPEN_1:
						doorTriggers[i].currentFrame = DOOR_UP_CLOSING_1;
						break;
					case DOOR_UP_OPEN_2:
						doorTriggers[i].currentFrame = DOOR_UP_CLOSING_2;
						break;
				}
				g_shipDeckItr->second.tiles[doorTriggers[i].tileIndex] = doorTriggers[i].currentFrame;
				gam_changeDoorFilters (doorTriggers[i].currentFrame, i);
			}
		}
	}    // end of for each door loop
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render current door frames onto map
void gam_renderDoorFrames()
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_Texture *tempTexture;

	tempTexture = SDL_GetRenderTarget (renderer.renderer);

	SDL_SetRenderTarget (renderer.renderer, gam_getPlayfieldTexture ());

	for (const auto &doorIndex: doorTriggers)
	{
		if (sys_visibleOnScreen (doorIndex.renderPosition, tileSize))
		{
			gam_renderSingleTile (doorIndex.renderPosition.x, doorIndex.renderPosition.y, doorIndex.currentFrame);
		}
	}
	SDL_SetRenderTarget (renderer.renderer, tempTexture);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Find the doors for this deck and prepare a sensor object
void gam_doorTriggerSetup()
//----------------------------------------------------------------------------------------------------------------------
{
	int i {0};
	int currentTile {0};
	int sourceX {0};
	int sourceY {0};

	doorTrigger_ tempDoorTrigger;
	doorTrigger_ tempDoorBulletSensor;

	if (!doorTriggers.empty ())  // Empty out array
	{
		sys_clearAllDoors ();
	}

	sourceX = 0.0f;
	sourceY = 0.0f;
	for (i  = 0; i != g_shipDeckItr->second.levelDimensions.x * g_shipDeckItr->second.levelDimensions.y; i++)
	{
		currentTile = g_shipDeckItr->second.tiles[i];
		if (currentTile < 0)
		{
			sys_shutdownWithError (sys_getString ("Tile in doorTriggerSetup is invalid - Tile [ %i ]", i));
		}

		switch (currentTile)
		{
			case DOOR_ACROSS_CLOSED:
			case DOOR_ACROSS_OPEN_1:
			case DOOR_ACROSS_OPEN_2:
			case DOOR_ACROSS_OPENED:
			case DOOR_ACROSS_CLOSING_1:
			case DOOR_ACROSS_CLOSING_2:
			{
				tempDoorTrigger.height = tileSize;
				tempDoorTrigger.width  = tileSize * 2;

				tempDoorTrigger.topLeft.x = (sourceX * tileSize) - (tileSize / 2);
				tempDoorTrigger.topLeft.y = (sourceY * tileSize) - (tileSize);

				tempDoorTrigger.topRight.x = (sourceX * tileSize) + (tileSize) + (tileSize / 2);
				tempDoorTrigger.topRight.y = (sourceY * tileSize) - (tileSize);

				tempDoorTrigger.botLeft.x = (sourceX * tileSize) - (tileSize / 2);
				tempDoorTrigger.botLeft.y = (sourceY * tileSize) + (tileSize * 2);

				tempDoorTrigger.botRight.x = (sourceX * tileSize) + (tileSize) + (tileSize / 2);
				tempDoorTrigger.botRight.y = (sourceY * tileSize) + (tileSize * 2);

				tempDoorTrigger.worldPosition.x  = ((sourceX * tileSize) + (tileSize / 2));
				tempDoorTrigger.worldPosition.y  = ((sourceY * tileSize) + (tileSize / 2));
				tempDoorTrigger.renderPosition.x = sourceX * tileSize;
				tempDoorTrigger.renderPosition.y = sourceY * tileSize;
				tempDoorTrigger.tileIndex        = i;
				tempDoorTrigger.frameDelay       = 1.0f;
				tempDoorTrigger.inUse            = false;
				tempDoorTrigger.currentFrame     = DOOR_ACROSS_CLOSED;
				doorTriggers.push_back (tempDoorTrigger);

				tempDoorBulletSensor.height          = tileSize / 2;
				tempDoorBulletSensor.width           = tileSize;
				tempDoorBulletSensor.worldPosition.x = ((sourceX * tileSize) + (tileSize / 2));
				tempDoorBulletSensor.worldPosition.y = ((sourceY * tileSize) + (tileSize / 2));
				tempDoorBulletSensor.currentFrame    = DOOR_ACROSS_CLOSED;
				tempDoorBulletSensor.direction       = DIRECTION_ACROSS;
				sys_createDoorBulletSensor (sys_addNewDoorSensor (tempDoorBulletSensor));
				break;
			}

			case DOOR_UP_CLOSED:
			case DOOR_UP_OPEN_1:
			case DOOR_UP_OPEN_2:
			case DOOR_UP_OPENED:
			case DOOR_UP_CLOSING_1:
			case DOOR_UP_CLOSING_2:
			{
				tempDoorTrigger.height = tileSize * 2;
				tempDoorTrigger.width  = tileSize;

				tempDoorTrigger.topLeft.x = (sourceX * tileSize) - tileSize;
				tempDoorTrigger.topLeft.y = (sourceY * tileSize) - (tileSize / 2);

				tempDoorTrigger.topRight.x = (sourceX * tileSize) + (tileSize * 2);
				tempDoorTrigger.topRight.y = (sourceY * tileSize) - (tileSize / 2);

				tempDoorTrigger.botLeft.x = (sourceX * tileSize) - tileSize;
				tempDoorTrigger.botLeft.y = (sourceY * tileSize) + (tileSize) + (tileSize / 2);

				tempDoorTrigger.botRight.x = (sourceX * tileSize) + (tileSize * 2);
				tempDoorTrigger.botRight.y = (sourceY * tileSize) + (tileSize) + (tileSize / 2);

				tempDoorTrigger.worldPosition.x  = ((sourceX * tileSize) + (tileSize / 2));
				tempDoorTrigger.worldPosition.y  = ((sourceY * tileSize) + (tileSize / 2));
				tempDoorTrigger.renderPosition.x = sourceX * tileSize;
				tempDoorTrigger.renderPosition.y = sourceY * tileSize;
				tempDoorTrigger.tileIndex        = i;
				tempDoorTrigger.frameDelay       = 1.0f;
				tempDoorTrigger.inUse            = false;
				tempDoorTrigger.currentFrame     = DOOR_UP_CLOSED;
				doorTriggers.push_back (tempDoorTrigger);

				tempDoorBulletSensor.height          = tileSize;
				tempDoorBulletSensor.width           = tileSize / 2;
				tempDoorBulletSensor.worldPosition.x = ((sourceX * tileSize) + (tileSize / 2));
				tempDoorBulletSensor.worldPosition.y = ((sourceY * tileSize) + (tileSize / 2));
				tempDoorBulletSensor.currentFrame    = DOOR_UP_CLOSED;
				tempDoorBulletSensor.direction       = DIRECTION_UP;
				sys_createDoorBulletSensor (sys_addNewDoorSensor (tempDoorBulletSensor));
				break;
			}

			default:
				break;
		}
		sourceX++;
		if (sourceX == (int) g_shipDeckItr->second.levelDimensions.x)
		{
			sourceX = 0;
			sourceY++;
		}
	}
}