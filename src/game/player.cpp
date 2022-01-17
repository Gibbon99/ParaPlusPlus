#include <SDL2_gfxPrimitives.h>
#include <game/score.h>
#include "io/logFile.h"
#include "system/util.h"
#include "game/database.h"
#include "game/hud.h"
#include "game/audio.h"
#include "game/lifts.h"
#include "game/alertLevel.h"
#include "game/game.h"
#include "game/transferGame.h"
#include "game/particles.h"
#include "game/player.h"
#include "classes/paraBullet.h"

paraDroid playerDroid {};
double    playerFriction {};                    // From script
float     influenceTimelimit {};
float     influenceTimelimtDelay {};
float     influenceTimeLeftWarning {};
int       bounceCounterTimeout {};              // How many bumps before ignoring them
int       bounceCounter {};
int       maxNumBumps {};                       // From script
float     bounceCounterDelay {};                // From script
int       radius {40};

std::vector<cpVect> playerTrail;
unsigned long       maxTrailSize {10};

//-----------------------------------------------------------------------------
//
// Check and decrement the bump counter
void gam_checkBumpCounter()        // TODO - Can be removed?
//-----------------------------------------------------------------------------
{
	if (bounceCounter > 0)
	{
		bounceCounterTimeout -= 1.0f * bounceCounterDelay;
		if (bounceCounterTimeout < 0.0f)
		{
			bounceCounterTimeout = 1.0f;
			bounceCounter--;
		}
	}
}

//-----------------------------------------------------------------------------
//
// Recharge player weapon
void gam_weaponRechargePlayer()
//-----------------------------------------------------------------------------
{
	if (!playerDroid.getWeaponCanFire ())
	{
		gam_setHudText ("hudRecharging");

		if (dataBaseEntry[playerDroid.getDroidType ()].canShoot)
			playerDroid.setWeaponDelay (playerDroid.getWeaponDelay () + dataBaseEntry[playerDroid.getDroidType ()].rechargeTime);
		else
			playerDroid.setWeaponDelay (playerDroid.getWeaponDelay () + dataBaseEntry[0].rechargeTime);

		if (playerDroid.getWeaponDelay () > 1.0f)
		{
			playerDroid.setWeaponDelay (0.0f);
			playerDroid.setWeaponCanFire (true);
			gam_setHudText ("hudMoving");
		}
	}
}

//-----------------------------------------------------------------------------------------------------------------
//
// Setup the player droid - run once at start of each new game
void gam_setupPlayerDroid()
//-----------------------------------------------------------------------------------------------------------------
{
	playerDroid.setDroidType (0);
	playerDroid.setDroidName ("001");
	playerDroid.sprite.create (playerDroid.getDroidName (), 9, 1.0);
	playerDroid.ai2.setAcceleration (dataBaseEntry[0].accelerate);
	playerDroid.ai2.setMaxSpeed (dataBaseEntry[0].maxSpeed);
	playerDroid.setCurrentHealth (dataBaseEntry[0].maxHealth);
	playerDroid.setCurrentMode (DROID_MODE_NORMAL);
	playerDroid.sprite.setLowHealth (false);

	sys_setupPlayerPhysics ();
	gam_checkPlayerHealth ();
}

