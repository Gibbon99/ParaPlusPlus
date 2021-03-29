#include "game/transferRender.h"
#include "game/player.h"
#include "game/transfer.h"
#include "game/audio.h"
#include "game/transferDroidAI.h"

bool circuitFound        = false;
int  numDroidTokens      = 0;
int  nextCircuitToUse    = 0;
int  enemyTokenDirection = 0;
int  droidBlockPos       = 0;

float chooseRowDelay     = 1.0f;
float chooseRowDelayTime = 0.0f;

//---------------------------------------------------------------------------------------------------------------------
//
// Return if a circuit to be selected is a suitable color to change
// Return false if the circuit is already the correct color for the selected side
bool trn_isCircuitSuitable(__TRANSFER_ROW transferIndex)
//---------------------------------------------------------------------------------------------------------------------
{
	//
	// Player is on left - Droid Right
	if (transferPlayerWhichSide == TRANSFER_COLOR_LEFT)
	{
		if (transferIndex.currentColor == TRANSFER_COLOR_RIGHT)
		{
#ifdef MY_DEBUG
			printf("Droid on right - color is already right color\n");
#endif
			return false;
		}

	}
	else    // Droid is on left
	{
		if (transferIndex.currentColor == TRANSFER_COLOR_LEFT)
		{
#ifdef MY_DEBUG
			printf("Droid on left - color is already left color\n");
#endif
			return false;
		}
	}

	return true;
}

//---------------------------------------------------------------------------------------------------------------------
//
// Find a suitable circuit to use
//
int trn_findSuitableCircuitToUse (__TRANSFER_ROW transferIndex)
//---------------------------------------------------------------------------------------------------------------------
{
	int circuitType;

	//
	// Check if the circuit is already active - no good if it is
	//
	if (transferPlayerWhichSide == TRANSFER_COLOR_LEFT)
	{
		if (transferIndex.rightSideActive)
		{
			circuitFound = false;
			return -1;
		}

		circuitType = transferIndex.rightSideType;
	}
	else    // Droid is on left side
	{
		if (transferIndex.leftSideActive)
		{
			circuitFound = false;
			return -1;
		}

		circuitType = transferIndex.leftSideType;
	}

	switch (circuitType)
	{
		// Decide what to do based on circuit type
		case TRANSFER_ROW_FULL_LINE:
		case TRANSFER_ROW_FULL_LINE_1:
		case TRANSFER_ROW_FULL_LINE_2:
		case TRANSFER_ROW_FULL_LINE_3:
		case TRANSFER_ROW_REPEAT_HALF:
		case TRANSFER_ROW_REPEAT_QUARTER:
			circuitFound = trn_isCircuitSuitable(transferIndex);
			if (circuitFound)
				return 0;
			else
				return -1;
			break;

		case TRANSFER_ROW_ONE_INTO_TWO_MIDDLE:
			// Droid on right side
			if (transferPlayerWhichSide == TRANSFER_COLOR_LEFT)
			{
				if (transferRows[transferIndex.index - 1].currentColor == TRANSFER_COLOR_LEFT)  // Should set the middle row
				{
					circuitFound = true;
					return 0;
				}

				if (transferRows[transferIndex.index + 1].currentColor == TRANSFER_COLOR_LEFT)  // Should set the middle row
				{
					circuitFound = true;
					return 0;
				}
			}
			else    // Droid is on left side
			{
				if (transferRows[transferIndex.index - 1].currentColor == TRANSFER_COLOR_RIGHT)  // Should set the middle row
				{
					circuitFound = true;
					return 0;
				}

				if (transferRows[transferIndex.index + 1].currentColor == TRANSFER_COLOR_RIGHT)  // Should set the middle row
				{
					circuitFound = true;
					return 0;
				}
			}
			circuitFound = false;
			return -1;
			break;


		case TRANSFER_ROW_TWO_INTO_ONE_TOP:
			// Droid on right side
			if (transferPlayerWhichSide == TRANSFER_COLOR_LEFT)
			{
				if (transferRows[transferIndex.index + 1].currentColor == TRANSFER_COLOR_LEFT)  // Should set the middle row
				{
					circuitFound = true;
					return 0;
				}
			}
			else    // Droid is on left side
			{
				if (transferRows[transferIndex.index + 1].currentColor == TRANSFER_COLOR_RIGHT)    // Should set the middle row
				{
					circuitFound = true;
					return 0;
				}
			}
			circuitFound = false;
			return -1;

			break;

		case TRANSFER_ROW_TWO_INTO_ONE_BOTTOM:
			// Droid on right side
			if (transferPlayerWhichSide == TRANSFER_COLOR_LEFT)
			{
				if (transferRows[transferIndex.index - 1].currentColor == TRANSFER_COLOR_LEFT)  // Should set the middle row
				{
					circuitFound = true;
					return 0;
				}
			}
			else    // Droid is on left side
			{
				if (transferRows[transferIndex.index - 1].currentColor == TRANSFER_COLOR_RIGHT)    // Should set the middle row
				{
					circuitFound = true;
					return 0;
				}
			}
			circuitFound = false;
			return -1;

			break;


		case TRANSFER_ROW_HALF_LINE:
		case TRANSFER_ROW_3_4_LINE:
		case TRANSFER_ROW_QUARTER_LINE:
			circuitFound = false;
			return -1;
			break;


		case TRANSFER_ROW_REVERSE_HALF:
		case TRANSFER_ROW_REVERSE_QUARTER:
			circuitFound = trn_makeTransferMistake (playerDroid.transferTargetDroidType);
			return 0;
			break;

		case TRANSFER_ROW_TWO_INTO_ONE_MIDDLE:
			circuitFound = false;
			return -1;
			break;

		case TRANSFER_ROW_ONE_INTO_TWO_TOP:
		case TRANSFER_ROW_ONE_INTO_TWO_BOTTOM:
			circuitFound = false;
			return 0;
			break;

		default:
			circuitFound = false;
			return -1;
			break;
	}
}

