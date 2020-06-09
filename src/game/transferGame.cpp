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
			sys_setNewMode (MODE_PRE_TRANSFER_GAME, false);
		}
	}

	trn_processTransferDroidAI ();
	trn_processPlayerActions();
	trn_processCircuits ();
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