//-----------------------------------------------------------------------------
//
// Process player movement
void gam_processPlayerMovement()
//-----------------------------------------------------------------------------
{
	if (gui.keyDown (KEY_LEFT))
	{
		playerDroid.setVelocity (cpVect {static_cast<float>(playerDroid.getVelocity ().x - playerDroid.ai2.getAcceleration ()), playerDroid.getVelocity ().y});
		if (playerDroid.getVelocity ().x < -playerDroid.ai2.getMaxSpeed ())
		{
			playerDroid.setVelocity (cpVect {static_cast<float>(-playerDroid.ai2.getMaxSpeed ()), playerDroid.getVelocity ().y});
		}
	}

	else if (gui.keyDown (KEY_RIGHT))
	{
		playerDroid.setVelocity (cpVect {static_cast<float>(playerDroid.getVelocity ().x + playerDroid.ai2.getAcceleration ()), playerDroid.getVelocity ().y});
		if (playerDroid.getVelocity ().x > playerDroid.ai2.getMaxSpeed ())
		{
			playerDroid.setVelocity (cpVect {static_cast<float>(playerDroid.ai2.getMaxSpeed ()), playerDroid.getVelocity ().y});
		}
	}

	if (gui.keyDown (KEY_UP))
	{
		playerDroid.setVelocity (cpVect {playerDroid.getVelocity ().x, static_cast<float>(playerDroid.getVelocity ().y - playerDroid.ai2.getAcceleration ())});
		if (playerDroid.getVelocity ().y < -playerDroid.ai2.getMaxSpeed ())
		{
			playerDroid.setVelocity (cpVect {playerDroid.getVelocity ().x, static_cast<float>(-playerDroid.ai2.getMaxSpeed ())});
		}
	}

	else if (gui.keyDown (KEY_DOWN))
	{
		playerDroid.setVelocity (cpVect {playerDroid.getVelocity ().x, static_cast<float>(playerDroid.getVelocity ().y + playerDroid.ai2.getAcceleration ())});
		if (playerDroid.getVelocity ().y > playerDroid.ai2.getMaxSpeed ())
		{
			playerDroid.setVelocity (cpVect {playerDroid.getVelocity ().x, static_cast<float>(playerDroid.ai2.getMaxSpeed ())});
		}
	}

//
// Do gravity slowdown when no key is pressed
	if (!gui.keyDown (KEY_LEFT))
	{
		if (playerDroid.getVelocity ().x < 0.0f)
		{
			playerDroid.setVelocity (cpVect {static_cast<float>(playerDroid.getVelocity ().x + gravity), playerDroid.getVelocity ().y});
			if (playerDroid.getVelocity ().x > 0.0f)
			{
				playerDroid.setVelocity (cpVect {0.0f, playerDroid.getVelocity ().y});
			}
		}
	}

	if (!gui.keyDown (KEY_RIGHT))
	{
		if (playerDroid.getVelocity ().x > 0.0f)
		{
			playerDroid.setVelocity (cpVect {static_cast<float>(playerDroid.getVelocity ().x - gravity), playerDroid.getVelocity ().y});
			if (playerDroid.getVelocity ().x < 0.0f)
			{
				playerDroid.setVelocity (cpVect {0.0f, playerDroid.getVelocity ().y});
			}
		}
	}

	if (!gui.keyDown (KEY_UP))
	{
		if (playerDroid.getVelocity ().y < 0.0f)
		{
			playerDroid.setVelocity (cpVect {playerDroid.getVelocity ().x, static_cast<float>(playerDroid.getVelocity ().y + gravity)});
			if (playerDroid.getVelocity ().y > 0.0f)
			{
				playerDroid.setVelocity (cpVect {playerDroid.getVelocity ().x, 0.0f});
			}
		}
	}

	if (!gui.keyDown (KEY_DOWN))
	{
		if (playerDroid.getVelocity ().y > 0.0f)
		{
			playerDroid.setVelocity (cpVect {playerDroid.getVelocity ().x, static_cast<float>(playerDroid.getVelocity ().y - gravity)});
			if (playerDroid.getVelocity ().y < 0.0f)
			{
				playerDroid.setVelocity (cpVect {playerDroid.getVelocity ().x, 0.0f});
			}
		}
	}
}

