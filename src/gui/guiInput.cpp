#include <io/keyboard.h>
#include <game/audio.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Process the keyboard state
void gui_processKeyboard()
//----------------------------------------------------------------------------------------------------------------------
{
	if (gui.keyDown (KEY_LEFT))
	{

		std::cout << "LEFT key" << std::endl;

	}

	if (gui.keyDown (KEY_RIGHT))
	{

		std::cout << "RIGHT key" << std::endl;

	}

	if (gui.keyDown (KEY_UP))
	{

		std::cout << "UP key" << std::endl;

	}

	if (gui.keyDown (KEY_DOWN))
	{

		std::cout << "DOWN key" << std::endl;

	}

	if (gui.keyDown(KEY_ACTION))
	{

		switch (currentMode)
		{
			case MODE_GUI_INTROSCROLL:
				gam_addAudioEvent(EVENT_ACTION_AUDIO_STOP, false, 0, 0, "scrollBeeps");
				sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_CHANGE_MODE, 0, to_string(MODE_GUI_MAINMENU)+"|"+to_string(true));
				break;

			case MODE_GUI_WON_SCREEN:
			case MODE_GUI_HIGHSCORE_SCREEN:
				sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_CHANGE_MODE, 0, to_string(MODE_GUI_MAINMENU)+"|"+to_string(true));
				break;
		}
	}
}
