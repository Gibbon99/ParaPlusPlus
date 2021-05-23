#include <game/database.h>
#include <game/hud.h>
#include <system/util.h>
#include <game/audio.h>
#include <game/lifts.h>
#include <system/gameEvents.h>
#include <game/alertLevel.h>
#include <game/game.h>
#include <game/transferGame.h>
#include <game/particles.h>
#include <gui/guiHighScore.h>
#include <game/score.h>
#include <SDL2_gfxPrimitives.h>
#include "game/player.h"

#include "game/bullet.h"

droidClass playerDroid;
double     playerFriction;      // From script
float      influenceTimelimit;
float      influenceTimelimtDelay;
float      influenceTimeLeftWarning;
int        bounceCounterTimeout = 0;       // How many bumps before ignoring them
int        bounceCounter        = 0;
int        maxNumBumps;        // From script
float      bounceCounterDelay;           // From script

static double angleCounter       = 1.0;
int           radius             = 40;

std::vector<b2Vec2> playerTrail;
unsigned long       maxTrailSize = 10;

//-----------------------------------------------------------------------------------------------------------------
//
// Render the test circle
void gam_moveTestCircle ()
//-----------------------------------------------------------------------------------------------------------------
{
	static int angle = 0;

	angleCounter -= 1.0 / 0.3;
	if (angleCounter < 0)
	{
		angleCounter = 1.0;
		angle++;
		if (angle > 359)
			angle = 0;
	}
}

//-----------------------------------------------------------------------------
//
// Return the type of droid for checking access in database view
int gam_getDroidType ()
//-----------------------------------------------------------------------------
{
	return playerDroid.droidType;
}

//-----------------------------------------------------------------------------
//
// Check and decrement the bump counter
void gam_checkBumpCounter ()
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
void gam_weaponRechargePlayer ()
//-----------------------------------------------------------------------------
{
	if (!playerDroid.weaponCanFire)
	{
		gam_setHudText ("hudRecharging");

		if (dataBaseEntry[playerDroid.droidType].canShoot)
			playerDroid.weaponDelay += dataBaseEntry[playerDroid.droidType].rechargeTime;
		else
			playerDroid.weaponDelay += dataBaseEntry[0].rechargeTime;

		if (playerDroid.weaponDelay > 1.0f)
		{
			playerDroid.weaponDelay   = 0.0f;
			playerDroid.weaponCanFire = true;
			gam_setHudText ("hudMoving");
		}
	}
}

//-----------------------------------------------------------------------------------------------------------------
//
// Setup the player droid - run once at start of each new game
void gam_setupPlayerDroid ()
//-----------------------------------------------------------------------------------------------------------------
{
	playerDroid.droidType = 0;
	playerDroid.droidName = "001";
	playerDroid.sprite.create (playerDroid.droidName, 9, 1.0);
	playerDroid.ai.setAcceleration (dataBaseEntry[0].accelerate);
	playerDroid.ai.setMaxSpeed (dataBaseEntry[0].maxSpeed);
	playerDroid.currentHealth = dataBaseEntry[0].maxHealth;
	playerDroid.currentMode   = DROID_MODE_NORMAL;
	playerDroid.sprite.setLowHealth (false);

	sys_setupPlayerPhysics ();
	gam_checkPlayerHealth ();
}