//----------------------------------------------------------------------------
//
// Start all the actions resulting from pressing the action key
void gam_processActionKey()
//----------------------------------------------------------------------------
{
	gam_setHudText ("hudMoving");

	if ((!gui.keyDown (KEY_ACTION) && (playerDroid.getInTransferMode ())))
	{
		playerDroid.setInTransferMode (false);
		gam_addAudioEvent (EVENT_ACTION_AUDIO_STOP, false, 0, 127, "transferMove");
		gam_setHudText ("hudMoving");
	}

	if (playerDroid.getInTransferMode ())
		gam_setHudText ("hudTransfer");

	if (gui.keyDown (KEY_ACTION))
	{
		//
		// Actions when no movements are down
		if ((!gui.keyDown (KEY_LEFT)) && (!gui.keyDown (KEY_RIGHT)) && (!gui.keyDown (KEY_DOWN)) && (!gui.keyDown (KEY_UP)))
		{
			if (playerDroid.getOverLiftTile ())
			{
				if (!playerDroid.getInTransferMode ())
				{
					gam_setHudText ("hudLift");
					gui.setState (KEY_ACTION, false, 0);
					gam_setCurrentTunnelDeckIndex ();
					gam_performLiftAction ();
					return;
				}
			}

			if (playerDroid.getOverTerminalTile ())
			{
				if (!playerDroid.getInTransferMode ())
				{
					gui.setState (KEY_ACTION, false, 0);
					gam_setHudText ("terminalMenu.terminalText");
					gam_stopAlertLevelSound (gam_getCurrentAlertLevel ());
					sys_setNewMode (MODE_GUI_TERMINAL, true);
					gui.setCurrentScreen (gui.getIndex (GUI_OBJECT_SCREEN, "terminalMenu"));
					gui.setActiveObject (gui.getCurrentScreen (), "terminalMenu.databaseButton");
					return;
				}
			}

			if (!playerDroid.getInTransferMode ())
			{
				playerDroid.setInTransferMode (true);
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, true, 0, 127, "transferMove");
				gam_setHudText ("hudTransfer");
			}
		}

		if (!playerDroid.getInTransferMode ())
		{
			{
				if (playerDroid.getWeaponCanFire ())
				{
					if ((playerDroid.getVelocity ().x == 0.0f) && (playerDroid.getVelocity ().y == 0.0f))       // Don't create bullet with no velocity
						return;

					gam_addBullet (-1);
					gui.setState (KEY_ACTION, false, 0);
					return;
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------------------------
//
// Check the players health, and set low health for animation
void gam_checkPlayerHealth()
//-----------------------------------------------------------------------------------------------------------------
{
	float dangerHealthLevel;
	float newAnimationSpeed;

	//
	// Process player health and animation
	if (playerDroid.getCurrentHealth () < 0)
	{
		if (playerDroid.getCurrentMode () != DROID_MODE_EXPLODING)
		{
#if MY_DEBUG
			log_addEvent (sys_getString ("[ %s ] Player mode set to DROID_MODE_EXPLODING.", __func__));
#endif
			playerDroid.setCurrentMode (DROID_MODE_EXPLODING);

			playerDroid.setVelocity (cpVect {0, 0});
			playerDroid.sprite.create ("explosion", 25, explosionAnimationSpeed);
			playerDroid.sprite.setAnimateSpeed (explosionAnimationSpeed);      // Set for explosion animation

			gam_addAudioEvent (EVENT_ACTION_AUDIO_STOP_ALL, false, 0, 0, "");

			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 1, 127, "explode1");
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 1, 127, "explode2");

			gam_addEmitter (playerDroid.getWorldPosInPixels (), PARTICLE_TYPE_EXPLOSION, 0);

			sys_clearDroidPhysics (gam_getCurrentDeckName ());

			gam_setFinalScore ();

			sys_setNewMode (MODE_GAME_OVER, false);
		}
		return;
	}

	dangerHealthLevel = static_cast<float>(dataBaseEntry[playerDroid.getDroidType ()].maxHealth) * 0.25f;

	if (playerDroid.getCurrentHealth () < static_cast<int>(dangerHealthLevel))
	{
		playerDroid.sprite.setLowHealth (true);
		if (!audio.isPlaying ("lowEnergy"))
		{
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, true, 0, 127, "lowEnergy");
		}
	}
	else
	{
		playerDroid.sprite.setLowHealth (false);
		if (audio.isPlaying ("lowEnergy"))
		{
			gam_addAudioEvent (EVENT_ACTION_AUDIO_STOP, true, 0, 127, "lowEnergy");
		}
	}
	//
	// Work out the player droid animation speed based on health
	newAnimationSpeed     = static_cast<float>(playerDroid.getCurrentHealth ()) / static_cast<float>(dataBaseEntry[playerDroid.getDroidType ()].maxHealth);
	if (newAnimationSpeed < 0.0f)
		newAnimationSpeed = 0.1f;

	if (newAnimationSpeed > 1.0f)
		newAnimationSpeed = 1.0f;

	playerDroid.sprite.setAnimateSpeed (newAnimationSpeed);
}

//-----------------------------------------------------------------------------------------------------------------
//
// Damage to player droid
void gam_damageToPlayer(int damageSource, int sourceDroid)
//-----------------------------------------------------------------------------------------------------------------
{
#ifdef MY_DEBUG
	log_addEvent (sys_getString ("[ %s ] damageSource [ %i ] sourceDroid [ %i ]", __func__, damageSource, sourceDroid));
#endif

	switch (damageSource)
	{
		case PHYSIC_DAMAGE_BUMP:
#ifdef MY_DEBUG
			log_addEvent (sys_getString ("[ %s ] Player damaged by BUMP.", __func__));
#endif
			bounceCounter++;
			if (bounceCounter > maxNumBumps)
				return;

			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 100, 127, "collision1");

			if (g_shipDeckItr->second.droid[sourceDroid].getCurrentMode () == DROID_MODE_EXPLODING)
				playerDroid.setCurrentHealth (playerDroid.getCurrentHealth () - explosionDamage);
			else
				playerDroid.setCurrentHealth (playerDroid.getCurrentHealth () - dataBaseEntry[g_shipDeckItr->second.droid[sourceDroid].getDroidType ()].bounceDamage);
			break;

		case PHYSIC_DAMAGE_BULLET:
#ifdef MY_DEBUG
			log_addEvent (sys_getString ("[ %s ] Player hit by BULLET.", __func__));
#endif
			if (sourceDroid != TARGET_PLAYER)       // Stop player shooting self
			{
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 1, 127, "damage");
				playerDroid.setCurrentHealth (playerDroid.getCurrentHealth () - dataBaseEntry[g_shipDeckItr->second.droid[sourceDroid].getDroidType ()].bulletDamage);
			}

			break;

		case PHYSIC_DAMAGE_EXPLOSION:
			// Not required - handled in BUMP case
			break;
	}
}

