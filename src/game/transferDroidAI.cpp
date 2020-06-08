#include <game/transferRender.h>
#include <game/player.h>
#include <game/transfer.h>
#include <game/audio.h>
#include "game/transferDroidAI.h"

int  numDroidTokens      = 0;
bool circuitFound        = false;
int  nextCircuitToUse    = 0;
int  enemyTokenDirection = 0;
int  droidBlockPos       = 0;
int  numPlayerTokens     = 0;
int  playerBlockPos      = 0;

float chooseRowDelay     = 1.0f;
float chooseRowDelayTime = 0.0f;

//------------------------------------------------------------
//
// Find a suitable circuit to use
//
// TODO: Also only choose ones that need to be changed color - maybe only for high droids
int findSuitableCircuitToUse (__TRANSFER_ROW transferIndex)
//------------------------------------------------------------
{
	int rowCount = 0;

//	for (auto transferIndex : transferRows)
	{
		if (transferPlayerWhichSide == TRANSFER_COLOR_LEFT)
		{
			if (transferIndex.rightSideActive)
			{
				circuitFound = false;
				return -1;
			}
			//
			// Droid is on right side - look for a suitable circuit
			switch (transferIndex.rightSideType)
			{
				// Decide what to do based on circuit type
				case TRANSFER_ROW_FULL_LINE:
				case TRANSFER_ROW_FULL_LINE_1:
				case TRANSFER_ROW_FULL_LINE_2:
				case TRANSFER_ROW_FULL_LINE_3:
				case TRANSFER_ROW_ONE_INTO_TWO_MIDDLE:
				case TRANSFER_ROW_REPEAT_HALF:
				case TRANSFER_ROW_REPEAT_QUARTER:
					circuitFound = true;
					return rowCount;
					break;

				case TRANSFER_ROW_HALF_LINE:
				case TRANSFER_ROW_3_4_LINE:
				case TRANSFER_ROW_QUARTER_LINE:
					circuitFound = trn_makeTransferMistake (playerDroid.transferTargetDroidType);
					return rowCount;
					break;

				case TRANSFER_ROW_REVERSE_HALF:
				case TRANSFER_ROW_REVERSE_QUARTER:
					circuitFound = trn_makeTransferMistake (playerDroid.transferTargetDroidType);
					return rowCount;
					break;

				case TRANSFER_ROW_TWO_INTO_ONE_MIDDLE:
					circuitFound = false;
					break;

				case TRANSFER_ROW_ONE_INTO_TWO_TOP:
				case TRANSFER_ROW_ONE_INTO_TWO_BOTTOM:
				case TRANSFER_ROW_TWO_INTO_ONE_TOP:
				case TRANSFER_ROW_TWO_INTO_ONE_BOTTOM:
					circuitFound = false;
					break;

				default:
					return -1;
					break;
			}
			rowCount++;
		}
	}
	return -1;
}

// ----------------------------------------------------------------------------
//
// Move the enemy token based on passed on direction
void moveEnemyToken (int direction)
// ----------------------------------------------------------------------------
{
//	printf ("Move enemy token\n");

	if (TRANSFER_MOVE_UP == direction)
	{
		droidBlockPos--;
		if (droidBlockPos < 0)
			droidBlockPos = transferRows.size () - 1;  // wrap around to bottom
	}

	if (TRANSFER_MOVE_DOWN == direction)
	{
		droidBlockPos++;
		if (droidBlockPos > transferRows.size () - 1)
			droidBlockPos = 0;          // wrap back to top
	}
	gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");
}

// ----------------------------------------------------------------------------
//
// Move to the circuit we have picked
void moveToCircuit ()
// ----------------------------------------------------------------------------
{
	//
	// Didn't find one to use - so just wander
	if (-1 == nextCircuitToUse)
	{
		moveEnemyToken (TRANSFER_MOVE_UP);
		return;
	}

	if (droidBlockPos == nextCircuitToUse)
	{
//			trn_placeToken (droidBlockPos, droidOnSide, DROID_TRANSFER);

		transferRows[nextCircuitToUse].rightSideActiveCounter = 3.0f;
		transferRows[nextCircuitToUse].rightSideActive        = true;
		switch (transferRows[nextCircuitToUse].rightSideType)
		{
			case TRANSFER_ROW_REPEAT_HALF:
			case TRANSFER_ROW_REPEAT_QUARTER:
				transferRows[nextCircuitToUse].rightSideActiveIsOn = true;
				break;

			default:
				break;
		}

		circuitFound = false;
		numDroidTokens--;
		if (numDroidTokens > 0)
			droidBlockPos = -1;
		else
			droidBlockPos = -2;
	}
	else
	{
		if (enemyTokenDirection == TRANSFER_MOVE_UP)
		{
			moveEnemyToken (TRANSFER_MOVE_UP);
		}
		else
		{
			moveEnemyToken (TRANSFER_MOVE_DOWN);
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------
//
// Play the transfer game
void trn_processTransferGame (float tickTime)
//---------------------------------------------------------------------------------------------------------------------
{
	// Move droid tokens
	// Check cell status and update colors
	// Check cell timers for energy stop

	int rowCounter = 0;

	if (numDroidTokens < 0)
		return;

	trn_processCircuits (tickTime);

	chooseRowDelay -= chooseRowDelayTime * tickTime;
	if (chooseRowDelay < 0.0f)
	{
		chooseRowDelay = 1.0f;

		if (circuitFound == false)
		{
			//
			// Look through each row for a suitable circuit to use
			for (auto &transferRowIndex : transferRows)
			{
				nextCircuitToUse = findSuitableCircuitToUse (transferRowIndex);
				if (nextCircuitToUse != -1)
				{
					circuitFound            = true;
					nextCircuitToUse        = rowCounter;
					if (nextCircuitToUse > transferRows.size () / 2)
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
			moveToCircuit ();
		}
	}
}