//---------------------------------------------------------------------------------------------------------------------
//
// Move the enemy token based on passed on direction
void trn_moveEnemyToken (int direction)
//---------------------------------------------------------------------------------------------------------------------
{
	if (TRANSFER_MOVE_UP == direction)
	{
		droidBlockPos--;
		if (droidBlockPos < 0)
			droidBlockPos = static_cast<int>(transferRows.size ()) - 1;  // wrap around to bottom
	}

	if (TRANSFER_MOVE_DOWN == direction)
	{
		droidBlockPos++;
		if (droidBlockPos > static_cast<int>(transferRows.size ()) - 1)
			droidBlockPos = 0;          // wrap back to top
	}
	gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");
}

//---------------------------------------------------------------------------------------------------------------------
//
// Move to the circuit we have picked
void trn_moveToCircuit ()
//---------------------------------------------------------------------------------------------------------------------
{
	//
	// Didn't find one to use - so just wander
	if (-1 == nextCircuitToUse)
	{
		trn_moveEnemyToken (TRANSFER_MOVE_UP);
		return;
	}

	if (droidBlockPos == nextCircuitToUse)
	{
		if (transferPlayerWhichSide == TRANSFER_COLOR_LEFT)
			trn_placeToken (nextCircuitToUse, TRANSFER_COLOR_RIGHT, 0);
		else
			trn_placeToken (nextCircuitToUse, TRANSFER_COLOR_LEFT, 0);

		circuitFound = false;
		nextCircuitToUse = -1;
		numDroidTokens--;
		if (numDroidTokens > 0)
			droidBlockPos = -1;     // Put on launchpad
		else
			droidBlockPos = -2;

		gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "transferAction");
	}
	else
	{
		if (enemyTokenDirection == TRANSFER_MOVE_UP)
			trn_moveEnemyToken (TRANSFER_MOVE_UP);
		else
			trn_moveEnemyToken (TRANSFER_MOVE_DOWN);
	}
}

//---------------------------------------------------------------------------------------------------------------------
//
// Play the transfer game
void trn_processTransferDroidAI ()
//---------------------------------------------------------------------------------------------------------------------
{
	// Move droid tokens
	// Check cell status and update colors
	// Check cell timers for energy stop

	int rowCounter = 0;

	if (numDroidTokens < 0)
		return;

	chooseRowDelay -= 1.0f * chooseRowDelayTime;
	if (chooseRowDelay < 0.0f)
	{
		chooseRowDelay = 1.0f;

		if (!circuitFound)
		{
			//
			// Look through each row for a suitable circuit to use
			// rowCounter is the row that has been found suitable
			for (auto &transferRowIndex : transferRows)
			{
				nextCircuitToUse = trn_findSuitableCircuitToUse (transferRowIndex);
				if (nextCircuitToUse != -1)
				{
					circuitFound            = true;
					nextCircuitToUse        = rowCounter;
					//
					// Which way will the token move
					if (nextCircuitToUse > static_cast<int>(transferRows.size ()) / 2)
						enemyTokenDirection = TRANSFER_MOVE_UP;
					else
						enemyTokenDirection = TRANSFER_MOVE_DOWN;

					return;
				}
				rowCounter++;
			}
		}
		else
		{
			trn_moveToCircuit ();
		}
	}
}