#include <game/database.h>
#include <game/hud.h>
#include <gui/guiLanguage.h>
#include <system/util.h>
#include <game/audio.h>
#include <game/lifts.h>
#include <system/gameEvents.h>
#include <game/alertLevel.h>
#include "game/player.h"

#include "game/bullet.h"

droidClass playerDroid;
double     playerFriction;      // From script

droidClass    testCircle;
static double angleCounter = 1.0;
int           radius       = 40;

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

	testCircle.worldPosInPixels.x = (gameWinWidth / 2) + (radius * cos (angle * (3.14 / 180)));
	testCircle.worldPosInPixels.y = (gameWinHeight / 2) + (radius * sin (angle * (3.14 / 180)));
}

//-----------------------------------------------------------------------------
//
// Recharge player weapon
void gam_weaponRechargePlayer ()
//-----------------------------------------------------------------------------
{
	if (!playerDroid.weaponCanFire)
	{
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
// Setup the player droid - run once
void gam_setupPlayerDroid ()
//-----------------------------------------------------------------------------------------------------------------
{
	playerDroid.droidType = 0;
	playerDroid.droidName = "001";
	playerDroid.sprite.create (playerDroid.droidName, 9, 0.3);
	playerDroid.ai.setAcceleration (dataBaseEntry[0].accelerate);
	playerDroid.ai.setMaxSpeed (dataBaseEntry[0].maxSpeed);

	sys_setupPlayerPhysics ();
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

	if (gui.keyDown(KEY_ACTION))
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
					gam_stopAlertLevelSound (gam_getCurrentAlertLevel());
					sys_setNewMode (MODE_GUI_TERMINAL, true);
					gui.setCurrentScreen (gui.getIndex (GUI_OBJECT_SCREEN, "terminalMenu"));
					gui.setActiveObject (gui.getCurrentScreen (), GUI_OBJECT_BUTTON, "terminalMenu.backButton");
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
					playerDroid.weaponCanFire = false;
					gui.setState (KEY_ACTION, false, 0);
					gam_setHudText ("hudRecharging");
					return;
				}
			}
		}
	}
}