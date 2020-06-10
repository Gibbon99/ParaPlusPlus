#include <gui/guiLanguage.h>
#include <game/hud.h>
#include <game/transferGame.h>
#include <game/transferPlayer.h>
#include "game/transferRender.h"
#include "classes/paraRandom.h"
#include "system/util.h"
#include "game/alertLevel.h"
#include "game/audio.h"
#include "game/player.h"
#include "game/database.h"
#include "game/texture.h"
#include "game/transferDroidAI.h"
#include "game/transfer.h"

paraRandom transferRandom;
int        chooseSideTimeOut;  // From script
float      chooseSideDelayTime; // From script
float      activeRowCounter;
int        sideTimeoutCountdown;

//---------------------------------------------------------------------------------------------------------------------
//
// Print out the types of cells
void trn_debugTransferCells (int whichSide)
//---------------------------------------------------------------------------------------------------------------------
{
	for (auto cellItr : transferRows)
	{
		if (whichSide == TRANSFER_COLOR_LEFT)
			std::cout << trn_getRowName (cellItr.leftSideType) << std::endl;
		else
			std::cout << trn_getRowName (cellItr.rightSideType) << std::endl;
	}
}

//---------------------------------------------------------------------------------------------------------------------
//
// Process active circuits time
void trn_processCircuits ()
//---------------------------------------------------------------------------------------------------------------------
{
	int leftSideCount  = 0;
	int rightSideCount = 0;

	for (auto &transferRowIndex : transferRows)
	{
		if ((transferRowIndex.rightSideActive) || (transferRowIndex.rightSideActiveIsOn))
		{
			transferRowIndex.rightSideActiveCounter -= 1.0f * activeRowCounter;
			if (transferRowIndex.rightSideActiveCounter < 0.0f)
			{
				transferRowIndex.rightSideActiveCounter = 1.0f;
				transferRowIndex.rightSideActive        = false;
			}
			//
			// Flicker the active circuits
			transferRowIndex.rightSideActiveAlphaColor -= 10.0f;
			if (transferRowIndex.rightSideActiveAlphaColor < 200.0f)
				transferRowIndex.rightSideActiveAlphaColor = static_cast<float>(transferRandom.get (200, 254));

			switch (transferRowIndex.rightSideType)
			{
				case TRANSFER_ROW_FULL_LINE:
				case TRANSFER_ROW_FULL_LINE_1:
				case TRANSFER_ROW_FULL_LINE_2:
				case TRANSFER_ROW_FULL_LINE_3:
				case TRANSFER_ROW_REPEAT_HALF:
				case TRANSFER_ROW_REPEAT_QUARTER:
					transferRowIndex.currentColor = TRANSFER_COLOR_RIGHT;
					break;

				case TRANSFER_ROW_REVERSE_HALF:
				case TRANSFER_ROW_REVERSE_QUARTER:
					//
					// Reverse the color effect
					transferRowIndex.currentColor = TRANSFER_COLOR_LEFT;
					break;

				case TRANSFER_ROW_TWO_INTO_ONE_TOP:
					if ((transferRowIndex.rightSideActive) && (transferRows[transferRowIndex.index + 2].rightSideActive))
					{
						transferRows[transferRowIndex.index + 1].currentColor = TRANSFER_COLOR_RIGHT;
					}
					break;

				case TRANSFER_ROW_TWO_INTO_ONE_BOTTOM:
					if ((transferRowIndex.rightSideActive) && (transferRows[transferRowIndex.index - 2].rightSideActive))
					{
						transferRows[transferRowIndex.index - 1].currentColor = TRANSFER_COLOR_RIGHT;
					}
					break;

				case TRANSFER_ROW_ONE_INTO_TWO_TOP:
					if (transferRows[transferRowIndex.index + 1].rightSideActive)
					{
						transferRowIndex.currentColor = TRANSFER_COLOR_RIGHT;
					}
					break;

				case TRANSFER_ROW_ONE_INTO_TWO_BOTTOM:
					if (transferRows[transferRowIndex.index - 1].rightSideActive)
					{
						transferRowIndex.currentColor = TRANSFER_COLOR_RIGHT;
					}
					break;

				case TRANSFER_ROW_ONE_INTO_TWO_MIDDLE:
					transferRows[transferRowIndex.index - 1].currentColor = TRANSFER_COLOR_RIGHT;
					transferRows[transferRowIndex.index + 1].currentColor = TRANSFER_COLOR_RIGHT;
					break;
			}
		}

		if ((transferRowIndex.leftSideActive) || (transferRowIndex.leftSideActiveIsOn))
		{
			transferRowIndex.leftSideActiveCounter -= 1.0f * activeRowCounter;
			if (transferRowIndex.leftSideActiveCounter < 0.0f)
			{
				transferRowIndex.leftSideActiveCounter = 1.0f;
				transferRowIndex.leftSideActive        = false;
			}
			//
			// Flicker the active circuits
			transferRowIndex.leftSideActiveAlphaColor -= 10.0f;
			if (transferRowIndex.leftSideActiveAlphaColor < 200.0f)
				transferRowIndex.leftSideActiveAlphaColor = static_cast<float>(transferRandom.get (200, 254));

			//
			// Check the left side for setting the color
			switch (transferRowIndex.leftSideType)
			{
				case TRANSFER_ROW_FULL_LINE:
				case TRANSFER_ROW_FULL_LINE_1:
				case TRANSFER_ROW_FULL_LINE_2:
				case TRANSFER_ROW_FULL_LINE_3:
				case TRANSFER_ROW_REPEAT_HALF:
				case TRANSFER_ROW_REPEAT_QUARTER:
					transferRowIndex.currentColor = TRANSFER_COLOR_LEFT;
					break;

				case TRANSFER_ROW_REVERSE_HALF:
				case TRANSFER_ROW_REVERSE_QUARTER:
					//
					// Reverse the color effect
					transferRowIndex.currentColor = TRANSFER_COLOR_RIGHT;
					break;

				case TRANSFER_ROW_TWO_INTO_ONE_TOP:
					if ((transferRowIndex.leftSideActive) && (transferRows[transferRowIndex.index + 2].leftSideActive))
					{
						transferRows[transferRowIndex.index + 1].currentColor = TRANSFER_COLOR_LEFT;
					}
					break;

				case TRANSFER_ROW_TWO_INTO_ONE_BOTTOM:
					if ((transferRowIndex.leftSideActive) && (transferRows[transferRowIndex.index - 2].leftSideActive))
					{
						transferRows[transferRowIndex.index - 1].currentColor = TRANSFER_COLOR_LEFT;
					}
					break;

				case TRANSFER_ROW_ONE_INTO_TWO_TOP:
					if (transferRows[transferRowIndex.index + 1].leftSideActive)
					{
						transferRowIndex.currentColor = TRANSFER_COLOR_LEFT;
					}
					break;

				case TRANSFER_ROW_ONE_INTO_TWO_BOTTOM:
					if (transferRows[transferRowIndex.index - 1].leftSideActive)
					{
						transferRowIndex.currentColor = TRANSFER_COLOR_LEFT;
					}
					break;

				case TRANSFER_ROW_ONE_INTO_TWO_MIDDLE:
					transferRows[transferRowIndex.index - 1].currentColor = TRANSFER_COLOR_LEFT;
					transferRows[transferRowIndex.index + 1].currentColor = TRANSFER_COLOR_LEFT;
					break;
			}
		}

		if (transferRowIndex.currentColor == TRANSFER_COLOR_LEFT)
			leftSideCount++;
		else
			rightSideCount++;
	}

	//
	// Get current row type - compare against other legit row types, and then set color to dispute
	// Return false for something like two_into_one_top
	//
	for (auto &transferRowIndex : transferRows)
	{
		if (((transferRowIndex.leftSideActive) || (transferRowIndex.leftSideActiveIsOn)) &&
		(((transferRowIndex.rightSideActive) || transferRowIndex.rightSideActiveIsOn)))
		{
			transferRowIndex.currentColor = TRANSFER_COLOR_DISPUTE;
		}
	}

	if (leftSideCount > rightSideCount)
	{
		transferColorStatusCell = transferColorLeft;
	}
	else if (rightSideCount > leftSideCount)
	{
		transferColorStatusCell = transferColorRight;
	}
	else
	{
		transferColorStatusCell.r = transferRandom.get (100, 200);
		transferColorStatusCell.g = transferRandom.get (100, 200);
		transferColorStatusCell.b = transferRandom.get (100, 200);
		transferColorStatusCell.a = transferRandom.get (100, 200);
	}
}