//-----------------------------------------------------------------------------
//
// Process player movement
void gam_processPlayerMovement ()
//-----------------------------------------------------------------------------
{
	if (gui.keyDown (KEY_LEFT))
	{
		playerDroid.velocity.x -= playerDroid.ai.getAcceleration ();
		if (playerDroid.velocity.x < -playerDroid.ai.getMaxSpeed ())
		{
			playerDroid.velocity.x = -playerDroid.ai.getMaxSpeed ();
		}
	}

	else if (gui.keyDown (KEY_RIGHT))
	{
		playerDroid.velocity.x += playerDroid.ai.getAcceleration ();
		if (playerDroid.velocity.x > playerDroid.ai.getMaxSpeed ())
		{
			playerDroid.velocity.x = playerDroid.ai.getMaxSpeed ();
		}
	}

	if (gui.keyDown (KEY_UP))
	{
		playerDroid.velocity.y -= playerDroid.ai.getAcceleration ();
		if (playerDroid.velocity.y < -playerDroid.ai.getMaxSpeed ())
		{
			playerDroid.velocity.y = -playerDroid.ai.getMaxSpeed ();
		}
	}

	else if (gui.keyDown (KEY_DOWN))
	{
		playerDroid.velocity.y += playerDroid.ai.getAcceleration ();
		if (playerDroid.velocity.y > playerDroid.ai.getMaxSpeed ())
		{
			playerDroid.velocity.y = playerDroid.ai.getMaxSpeed ();
		}
	}

//
// Do gravity slowdown when no key is pressed
	if (!gui.keyDown (KEY_LEFT))
	{
		if (playerDroid.velocity.x < 0.0f)
		{
			playerDroid.velocity.x += gravity;
			if (playerDroid.velocity.x > 0.0f)
			{
				playerDroid.velocity.x = 0.0f;
			}
		}
	}

	if (!gui.keyDown (KEY_RIGHT))
	{
		if (playerDroid.velocity.x > 0.0f)
		{
			playerDroid.velocity.x -= gravity;
			if (playerDroid.velocity.x < 0.0f)
			{
				playerDroid.velocity.x = 0.0f;
			}
		}
	}

	if (!gui.keyDown (KEY_UP))
	{
		if (playerDroid.velocity.y < 0.0f)
		{
			playerDroid.velocity.y += gravity;
			if (playerDroid.velocity.y > 0.0f)
			{
				playerDroid.velocity.y = 0.0f;
			}
		}
	}

	if (!gui.keyDown (KEY_DOWN))
	{
		if (playerDroid.velocity.y > 0.0f)
		{
			playerDroid.velocity.y -= gravity;
			if (playerDroid.velocity.y < 0.0f)
			{
				playerDroid.velocity.y = 0.0f;
			}
		}
	}
}

