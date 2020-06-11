#include <game/player.h>
#include <game/database.h>
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
float                       transferDelayTime;
std::vector<__TRANSFER_ROW> transferRows;

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
			playerDroid.inTransferMode = false;
			trn_transferIntoDroid();
			sys_setNewMode(MODE_GAME, true);
//			sys_setNewMode (MODE_PRE_TRANSFER_GAME, false);
		}
	}

	trn_processTransferDroidAI ();
	trn_processPlayerActions();
	trn_processCircuits ();
}

//---------------------------------------------------------------------------------------------------------------------
//
// Transfer deadlock - start again
void trn_transferDeadlock()
//---------------------------------------------------------------------------------------------------------------------
{

}

//---------------------------------------------------------------------------------------------------------------------
//
// Lost the transfer - back to 001
void trn_transferLostGame()
//---------------------------------------------------------------------------------------------------------------------
{
	gam_setupPlayerDroid ();        // reset back to 001
	playerDroid.currentHealth = dataBaseEntry[playerDroid.droidType].maxHealth / 4;   // Not in a good way
}

//---------------------------------------------------------------------------------------------------------------------
//
// Lost the transfer - burnt out - game over
void trn_transferBurntOut()
//---------------------------------------------------------------------------------------------------------------------
{
	playerDroid.velocity = {0,0};
}

//---------------------------------------------------------------------------------------------------------------------
//
// Transfer into the droid
void trn_transferIntoDroid()
//---------------------------------------------------------------------------------------------------------------------
{
	gam_setInfluenceTimelimit (playerDroid.transferTargetDroidType);
	playerDroid.droidType = playerDroid.transferTargetDroidType;
	playerDroid.currentHealth = g_shipDeckItr->second.droid[playerDroid.transferTargetDroidIndex].currentHealth;
	playerDroid.velocity = {0,0};
	playerDroid.droidName = g_shipDeckItr->second.droid[playerDroid.transferTargetDroidIndex].droidName;
	playerDroid.sprite = g_shipDeckItr->second.droid[playerDroid.transferTargetDroidIndex].sprite;
	//
	// Destroy the droid
	g_shipDeckItr->second.droid[playerDroid.transferTargetDroidIndex].currentHealth = -10;
	gam_damageToDroid(playerDroid.transferTargetDroidIndex, PHYSIC_DAMAGE_BULLET, -1);
}

//---------------------------------------------------------------------------------------------------------------------
//
// Handle the end of transfer game
void trn_processEndOfTransferGame ()
//---------------------------------------------------------------------------------------------------------------------
{
	//
	// Wait for circuits to stop being active

	// count for player side

	// if less and player is 001 - explode
	// if less and player is > 001 - drop health, back to game

	// if more, do transfer
	// explode other droid - do score - call damageToDroid ?
	// back to game
}