//---------------------------------------------------------------------------------------------------------------------
//
// Return true if a transfer mistake is made
bool trn_makeTransferMistake (int whichDroidLevel)
//---------------------------------------------------------------------------------------------------------------------
{
	int mistakeLevel = 0;

	mistakeLevel = transferRandom.get (0, 25);
	mistakeLevel -= whichDroidLevel;

	return mistakeLevel > 0;
}

//--------------------------------------------------------
//
// Helper function - return true if circuit is a split type
bool trn_isCircuitSplit (int whichType)
//--------------------------------------------------------
{
	return (whichType == TRANSFER_ROW_TWO_INTO_ONE_MIDDLE) || (whichType == TRANSFER_ROW_ONE_INTO_TWO_MIDDLE) || (whichType == TRANSFER_ROW_TWO_INTO_ONE_BOTTOM) || (whichType == TRANSFER_ROW_TWO_INTO_ONE_TOP) ||
	       (whichType == TRANSFER_ROW_ONE_INTO_TWO_TOP) || (whichType == TRANSFER_ROW_ONE_INTO_TWO_BOTTOM);
}

//------------------------------------------------------------
//
// Called to setup the transfer cell values
void trn_setupTransferCellValues ()
//------------------------------------------------------------
{
	int i       = 0;
	int randNum = 0;

	//
	// Left side
	//
	for (i = 0; i != numberTransferRows; i++)
	{
		randNum = transferRandom.get (0, TRANSFER_ROW_FULL_LINE_3 - 1);

		if (randNum > TRANSFER_ROW_FULL_LINE_3)
			sys_shutdownWithError (sys_getString ("Transfer cell setup : Randnum is larger than row type."));

		if ((i == 0) || (i == numberTransferRows - 1))  // Can't have split on first or last slot
		{
			if (trn_isCircuitSplit (randNum))
				transferRows[i].leftSideType = TRANSFER_ROW_FULL_LINE;
			else
				transferRows[i].leftSideType = randNum;
		}
		else if (TRANSFER_ROW_ONE_INTO_TWO_MIDDLE == randNum)
		{
			if (trn_isCircuitSplit (transferRows[i - 1].leftSideType))
				transferRows[i].leftSideType = TRANSFER_ROW_FULL_LINE;
			else
			{
				transferRows[i].leftSideType     = randNum;
				transferRows[i - 1].leftSideType = TRANSFER_ROW_ONE_INTO_TWO_TOP;
				i++;
				transferRows[i].leftSideType = TRANSFER_ROW_ONE_INTO_TWO_BOTTOM;
			}
		}
		else if (TRANSFER_ROW_TWO_INTO_ONE_MIDDLE == randNum)
		{
			if (trn_isCircuitSplit (transferRows[i - 1].leftSideType))
				transferRows[i].leftSideType = TRANSFER_ROW_FULL_LINE;
			else
			{
				transferRows[i].leftSideType     = randNum;
				transferRows[i - 1].leftSideType = TRANSFER_ROW_TWO_INTO_ONE_TOP;
				i++;
				transferRows[i].leftSideType = TRANSFER_ROW_TWO_INTO_ONE_BOTTOM;
			}
		}
		else // Ok to use non split one here
		{
			transferRows[i].leftSideType = randNum;
		}
	}
	//
	// Right side
	//
	for (i = 0; i != numberTransferRows; i++)
	{
		randNum = transferRandom.get (0, TRANSFER_ROW_FULL_LINE_3);

		if ((i == 0) || (i == numberTransferRows - 1))  // Can't have split on first or last slot
		{
			if (trn_isCircuitSplit (randNum))
				transferRows[i].rightSideType = TRANSFER_ROW_FULL_LINE;
			else
				transferRows[i].rightSideType = randNum;
		}
		else if (TRANSFER_ROW_ONE_INTO_TWO_MIDDLE == randNum)
		{
			if (trn_isCircuitSplit (transferRows[i - 1].rightSideType))
				transferRows[i].rightSideType = TRANSFER_ROW_FULL_LINE;
			else
			{
				transferRows[i].rightSideType     = randNum;
				transferRows[i - 1].rightSideType = TRANSFER_ROW_ONE_INTO_TWO_TOP;
				i++;
				transferRows[i].rightSideType = TRANSFER_ROW_ONE_INTO_TWO_BOTTOM;
			}
		}
		else if (TRANSFER_ROW_TWO_INTO_ONE_MIDDLE == randNum)
		{
			if (trn_isCircuitSplit (transferRows[i - 1].rightSideType))
				transferRows[i].rightSideType = TRANSFER_ROW_FULL_LINE;
			else
			{
				transferRows[i].rightSideType     = randNum;
				transferRows[i - 1].rightSideType = TRANSFER_ROW_TWO_INTO_ONE_TOP;
				i++;
				transferRows[i].rightSideType = TRANSFER_ROW_TWO_INTO_ONE_BOTTOM;
			}
		}
		else // Ok to use non split one here
		{
			transferRows[i].rightSideType = randNum;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
//
// Get ready for the second transfer screen
void trn_initTransferScreenTwo ()
//-------------------------------------------------------------------------------------------------------------------
{
	std::string newFileName;
	std::string newKeyName;

	newKeyName  = "db_droid";
	newFileName = dataBaseEntry[g_shipDeckItr->second.droid[playerDroid.transferTargetDroidIndex].droidType].dbImageFileName + ".bmp";
	gam_loadTexture (newFileName, newKeyName);

	databaseSprite.setCurrentFrame (0);
	databaseSprite.setTintColor (64, 64, 64);

	gam_addAudioEvent (EVENT_ACTION_AUDIO_STOP, false, 0, 0, "transferStage1");
	gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "transferStage2");

	sys_setNewMode (MODE_TRANSFER_SCREEN_TWO, true);
}

//-------------------------------------------------------------------------------------------------------------------
//
// Setup the default values for the transfer rows - gameEvent EVENT_ACTION_INIT_TRANSFER_MODE
void trn_initTransferValues (int transferTargetIndex)
//-------------------------------------------------------------------------------------------------------------------
{
	__TRANSFER_ROW tempTransferRow{};
	std::string    newFileName;
	std::string    newKeyName;

	gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "transferStage1");

	if (transferRows.empty ())
	{
		for (int i = 0; i != numberTransferRows; i++)
		{
			tempTransferRow.index                     = i;
			tempTransferRow.startX                    = static_cast<float>(transferBitmapWidth) / 2;
			tempTransferRow.startY                    = transferRowStartY + (static_cast<float>(i) * transferRowHeight);
			tempTransferRow.leftSideActiveCounter     = 0.0f;
			tempTransferRow.rightSideActiveCounter    = 0.0f;
			tempTransferRow.leftSideType              = TRANSFER_ROW_REVERSE_QUARTER;
			tempTransferRow.rightSideType             = TRANSFER_ROW_REVERSE_QUARTER;
			tempTransferRow.leftSideActive            = false;
			tempTransferRow.rightSideActive           = false;
			tempTransferRow.rightSideActiveIsOn       = false;
			tempTransferRow.leftSideActiveIsOn        = false;
			tempTransferRow.rightSideActiveAlphaColor = 255.0f;
			tempTransferRow.rightSideActiveAlphaCount = 0.0f;
			tempTransferRow.leftSideActiveAlphaColor  = 255.0f;
			tempTransferRow.leftSideActiveAlphaCount  = 0.0f;

			if (i % 2 == 0)
				tempTransferRow.currentColor = TRANSFER_COLOR_LEFT;
			else
				tempTransferRow.currentColor = TRANSFER_COLOR_RIGHT;

			transferRows.push_back (tempTransferRow);
		}
	}

	transferColorStatusCell = transferColorBorder;
	transferPlayerWhichSide = TRANSFER_COLOR_LEFT;

	activeTokenPosX = BLOCK_WIDTH * 3;

	transferColorLeftActive.r = 255;
	transferColorLeftActive.g = 255;
	transferColorLeftActive.b = 0;
	transferColorLeftActive.a = 255;

	transferColorRightActive.r = 0;
	transferColorRightActive.g = 255;
	transferColorRightActive.b = 255;
	transferColorRightActive.a = 255;

	droidBlockPos  = -1;
	playerBlockPos = -1;

	playerDroid.transferTargetDroidIndex = transferTargetIndex;
	playerDroid.transferTargetDroidType  = g_shipDeckItr->second.droid[transferTargetIndex].droidType;

	numDroidTokens  = dataBaseEntry[g_shipDeckItr->second.droid[transferTargetIndex].droidType].tokenCount;
	numPlayerTokens = dataBaseEntry[playerDroid.droidType].tokenCount;

	trn_setupTransferCellValues ();

	gam_stopAlertLevelSound (gam_getCurrentAlertLevel ());
	gam_addAudioEvent (EVENT_ACTION_AUDIO_STOP, false, 0, 127, "transferMove");
	gui.setCurrentScreen (gui.getIndex (GUI_OBJECT_SCREEN, "guiTransferOne"));
	gui.setActiveObject (gui.getCurrentScreen (), GUI_OBJECT_BUTTON, "guiTransferOne.nextButton");
	gui.setState (KEY_ACTION, false, 0);

	newKeyName  = "db_droid";
	newFileName = dataBaseEntry[playerDroid.droidType].dbImageFileName + ".bmp";
	gam_loadTexture (newFileName, newKeyName);
	databaseSprite.setCurrentFrame (0);
	databaseSprite.setTintColor (64, 64, 64);

	sys_setNewMode (MODE_TRANSFER_SCREEN_ONE, true);
}

//---------------------------------------------------------------------------------------------------------------------
//
// Check the playing sounds for transfer information screens, move on if still playing
void trn_checkTransferScreenSounds ()
//---------------------------------------------------------------------------------------------------------------------
{
	switch (currentMode)
	{
		case MODE_TRANSFER_SCREEN_ONE:
			if (!audio.isPlaying ("transferStage1"))
			{
				sys_setNewMode (MODE_TRANSFER_PRE_SCREEN_TWO, false);
				return;
			}
			break;

		case MODE_TRANSFER_SCREEN_TWO:
			if (!audio.isPlaying ("transferStage2"))
			{
				sys_setNewMode (MODE_PRE_TRANSFER_CHOOSE_SIDE, false);
				return;
			}
			break;
	}
}

//---------------------------------------------------------------------------------------------------------------------
//
// Handle the keyboard for choosing a side for transfer game
void trn_handleTransferChooseSide ()
//---------------------------------------------------------------------------------------------------------------------
{
	if (gui.keyDown (KEY_LEFT))
	{
		gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");
		transferPlayerWhichSide = TRANSFER_COLOR_LEFT;
		gui.setState (KEY_LEFT, false, 0);
		return;
	}

	if (gui.keyDown (KEY_RIGHT))
	{
		gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");
		transferPlayerWhichSide = TRANSFER_COLOR_RIGHT;
		gui.setState (KEY_RIGHT, false, 0);
		return;
	}

	if (gui.keyDown (KEY_ACTION))
	{
		gui.setState (KEY_ACTION, false, 0);
		sys_setNewMode (MODE_PRE_TRANSFER_GAME, false);
	}
}

//---------------------------------------------------------------------------------------------------------------------
//
// Process the countdown timer for transfer - mode MODE_TRANSFER_CHOOSE_SIDE
void trn_processTransferCountDown ()
//---------------------------------------------------------------------------------------------------------------------
{
	static float sideTimeoutCounter = 0.0f;

	gam_setHudText ("");

	sideTimeoutCounter -= 1.0f * chooseSideDelayTime;
	if (sideTimeoutCounter < 0.0)
	{
		sideTimeoutCounter = 1.0f;
		sideTimeoutCountdown--;
		if (sideTimeoutCountdown < 0)
		{
			sideTimeoutCountdown = chooseSideTimeOut;
			sys_setNewMode (MODE_PRE_TRANSFER_GAME, false);
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
//
// Prepare to start the transfer game - mode MODE_PRE_TRANSFER_GAME
void trn_prepareTransferGame ()
//---------------------------------------------------------------------------------------------------------------------
{
	transferTimeoutCountdown = transferTimeOut;
	sys_setNewMode (MODE_TRANSFER_GAME, false);
}

//---------------------------------------------------------------------------------------------------------------------
//
// Return the time to display on the HUD
std::string trn_getCountdown ()
//---------------------------------------------------------------------------------------------------------------------
{
	return sys_getString ("%s %i", gui_getString ("selectSide").c_str (), sideTimeoutCountdown);
}

//---------------------------------------------------------------------------------------------------------------------
//
// Prepare the transfer countdown sequence - mode MODE_PRE_TRANSFER_CHOOSE_SIDE
void trn_prepareTransferCountDown ()
//---------------------------------------------------------------------------------------------------------------------
{
	sideTimeoutCountdown = chooseSideTimeOut;
	gam_addAudioEvent (EVENT_ACTION_AUDIO_STOP, false, 0, 0, "transferStage2");
	gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "transferStart");
	sys_setNewMode (MODE_TRANSFER_CHOOSE_SIDE, false);
}
