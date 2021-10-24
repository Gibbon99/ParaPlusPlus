#include "game/hud.h"
#include "game/audio.h"
#include "system/gameEvents.h"
#include "game/pauseMode.h"

bool pauseModeOn = false;

//-------------------------------------------------------------------------------------------------------------------------------------------
//
// Get the pause mode current state
bool gam_pauseModeOn()
//-------------------------------------------------------------------------------------------------------------------------------------------
{
	return pauseModeOn;
}

//-------------------------------------------------------------------------------------------------------------------------------------------
//
// Start or stop pause mode
void gam_changePauseMode(int newPauseMode)
//-------------------------------------------------------------------------------------------------------------------------------------------
{
	switch (newPauseMode)
	{
		case MODE_GAME_PAUSE_ON:
			gam_setHudText ("hudPauseMode");
			gam_addAudioEvent (EVENT_ACTION_AUDIO_STOP_ALL, false, 0, 0, "");
			pauseModeOn = true;
			break;

		case MODE_GAME_PAUSE_OFF:
			gam_addEvent (EVENT_ACTION_AUDIO_START_BACKGROUND, 0, "");
			pauseModeOn = false;
			break;
	}
}