//-----------------------------------------------------------------------------------------------------------------
//
// Set the influence time limit - based on droid class
void gam_setInfluenceTimelimit(int targetDroidClass)
//-----------------------------------------------------------------------------------------------------------------
{
	playerDroid.setInfluenceTimeLeft (influenceTimelimit - (static_cast<float>(targetDroidClass) * 2));
	playerDroid.setLowInfluenceTimeFlag (false);
}

//-----------------------------------------------------------------------------------------------------------------
//
// Reset the Influence time flag - so player doesn't change color
void gam_resetInfluenceTimeLeftFlag()
//-----------------------------------------------------------------------------------------------------------------
{
	playerDroid.setLowInfluenceTimeFlag (false);
}

//-----------------------------------------------------------------------------------------------------------------
//
// Process how long a droid can be controlled for
void gam_processInfluenceTime()
//-----------------------------------------------------------------------------------------------------------------
{
	if (playerDroid.getDroidType () == 0)
		return;

	//
	// Decrement count normally
	playerDroid.setInfluenceTimeLeft (playerDroid.getInfluenceTimeLeft () - 1.0f * influenceTimelimtDelay);
	//
	// Check if control time on this droid is up
	if (playerDroid.getInfluenceTimeLeft () < 0.0f)
	{
		playerDroid.setVelocity (cpVect {0, 0});
		gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 1, 127, "explode2");
		gam_addEmitter (playerDroid.getWorldPosInPixels (), PARTICLE_TYPE_EXPLOSION, 0);
		gam_addEmitter (playerDroid.getWorldPosInPixels (), PARTICLE_TYPE_EXPLOSION, 0);
		trn_transferLostGame ();
		playerDroid.sprite.setTintColor (255, 255, 255);
		playerDroid.setLowInfluenceTimeFlag (false);
		return;
	}
	//
	// Flash warning as time will soon be up
	if (playerDroid.getInfluenceTimeLeft () < influenceTimeLeftWarning)
	{
		if (playerDroid.getLowInfluenceTimeFlag())
		{
			playerDroid.setInfluenceFadeValue (playerDroid.getInfluenceFadeValue () - 10);
			if (playerDroid.getInfluenceFadeValue () < 50)
			{
				playerDroid.setInfluenceFadeValue (50);
				playerDroid.setLowInfluenceTimeFlag (!playerDroid.getLowInfluenceTimeFlag());
			}
		}
		else
		{
			playerDroid.setInfluenceFadeValue (playerDroid.getInfluenceFadeValue () + 10);
			if (playerDroid.getInfluenceFadeValue () > 254)
			{
				playerDroid.setInfluenceFadeValue (254);
				playerDroid.setLowInfluenceTimeFlag (!playerDroid.getLowInfluenceTimeFlag());
			}
		}
	}

