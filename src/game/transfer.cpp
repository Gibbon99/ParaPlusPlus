#include <game/transferRender.h>
#include <classes/paraRandom.h>
#include <system/util.h>
#include <game/alertLevel.h>
#include <game/audio.h>
#include <game/player.h>
#include <game/database.h>
#include <game/texture.h>
#include "game/transfer.h"

paraRandom transferRandom;
int transferTargetDroidIndex;

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
	int  i          = 0;
	int  randNum    = 0;
	int  randEffect = 0;     // Will the circuit be reversed or repeater
	bool validCircuit;

	for (i = 0; i != numberTransferRows; i++)
	{
//
// Left side
//
		randNum = transferRandom.get (0, TRANSFER_ROW_FULL_LINE_3 - 1);
		randNum = randNum + 1;

		if (randNum > TRANSFER_ROW_FULL_LINE_3)
		{
			printf ("Randnum is larger than row type\n");
			exit (-1);
		}

		if (TRANSFER_ROW_ONE_INTO_TWO_MIDDLE == randNum)
			printf ("Got one into two middle\n");

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
//			as_transSetDefaultValues (i);

		randNum = transferRandom.get (0, TRANSFER_ROW_FULL_LINE_3);
		randNum = randNum + 1;

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
			if (trn_isCircuitSplit (transferRows[i].rightSideType))
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
void trn_initTransferScreenTwo()
//-------------------------------------------------------------------------------------------------------------------
{
	std::string    newFileName;
	std::string    newKeyName;

	newKeyName = "db_droid";
	newFileName = dataBaseEntry[g_shipDeckItr->second.droid[transferTargetDroidIndex].droidType].dbImageFileName + ".bmp";
	gam_loadTexture (newFileName, newKeyName);

	databaseSprite.setCurrentFrame(0);
	databaseSprite.setTintColor(64,64,64);

	sys_setNewMode(MODE_TRANSFER_SCREEN_TWO, true);
}

//-------------------------------------------------------------------------------------------------------------------
//
// Setup the default values for the transfer rows
void trn_initTransferValues (int transferTargetIndex)
//-------------------------------------------------------------------------------------------------------------------
{
	__TRANSFER_ROW tempTransferRow{};
	std::string    newFileName;
	std::string    newKeyName;

	if (transferRows.empty ())
	{
		for (int i = 0; i != numberTransferRows; i++)
		{
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
			tempTransferRow.rightSideActiveAlphaColor = 0.0f;
			tempTransferRow.rightSideActiveAlphaCount = 0.0f;
			tempTransferRow.leftSideActiveAlphaColor  = 0.0f;
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
	transferColorLeftActive.g = 0;
	transferColorLeftActive.b = 0;
	transferColorLeftActive.a = 255;

	transferColorRightActive.r = 255;
	transferColorRightActive.g = 255;
	transferColorRightActive.b = 0;
	transferColorRightActive.a = 255;

	transferTargetDroidIndex = transferTargetIndex;

	trn_setupTransferCellValues ();

	gam_stopAlertLevelSound (gam_getCurrentAlertLevel());
	gam_addAudioEvent (EVENT_ACTION_AUDIO_STOP, false, 0, 127, "transferMove");
	gui.setCurrentScreen (gui.getIndex (GUI_OBJECT_SCREEN, "guiTransferOne"));
	gui.setActiveObject (gui.getCurrentScreen (), GUI_OBJECT_BUTTON, "guiTransferOne.nextButton");
	gui.setState (KEY_ACTION, false, 0);

	newKeyName = "db_droid";
	newFileName = dataBaseEntry[playerDroid.droidType].dbImageFileName + ".bmp";
	gam_loadTexture (newFileName, newKeyName);
	databaseSprite.setCurrentFrame (0);
	databaseSprite.setTintColor (64, 64, 64);

	sys_setNewMode(MODE_TRANSFER_SCREEN_ONE, true);
}