//----------------------------------------------------------------------------
//
// Start all the actions resulting from pressing the action key
void gam_processActionKey ()
//----------------------------------------------------------------------------
{
	gam_setHudText ("hudMoving");

	if ((!gui.keyDown (KEY_ACTION) && (playerDroid.inTransferMode)))
	{
		playerDroid.inTransferMode = false;
		gam_addAudioEvent (EVENT_ACTION_AUDIO_STOP, false, 0, 127, "transferMove");
		gam_setHudText ("hudMoving");
	}

	if (playerDroid.inTransferMode)
		gam_setHudText ("hudTransfer");

	if (gui.keyDown (KEY_ACTION))
	{
		//
		// Actions when no movements are down
		if ((!gui.keyDown (KEY_LEFT)) && (!gui.keyDown (KEY_RIGHT)) && (!gui.keyDown (KEY_DOWN)) && (!gui.keyDown (KEY_UP)))
		{
			if (playerDroid.overLiftTile)
			{
				if (!playerDroid.inTransferMode)
				{
					gam_setHudText ("hudLift");
					gui.setState (KEY_ACTION, false, 0);
					gam_setCurrentTunnelDeckIndex ();
					gam_performLiftAction ();
					return;
				}
			}

			if (playerDroid.overTerminalTile)
			{
				if (!playerDroid.inTransferMode)
				{
					gui.setState (KEY_ACTION, false, 0);
					gam_setHudText ("terminalMenu.terminalText");
					gam_stopAlertLevelSound (gam_getCurrentAlertLevel ());
					sys_setNewMode (MODE_GUI_TERMINAL, true);
					gui.setCurrentScreen (gui.getIndex (GUI_OBJECT_SCREEN, "terminalMenu"));
					gui.setActiveObject (gui.getCurrentScreen (), GUI_OBJECT_BUTTON, "terminalMenu.databaseButton");
					return;
				}
			}

			if (!playerDroid.inTransferMode)
			{
				playerDroid.inTransferMode = true;
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, true, 0, 127, "transferMove");
				gam_setHudText ("hudTransfer");
			}
		}

		if (!playerDroid.inTransferMode)
		{
			{
				if (playerDroid.weaponCanFire)
				{
					if ((playerDroid.velocity.x == 0.0f) && (playerDroid.velocity.y == 0.0f))       // Don't create bullet with no velocity
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
void gam_checkPlayerHealth ()
//-----------------------------------------------------------------------------------------------------------------
{
	float       dangerHealthLevel;
	float       newAnimationSpeed;
	static bool lowEnergySoundPlaying = false;

	//
	// Process player health and animation
	if (playerDroid.currentHealth < 0)
	{
		if (playerDroid.currentMode != DROID_MODE_EXPLODING)
		{
			playerDroid.currentMode = DROID_MODE_EXPLODING;

			playerDroid.velocity = {0, 0};
			playerDroid.sprite.create ("explosion", 25, explosionAnimationSpeed);
			playerDroid.sprite.setAnimateSpeed (explosionAnimationSpeed);      // Set for explosion animation

			gam_addAudioEvent (EVENT_ACTION_AUDIO_STOP_ALL, false, 0, 0, "");

			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 1, 127, "explode1");
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 1, 127, "explode2");

			gam_addEmitter (sys_convertToMeters (playerDroid.worldPosInPixels), PARTICLE_TYPE_EXPLOSION, 0);

			sys_clearDroidPhysics (gam_getCurrentDeckName ());

			sys_setNewMode (MODE_GAME_OVER, false);
		}
		return;
	}

	dangerHealthLevel     = static_cast<float>(dataBaseEntry[playerDroid.droidType].maxHealth) * 0.25f;

	if (playerDroid.currentHealth < static_cast<int>(dangerHealthLevel))
	{
		playerDroid.sprite.setLowHealth (true);
		if (!audio.isPlaying ("lowEnergy"))
		{
			lowEnergySoundPlaying = true;
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, true, 0, 127, "lowEnergy");
		}
	}
	else
	{
		playerDroid.sprite.setLowHealth (false);
		if (audio.isPlaying ("lowEnergy"))
		{
			lowEnergySoundPlaying = false;
			gam_addAudioEvent (EVENT_ACTION_AUDIO_STOP, true, 0, 127, "lowEnergy");
		}
	}
	//
	// Work out the player droid animation speed based on health
	newAnimationSpeed     = static_cast<float>(playerDroid.currentHealth) / static_cast<float>(dataBaseEntry[playerDroid.droidType].maxHealth);
	if (newAnimationSpeed < 0.0f)
		newAnimationSpeed = 0.1f;

	if (newAnimationSpeed > 1.0f)
		newAnimationSpeed = 1.0f;

	playerDroid.sprite.setAnimateSpeed (newAnimationSpeed);
}

//-----------------------------------------------------------------------------------------------------------------
//
// Damage to player droid
void gam_damageToPlayer (int damageSource, int sourceDroid)
//-----------------------------------------------------------------------------------------------------------------
{

#ifdef MY_DEBUG
	printf ("DamageToPlayer : damageSource [ %i ] sourceDroid [ %i ]\n\n", damageSource, sourceDroid);
#endif

	switch (damageSource)
	{
		case PHYSIC_DAMAGE_BUMP:
			bounceCounter++;
			if (bounceCounter > maxNumBumps)
				return;

			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 100, 127, "collision1");

			if (g_shipDeckItr->second.droid[sourceDroid].currentMode == DROID_MODE_EXPLODING)
				playerDroid.currentHealth -= explosionDamage;
			else
				playerDroid.currentHealth -= dataBaseEntry[g_shipDeckItr->second.droid[sourceDroid].droidType].bounceDamage;
			break;

		case PHYSIC_DAMAGE_BULLET:
#ifdef MY_DEBUG
			std::cout << "Player hit by bullet" << std::endl;
#endif
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 1, 127, "damage");
			playerDroid.currentHealth -= dataBaseEntry[g_shipDeckItr->second.droid[sourceDroid].droidType].bulletDamage;

			break;

		case PHYSIC_DAMAGE_EXPLOSION:
			// Not required - handled in BUMP case
			break;
	}
}

//-----------------------------------------------------------------------------------------------------------------
//
// Set the influence time limit - based on droid class
void gam_setInfluenceTimelimit (int targetDroidClass)
//-----------------------------------------------------------------------------------------------------------------
{
	playerDroid.influenceTimeLeft    = influenceTimelimit - (static_cast<float>(targetDroidClass) * 2);
	playerDroid.lowInfluenceTimeleft = false;
}

