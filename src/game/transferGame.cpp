#include <game/player.h>
#include <game/database.h>
#include <game/alertLevel.h>
#include "game/hud.h"
#include "system/util.h"
#include "gui/guiLanguage.h"
#include "game/transfer.h"
#include "game/transferPlayer.h"
#include "game/audio.h"
#include "game/transferDroidAI.h"
#include "game/transferGame.h"

int                         transferPlayerWhichSide;
int                         playerBlockPos;
int                         transferTimeoutCountdown = 0;
int                         transferTimeOut;
int                         transferResultDelay;
float                       transferDelayTime;
std::vector<__TRANSFER_ROW> transferRows;

//---------------------------------------------------------------------------------------------------------------------
//
// Deadlock the transfer - wait for sound to finish
void trn_processDeadlock()
//---------------------------------------------------------------------------------------------------------------------
{
	if (audio.isPlaying("transferdeadlock"))
		return;

	trn_initTransferValues (playerDroid.transferTargetDroidIndex);

//	trn_prepareTransferGame ();
}

//---------------------------------------------------------------------------------------------------------------------
//
// Check if all the circuits have timed out
bool trn_checkForActiveCircuits ()
//---------------------------------------------------------------------------------------------------------------------
{
	trn_processCircuits ();

	for (auto transferItr : transferRows)
	{
		if ((transferItr.rightSideActive) || (transferItr.leftSideActive))
		{
			return false;   // A Circuit is still counting down
		}
	}
	return true;
}

//---------------------------------------------------------------------------------------------------------------------
//
// Handle the end of transfer game
void trn_processEndOfTransferGame ()
//---------------------------------------------------------------------------------------------------------------------
{
	bool okToProceed = false;
	int  playerCount = 0;

	//
	// Wait for circuits to stop being active
	//
	while (!okToProceed)
	{
		okToProceed = trn_checkForActiveCircuits ();
	}
	//
	// count for player side
	//
	for (auto transferItr : transferRows)
	{
		if (transferPlayerWhichSide == TRANSFER_COLOR_LEFT)
		{
			if (transferItr.currentColor == TRANSFER_COLOR_LEFT)
			{
				playerCount++;
			}
		}
		else
		{
			if (transferItr.currentColor == TRANSFER_COLOR_RIGHT)
			{
				playerCount++;
			}
		}
	}

	//
	// If less and player is 001 - explode
	//
	if ((playerDroid.droidType == 0) && (playerCount < static_cast<int>(transferRows.size ()) / 2))
	{
		playerDroid.inTransferMode = false;
		playerDroid.currentHealth = -1;
		gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "transferBurntout");
		sys_setNewMode (MODE_TRANSFER_RESULT, false);
		gam_setHudText ("burntout");
		sys_addEvent(EVENT_TYPE_GAME, EVENT_ACTION_GAME_CHANGE_MODE, transferResultDelay, to_string(MODE_GAME)+"|"+to_string(true));
		return;
	}
	//
	// If less and player is > 001 - drop health, back to game
	//
	if ((playerDroid.droidType > 0) && (playerCount < static_cast<int>(transferRows.size ()) / 2))
	{
		playerDroid.inTransferMode = false;
		g_shipDeckItr->second.droid[playerDroid.transferTargetDroidIndex].currentHealth = -1;
		playerDroid.transferTargetDroidType = 0;    // Transfer back into 001 droid
		trn_transferIntoDroid();    // Drop back to 001 stats and sprite
		sys_setNewMode(MODE_TRANSFER_RESULT, false);
		gam_setHudText ("transferFailed");
		sys_addEvent(EVENT_TYPE_GAME, EVENT_ACTION_GAME_CHANGE_MODE, transferResultDelay, to_string(MODE_GAME)+"|"+to_string(true));
		sys_addEvent(EVENT_TYPE_GAME, EVENT_ACTION_START_BACKGROUND_SOUND, transferResultDelay, to_string(MODE_GAME)+"|"+to_string(true));
		return;
	}
	//
	// If the same - then deadlock
	//
	if (playerCount == static_cast<int>(transferRows.size() / 2))
	{
		gam_addAudioEvent(EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "transferdeadlock");
		sys_setNewMode(MODE_TRANSFER_DEADLOCK, false);
		gam_setHudText("deadlock");
		return;
	}
	//
	// if more, do transfer
	// explode other droid - do score - call damageToDroid ?
	// back to game after delay timer after showing transfer result
	//
	playerDroid.inTransferMode = false;
	trn_transferIntoDroid ();
	sys_setNewMode (MODE_TRANSFER_RESULT, false);
	gam_setHudText ("transferred");
	sys_addEvent(EVENT_TYPE_GAME, EVENT_ACTION_GAME_CHANGE_MODE, transferResultDelay, to_string(MODE_GAME)+"|"+to_string(true));
	sys_addEvent(EVENT_TYPE_GAME, EVENT_ACTION_START_BACKGROUND_SOUND, transferResultDelay, to_string(MODE_GAME)+"|"+to_string(true));
}

