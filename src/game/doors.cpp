#include <game/shipDecks.h>
#include <system/util.h>
#include <game/audio.h>
#include <game/player.h>
#include "game/doors.h"

double doorAnimSpeed = 1.0f;

std::vector<_doorTrigger> doorTriggers;
std::vector<_doorTrigger> doorBulletSensor;

// ----------------------------------------------------------------------------
//
// Check door trigger areas against sprite positions
void gam_doorCheckTriggerAreas ()
// ----------------------------------------------------------------------------
{
	int i = 0;
	int j = 0;

	if (0 == doorTriggers.size ())
		return;        // no doors on this level to process

	for (auto &doorItr : doorTriggers)
	{
		if ((playerDroid.previousWorldPosInPixels  .x > doorItr.topLeft.x) &&
		    (playerDroid.previousWorldPosInPixels.y > doorItr.topLeft.y) &&
		    (playerDroid.previousWorldPosInPixels.x < doorItr.botRight.x) &&
		    (playerDroid.previousWorldPosInPixels.y < doorItr.botRight.y))

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
	for (auto &doorItr : doorTriggers)
	{
		for (j = 0; j != shipdecks[gam_getCurrentDeckName()].numDroids; j++)
		{
			if (shipdecks.at (gam_getCurrentDeckName ()).droid[j].currentMode == DROID_MODE_NORMAL)
			{
				if ((shipdecks[gam_getCurrentDeckName ()].droid[j].worldPosInPixels.x + (24 / 2) > doorItr.topLeft.x) &&
				    (shipdecks[gam_getCurrentDeckName ()].droid[j].worldPosInPixels.y + (24 / 2) > doorItr.topLeft.y) &&
				    (shipdecks[gam_getCurrentDeckName ()].droid[j].worldPosInPixels.x + (24 / 2) < doorItr.botRight.x) &&
				    (shipdecks[gam_getCurrentDeckName ()].droid[j].worldPosInPixels.y + (24 / 2) < doorItr.botRight.y))
				{
					doorItr.inUse = true;
				}
			}
		}
	}
}


//----------------------------------------------------------------------------------------------------------------------
//
// Change the collision filters for a door
// Used by bullet to see if it passes through an open door
void gam_changeDoorFilters (int doorState, int whichDoor)
//----------------------------------------------------------------------------------------------------------------------
{
	b2Fixture *fixture = doorBulletSensor[whichDoor].body->GetFixtureList ();
	//
	// Get the existing filter
	b2Filter  filter   = fixture->GetFilterData ();

	switch (doorState)
	{
		case DOOR_ACROSS_OPENED:
		case DOOR_UP_OPENED:
			filter.categoryBits = PHYSIC_TYPE_DOOR_OPEN;
			break;

		default:
			filter.categoryBits = PHYSIC_TYPE_DOOR_CLOSED;
			break;
	}
	//
	// Set the updated category
	fixture->SetFilterData (filter);
}


// ----------------------------------------------------------------------------
//
// Process all the doors that are currently inUse
void gam_doorProcessActions ()
// ----------------------------------------------------------------------------
{
	int i;
//	int doorDelayTime = 0;

	if (doorTriggers.empty())
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
						gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "doorOpen.wav");
						//gam_playDoorSound(i);
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
						gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "doorOpen.wav");
						//gam_playDoorSound(i);
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
				shipdecks.at (gam_getCurrentDeckName()).tiles[doorTriggers[i].tileIndex] = doorTriggers[i].currentFrame;
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
//						gam_playDoorSound(i);
						doorTriggers[i].currentFrame = DOOR_ACROSS_CLOSING_1;
						gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "doorClose.wav");
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
//						gam_playDoorSound(i);
						doorTriggers[i].currentFrame = DOOR_UP_CLOSING_1;
						gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "doorClose.wav");
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
				shipdecks.at (gam_getCurrentDeckName ()).tiles[doorTriggers[i].tileIndex] = doorTriggers[i].currentFrame;
				gam_changeDoorFilters (doorTriggers[i].currentFrame, i);
			}
		}
	}    // end of for each door loop
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render current door frames onto map
void gam_renderDoorFrames ()
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_Texture    *tempTexture;

	tempTexture = SDL_GetRenderTarget(renderer.renderer);

	SDL_SetRenderTarget(renderer.renderer, gam_getPlayfieldTexture());

	for (const auto &doorIndex : doorTriggers)
	{
		if (sys_visibleOnScreen (doorIndex.renderPosition, tileSize))
		{
			gam_renderSingleTile (doorIndex.renderPosition.x, doorIndex.renderPosition.y, doorIndex.currentFrame);
		}
	}
	SDL_SetRenderTarget(renderer.renderer, tempTexture);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create a door bullet sensor