//-----------------------------------------------------------------------------------------------------------------
//
// Process how long a droid can be controlled for
void gam_processInfluenceTime ()
//-----------------------------------------------------------------------------------------------------------------
{
	if (playerDroid.droidType == 0)
		return;

	if (playerDroid.influenceFadeFlag)
	{
		playerDroid.influenceFade -= 10;
		if (playerDroid.influenceFade < 50)
		{
			playerDroid.influenceFade     = 50;
			playerDroid.influenceFadeFlag = !playerDroid.influenceFadeFlag;
		}
	}
	else
	{
		playerDroid.influenceFade += 10;
		if (playerDroid.influenceFade > 254)
		{
			playerDroid.influenceFade     = 254;
			playerDroid.influenceFadeFlag = !playerDroid.influenceFadeFlag;
		}
	}

	playerDroid.influenceTimeLeft -= 1.0f * influenceTimelimtDelay;

	if (playerDroid.influenceTimeLeft < influenceTimeLeftWarning)
		playerDroid.lowInfluenceTimeleft = true;

	if (playerDroid.influenceTimeLeft < 0.0f)
	{
		playerDroid.velocity = {0, 0};
		gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 1, 127, "explode2");
		gam_addEmitter (sys_convertToMeters (playerDroid.worldPosInPixels), PARTICLE_TYPE_EXPLOSION, 0);
		gam_addEmitter (sys_convertToMeters (playerDroid.worldPosInPixels), PARTICLE_TYPE_EXPLOSION, 0);
		trn_transferLostGame ();
		playerDroid.sprite.setTintColor (255, 255, 255);
		playerDroid.influenceFadeFlag    = false;
		playerDroid.lowInfluenceTimeleft = false;

#ifdef MY_DEBUG
		std::cout << "Influence time is up." << std::endl;
#endif
	}

#ifdef MY_DEBUG
	if (playerDroid.lowInfluenceTimeleft)
		std::cout << "Influence time is about to run out" << std::endl;
#endif
}

//-----------------------------------------------------------------------------------------------------------------
//
// Create a breadcrumb trail as the player moves around
void gam_createTrail ()
//-----------------------------------------------------------------------------------------------------------------
{

	b2Vec2 tempPosition;

	tempPosition = sys_convertToTiles (playerDroid.worldPosInPixels);

	if (playerTrail.size () == 0)
	{
		playerTrail.push_back (tempPosition);
		return;
	}

	if (tempPosition != playerTrail[0])
	{
		if (playerTrail.size () < maxTrailSize)
		{
			playerTrail.push_back(tempPosition);

			for (int i = playerTrail.size(); i != 0; i--)
			{
				playerTrail[i] = playerTrail[i - 1];
			}
			playerTrail[0] = tempPosition;
			return;
		}
		else    // Reached max size for trail
		{
			for (int i = playerTrail.size(); i != 0; i--)
			{
				playerTrail[i] = playerTrail[i - 1];
			}
			playerTrail[0] = tempPosition;
		}
	}
}

//-----------------------------------------------------------------------------------------------------------------
//
// Return the coordinates from the last position in the trial array
b2Vec2 gam_getLastPlayerTrail()
//-----------------------------------------------------------------------------------------------------------------
{
	b2Vec2  returnCoords;

	if (playerTrail.size() == 0)
	{
		returnCoords.x = returnCoords.y = -1;
		return returnCoords;
	}

	return playerTrail[playerTrail.size() - 1];
}

//-----------------------------------------------------------------------------------------------------------------
//
// Reset the trail data when changing to a new level
void gam_clearPlayerTrail()
//-----------------------------------------------------------------------------------------------------------------
{
	playerTrail.clear();
}

//-----------------------------------------------------------------------------------------------------------------
//
// Show the player trail
void gam_debugShowPlayerTrail()
//-----------------------------------------------------------------------------------------------------------------
{
	b2Vec2  drawPosition;

	for (auto trailItr : playerTrail)
	{
		drawPosition.x = trailItr.x * tileSize;
		drawPosition.y = trailItr.y * tileSize;

		drawPosition = sys_worldToScreen(drawPosition, tileSize);

		boxRGBA (renderer.renderer, static_cast<Sint16>(drawPosition.x), static_cast<Sint16>(drawPosition.y), static_cast<Sint16>(drawPosition.x + tileSize), static_cast<Sint16>(drawPosition.y + tileSize), 50, 50, 150, 128);
	}
}