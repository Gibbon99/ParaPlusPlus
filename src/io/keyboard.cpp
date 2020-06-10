#include <gui/guiLanguage.h>
#include <gui/guiInput.h>
#include <io/joystick.h>
#include <io/mouse.h>
#include <game/player.h>
#include <game/lifts.h>
#include <system/util.h>
#include <game/transfer.h>
#include <game/pauseMode.h>
#include "io/keyboard.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Callback for filtering events
int filterEvents (void *userData, SDL_Event *event)
//----------------------------------------------------------------------------------------------------------------------
{
	static int dropCount = 0;

	if (event->type == SDL_KEYDOWN)
	{
		dropCount++;
		if (dropCount > 5)
		{
			dropCount = 0;
			return 1;
		}
		return 0;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Process the keyboard state and populate the keybindings array
void io_processKeyboardState ()
//----------------------------------------------------------------------------------------------------------------------
{
	//
	// Don't process any key events until fade is finished and screen is ready
	if (renderer.currentFadeState != FADE_STATE_NONE)
		return;

//io_mapJoyToInput();       // TODO uncomment and calibrate



	switch (currentMode)
	{
		case MODE_GUI_TUT_TRANSFER_START:
		case MODE_GUI_TUT_TRANSFER_GAME:
		case MODE_GUI_TUT_LIFTS:
		case MODE_GUI_TUT_TERMINALS:
		case MODE_GUI_TUT_HEALING:
		case MODE_GUI_TUT_TIPS:

		case MODE_GUI_MAINMENU:
		case MODE_GUI_TERMINAL:
		case MODE_GUI_DATABASE:
		case MODE_GUI_DECKVIEW:
		case MODE_GUI_SHIPVIEW:
		case MODE_TRANSFER_SCREEN_ONE:
		case MODE_TRANSFER_SCREEN_TWO:
			io_mapMouseToInput ();
			gui.processGuiInput ();
			gui_processKeyboard ();
			break;

		case MODE_GUI_INTROSCROLL:
		case MODE_GUI_WON_SCREEN:
		case MODE_GUI_HIGHSCORE_SCREEN:
			gui_processKeyboard ();
			break;

		case MODE_TRANSFER_CHOOSE_SIDE:
			trn_handleTransferChooseSide ();
			break;

		case MODE_GUI_LIFTVIEW:
			if (gui.keyDown (KEY_UP))
			{
				gam_moveLift (1);
				gui.setState (KEY_UP, false, 0);
			}

			if (gui.keyDown (KEY_DOWN))
			{
				gam_moveLift (2);
				gui.setState (KEY_DOWN, false, 0);
			}

			if (gui.keyDown ((KEY_ACTION)))
			{
				gam_moveLift (3);
				gui.setState (KEY_ACTION, false, 0);
			}
			break;

		case MODE_SHOW_SPLASH:
			break;

		case MODE_GAME:
			if (!gam_pauseModeOn())
			{
				gam_processPlayerMovement ();
				gam_processActionKey ();

				if (gui.keyDown (KEY_CONSOLE))
				{
					sys_setNewMode (MODE_CONSOLE_EDIT, true);
					gui.setState (KEY_CONSOLE, false, 0);
				}

				if (gui.keyDown(KEY_PAUSE))
				{
					gam_changePauseMode (MODE_GAME_PAUSE_ON);
					gui.setState(KEY_PAUSE, false, 0);
				}
			}
			else
			{
				if (gui.keyDown(KEY_PAUSE))
				{
					gam_changePauseMode (MODE_GAME_PAUSE_OFF);
					gui.setState(KEY_PAUSE, false, 0);
				}
			}
			break;

		case MODE_CONSOLE_EDIT:
			if (gui.keyDown (KEY_CONSOLE))
			{
				sys_setNewMode (MODE_GAME, true);
				gui.setState (KEY_CONSOLE, false, 0);
			}
		default:
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Read the keyboard events and set the state in the gui class
void io_setKeyboardState (SDL_Scancode keycode, int action, bool repeatPress)
//----------------------------------------------------------------------------------------------------------------------
{
	if (action == SDL_PRESSED)
	{
		for (auto i = 0; i != KEY_NUMBER_ACTIONS; i++)
		{
			if (keycode == gui.getScancode (i))
			{
//				if ((repeatPress == false) && (gui.getRepeatOff () == true))
				gui.setState (i, true, 0);
//				else
//					gui.setState (i, false, 0);
			}
		}
	}

	if (action == SDL_RELEASED)
	{
		for (auto i = 0; i != KEY_NUMBER_ACTIONS; i++)
		{
			if (keycode == gui.getScancode (i))
			{
				gui.setState (i, false, 0);
			}
		}
	}
}