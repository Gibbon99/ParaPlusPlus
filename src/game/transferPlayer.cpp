#include "game/transferGame.h"
#include "game/audio.h"
#include "game/transferRender.h"
#include "game/transferPlayer.h"

int numPlayerTokens = 0;

// ----------------------------------------------------------------------------
//
// Move the player token based on passed on direction
void trn_movePlayerToken(int direction)
// ----------------------------------------------------------------------------
{
	if (TRANSFER_MOVE_UP == direction)
	{
		playerBlockPos--;
		if (playerBlockPos < 0)
			playerBlockPos = static_cast<int>(transferRows.size () - 1);  // wrap around to bottom
	}

	if (TRANSFER_MOVE_DOWN == direction)
	{
		playerBlockPos++;
		if (playerBlockPos > static_cast<int>(transferRows.size () - 1))
			playerBlockPos = 0;          // wrap back to top
	}
	gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");
}

//---------------------------------------------------------------------------------------------------------------------
//
// Do transfer movements for player
void trn_processPlayerActions()
//---------------------------------------------------------------------------------------------------------------------
{
	if (numPlayerTokens == -1)       // 0 or -1
		return;

	if (gui.keyDown (KEY_UP))
	{
		trn_movePlayerToken (TRANSFER_MOVE_UP);
		gui.setState (KEY_UP, false, 0);
		return;
	}

	if (gui.keyDown (KEY_DOWN))
	{
		trn_movePlayerToken (TRANSFER_MOVE_DOWN);
		gui.setState (KEY_DOWN, false, 0);
		return;
	}

	if (gui.keyDown (KEY_ACTION))
	{
		gui.setState (KEY_ACTION, false, 0);

		if (transferPlayerWhichSide == TRANSFER_COLOR_LEFT)
		{
			if (transferRows[playerBlockPos].leftSideActive)      // Can't put a token on already active circuit
			{
				if ((transferRows[playerBlockPos].leftSideType != TRANSFER_ROW_ONE_INTO_TWO_TOP) || (transferRows[playerBlockPos].leftSideType != TRANSFER_ROW_ONE_INTO_TWO_BOTTOM))
				{
					gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad");
					return;
				}
			}
			else
			{
				trn_placeToken (playerBlockPos, TRANSFER_COLOR_LEFT);
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "transferAction");
			}
		}
		else    // Player on right side
		{
			if (transferRows[playerBlockPos].rightSideActive)   // Can't put a token on already active circuit
			{
				if ((transferRows[playerBlockPos].rightSideType != TRANSFER_ROW_ONE_INTO_TWO_TOP) || (transferRows[playerBlockPos].rightSideType != TRANSFER_ROW_ONE_INTO_TWO_BOTTOM))
				{
					gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad");
					return;
				}
			}
			else
			{
				trn_placeToken (playerBlockPos, TRANSFER_COLOR_RIGHT);
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "transferAction");
			}
		}

		numPlayerTokens--;
		if (numPlayerTokens > 0)
			playerBlockPos = -1;     // Put on launchpad
		else
			playerBlockPos = -2;

		return;
	}
}