//---------------------------------------------------------------------------------------------------------------------
//
// Place a token onto a circuit
void trn_placeToken (int rowIndex, int whichSide, int whichDroid)
//---------------------------------------------------------------------------------------------------------------------
{
	gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "transfer1");

	if (whichSide == TRANSFER_COLOR_RIGHT)
	{
		transferRows[rowIndex].rightSideActiveCounter = 3.0f;
		transferRows[rowIndex].rightSideActive        = true;
		switch (transferRows[rowIndex].rightSideType)
		{
			case TRANSFER_ROW_REPEAT_HALF:
			case TRANSFER_ROW_REPEAT_QUARTER:
				transferRows[rowIndex].rightSideActiveIsOn = true;
				break;

			default:
				break;
		}
	}
	else
	{
		transferRows[rowIndex].leftSideActiveCounter = 3.0f;
		transferRows[rowIndex].leftSideActive        = true;
		switch (transferRows[rowIndex].leftSideType)
		{
			case TRANSFER_ROW_REPEAT_HALF:
			case TRANSFER_ROW_REPEAT_QUARTER:
				transferRows[rowIndex].leftSideActiveIsOn = true;
				break;

			default:
				break;
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
//
// Return the string showing the transfer countdown
std::string trn_getTransferCountdown ()
//---------------------------------------------------------------------------------------------------------------------
{
	return sys_getString ("%s %i", gui_getString ("transferFinish").c_str (), transferTimeoutCountdown);
}

//---------------------------------------------------------------------------------------------------------------------
//
// Process the end of the transfer game
void trn_processTransferGameEnd()
//---------------------------------------------------------------------------------------------------------------------
{

}

//---------------------------------------------------------------------------------------------------------------------
//
// Play the transfer game - mode MODE_TRANSFER_GAME
void trn_processTransferGame ()
//---------------------------------------------------------------------------------------------------------------------
{
	static float transferTimeoutCounter = 0.0f;

	gam_setHudText ("");

	transferTimeoutCounter -= 1.0f * transferDelayTime;
	if (transferTimeoutCounter < 0.0)
	{
		transferTimeoutCounter = 1.0f;
		transferTimeoutCountdown--;
		if (transferTimeoutCountdown < 0)
		{
			transferTimeoutCountdown = transferTimeOut;
			trn_processEndOfTransferGame ();
			return;
		}
	}

	trn_processTransferDroidAI ();
	trn_processPlayerActions ();
	trn_processCircuits ();
}

//---------------------------------------------------------------------------------------------------------------------
//
// Transfer deadlock - start again
void trn_transferDeadlock ()
//---------------------------------------------------------------------------------------------------------------------
{

}

//---------------------------------------------------------------------------------------------------------------------
//
// Lost the transfer - back to 001
void trn_transferLostGame ()
//---------------------------------------------------------------------------------------------------------------------
{
	gam_setupPlayerDroid ();        // reset back to 001
	playerDroid.currentHealth = dataBaseEntry[playerDroid.droidType].maxHealth / 4;   // Not in a good way
}

//---------------------------------------------------------------------------------------------------------------------
//
// Transfer into the droid
void trn_transferIntoDroid ()
//---------------------------------------------------------------------------------------------------------------------
{
	playerDroid.velocity      = {0, 0};

	if (playerDroid.transferTargetDroidType > 0)
	{
		gam_setInfluenceTimelimit (playerDroid.transferTargetDroidType);
		playerDroid.droidType     = playerDroid.transferTargetDroidType;
		playerDroid.currentHealth = g_shipDeckItr->second.droid[playerDroid.transferTargetDroidIndex].currentHealth;
		playerDroid.droidName     = g_shipDeckItr->second.droid[playerDroid.transferTargetDroidIndex].droidName;
		playerDroid.sprite        = g_shipDeckItr->second.droid[playerDroid.transferTargetDroidIndex].sprite;

		playerDroid.ai.setAcceleration (dataBaseEntry[playerDroid.droidType].accelerate);
		playerDroid.ai.setMaxSpeed (dataBaseEntry[playerDroid.droidType].maxSpeed);

		//
		// Destroy the droid
		g_shipDeckItr->second.droid[playerDroid.transferTargetDroidIndex].currentHealth = -10;

		gam_checkActionWitness();

//		gam_damageToDroid (playerDroid.transferTargetDroidIndex, PHYSIC_DAMAGE_BULLET, -1);
	}
	else
	{
		// Drop back to 001
		gam_setupPlayerDroid ();
		playerDroid.currentHealth = dataBaseEntry[0].maxHealth / 4;
		gam_checkPlayerHealth();    // Set animation speed
		gam_resetInfluenceTimeLeftFlag();   // Don't draw in blue if in low influence time and fail transfer
	}
}