#ifdef MY_DEBUG
	if (playerDroid.getLowInfluenceTimeFlag ())
		log_addEvent (sys_getString ("[ %s ] Influence time is about to run out.", __func__));
#endif
}

//-----------------------------------------------------------------------------------------------------------------
//
// Create a breadcrumb trail as the player moves around
void gam_createTrail()
//-----------------------------------------------------------------------------------------------------------------
{
	cpVect tempPosition;

	tempPosition = sys_convertPixelsToTiles (playerDroid.getWorldPosInPixels ());

	if (playerTrail.size () == 0)
	{
		playerTrail.push_back (tempPosition);
		return;
	}

	if ((tempPosition.x != playerTrail[0].x) && (tempPosition.y != playerTrail[0].y))
	{
		if (playerTrail.size () < maxTrailSize)
		{
			playerTrail.push_back (tempPosition);

			std::rotate (playerTrail.rbegin (), playerTrail.rbegin () + 1, playerTrail.rend ());
			playerTrail[0] = tempPosition;
			return;
		}
		else    // Reached max size for trail
		{
			std::rotate (playerTrail.rbegin (), playerTrail.rbegin () + 1, playerTrail.rend ());
			playerTrail[0] = tempPosition;
		}
	}
}

//-----------------------------------------------------------------------------------------------------------------
//
// Return the coordinates from the last position in the trial array
cpVect gam_getLastPlayerTrail()
//-----------------------------------------------------------------------------------------------------------------
{
	cpVect returnCoords;

	if (playerTrail.size () == 0)
	{
		returnCoords.x = returnCoords.y = -1;
		return returnCoords;
	}

	return playerTrail[playerTrail.size () - 1];
}

//-----------------------------------------------------------------------------------------------------------------
//
// Reset the trail data when changing to a new level
void gam_clearPlayerTrail()
//-----------------------------------------------------------------------------------------------------------------
{
	playerTrail.clear ();
}

//-----------------------------------------------------------------------------------------------------------------
//
// Show the player trail
void gam_debugShowPlayerTrail()
//-----------------------------------------------------------------------------------------------------------------
{
	cpVect drawPosition;

	for (auto trailItr: playerTrail)
	{
		drawPosition.x = trailItr.x * tileSize;
		drawPosition.y = trailItr.y * tileSize;

		drawPosition = sys_worldToScreen (drawPosition, tileSize);

		boxRGBA (renderer.renderer, static_cast<Sint16>(drawPosition.x), static_cast<Sint16>(drawPosition.y), static_cast<Sint16>(drawPosition.x + tileSize), static_cast<Sint16>(drawPosition.y + tileSize), 50, 50, 150, 128);
	}
}