void gam_createDoorBulletSensor (unsigned long whichDoor)
//----------------------------------------------------------------------------------------------------------------------
{
	doorBulletSensor[whichDoor].bodyDef.type = b2_staticBody;
	doorBulletSensor[whichDoor].bodyDef.position.Set (doorBulletSensor[whichDoor].worldPosition.x / pixelsPerMeter, doorBulletSensor[whichDoor].worldPosition.y / pixelsPerMeter);
	doorBulletSensor[whichDoor].body = sys_getPhysicsWorld ()->CreateBody (&doorBulletSensor[whichDoor].bodyDef);

	doorBulletSensor[whichDoor].userData            = new _userData;
	doorBulletSensor[whichDoor].userData->userType  = PHYSIC_TYPE_DOOR_BULLET;
	doorBulletSensor[whichDoor].userData->dataValue = (int) whichDoor;
	doorBulletSensor[whichDoor].body->SetUserData (doorBulletSensor[whichDoor].userData);

	doorBulletSensor[whichDoor].shape.SetAsBox (doorBulletSensor[whichDoor].height / pixelsPerMeter, doorBulletSensor[whichDoor].width / pixelsPerMeter);
	doorBulletSensor[whichDoor].fixtureDef.shape    = &doorBulletSensor[whichDoor].shape;
	doorBulletSensor[whichDoor].fixtureDef.isSensor = false;

	doorBulletSensor[whichDoor].fixtureDef.filter.categoryBits = PHYSIC_TYPE_DOOR_CLOSED;       // Fix door physics not working on start
	doorBulletSensor[whichDoor].fixtureDef.filter.maskBits     = PHYSIC_TYPE_ENEMY | PHYSIC_TYPE_PLAYER | PHYSIC_TYPE_BULLET_PLAYER | PHYSIC_TYPE_BULLET_ENEMY;

	doorBulletSensor[whichDoor].body->CreateFixture (&doorBulletSensor[whichDoor].fixtureDef);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Clear out memory for door triggers
void gam_clearAllDoors ()
//----------------------------------------------------------------------------------------------------------------------
{
	doorTriggers.clear ();

	for (auto &doorBulletItr : doorBulletSensor)
	{
		if (doorBulletItr.userData != nullptr)
			delete (doorBulletItr.userData);
		if (doorBulletItr.body != nullptr)
			sys_getPhysicsWorld ()->DestroyBody (doorBulletItr.body);
	}
	doorBulletSensor.clear ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Find the doors for this deck and prepare a sensor object
void gam_doorTriggerSetup (const std::string deckName)
//----------------------------------------------------------------------------------------------------------------------
{
	int i           = 0;
	int currentTile = 0;
	int sourceX     = 0;
	int sourceY     = 0;

	_doorTrigger tempDoorTrigger;
	_doorTrigger tempDoorBulletSensor;

	if (!doorTriggers.empty ())  // Empty out array
	{
		gam_clearAllDoors ();
	}

	sourceX = 0.0f;
	sourceY = 0.0f;
	for (i  = 0; i != shipdecks.at (deckName).levelDimensions.x * shipdecks.at (deckName).levelDimensions.y; i++)
	{
		currentTile = shipdecks.at (deckName).tiles[i];
		if (currentTile < 0)
		{
			sys_shutdownWithError(sys_getString ("Tile in doorTriggerSetup is invalid - Tile [ %i ]", i));
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

				tempDoorBulletSensor.height          = tileSize * 0.5f;
				tempDoorBulletSensor.width           = tileSize / 4;
				tempDoorBulletSensor.worldPosition.x = ((sourceX * tileSize) + (tileSize / 2));
				tempDoorBulletSensor.worldPosition.y = ((sourceY * tileSize) + (tileSize / 2));
				tempDoorBulletSensor.currentFrame    = DOOR_ACROSS_CLOSED;
				tempDoorBulletSensor.direction       = DIRECTION_ACROSS;
				doorBulletSensor.push_back (tempDoorBulletSensor);

				gam_createDoorBulletSensor (doorBulletSensor.size () - 1);
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

				tempDoorBulletSensor.height          = tileSize / 4;
				tempDoorBulletSensor.width           = tileSize * 0.5f;
				tempDoorBulletSensor.worldPosition.x = ((sourceX * tileSize) + (tileSize / 2));
				tempDoorBulletSensor.worldPosition.y = ((sourceY * tileSize) + (tileSize / 2));
				tempDoorBulletSensor.currentFrame    = DOOR_UP_CLOSED;
				tempDoorBulletSensor.direction       = DIRECTION_UP;
				doorBulletSensor.push_back (tempDoorBulletSensor);

				gam_createDoorBulletSensor (doorBulletSensor.size () - 1);
				break;
			}

			default:
				break;
		}
		sourceX++;
		if (sourceX == (int) shipdecks.at (deckName).levelDimensions.x)
		{
			sourceX = 0;
			sourceY++;
		}
	}
}