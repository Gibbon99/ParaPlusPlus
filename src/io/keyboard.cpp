#include <gui/guiLanguage.h>
#include <gui/guiInput.h>
#include <io/joystick.h>
#include <io/mouse.h>
#include <game/player.h>
#include <game/lifts.h>
#include <system/util.h>
#include <game/transfer.h>
#include <game/pauseMode.h>
#include <game/hud.h>
#include "io/keyboard.h"

static int settingCurrentKeyIndex;
static int previousGuiMode;

//----------------------------------------------------------------------------------------------------------------------
//
// Called when a key is pressed on the keyboard while in keyinput mode
void io_setNewKeycodeValue(int newKeyCode)
//----------------------------------------------------------------------------------------------------------------------
{
	gui.setScancode(settingCurrentKeyIndex, newKeyCode);

	sys_setNewMode(previousGuiMode, false);

	gam_setHudText("optionsMenu.controlsButton");
	gui.setCurrentScreen(gui.getIndex(GUI_OBJECT_SCREEN, "controlsMenu"));
	gui.setActiveObject(gui.getCurrentScreen(), GUI_OBJECT_BUTTON, "controlsMenu.backButton");

	paraScriptInstance.run ("as_refreshControlLabels", "");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Function called from the GUI script to change the mode to input a new key value
void io_initNewKeycodeValue(int whichKey)
//----------------------------------------------------------------------------------------------------------------------
{
	if ((whichKey < 0) || (whichKey > KEY_NUMBER_ACTIONS))
	{
		std::cout << "Invalid keyIndex passed to io_getNewKeycodeValue from script." << std::endl;
		return;
	}

	settingCurrentKeyIndex = whichKey;
	previousGuiMode = currentMode;
	sys_setNewMode(MODE_GUI_KEYCODE_ENTRY, false);
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

	if (playerDroid.getCurrentMode() == DROID_MODE_EXPLODING)
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
		case MODE_GUI_HIGHSCORE_DISPLAY:
		case MODE_GUI_HIGHSCORE_ENTRY:
			io_mapMouseToInput ();
			gui.processGuiInput ();
			gui_processKeyboard ();
			break;

		case MODE_GUI_WON_SCREEN:
		case MODE_GUI_INTROSCROLL:
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
			if (gui.getCurrentDialogbox() == NO_DIALOG_BOX)
			{
				if (!gam_pauseModeOn ())
				{
					gam_processPlayerMovement ();
					gam_processActionKey ();

					if (gui.keyDown (KEY_CONSOLE))
					{
						sys_setNewMode (MODE_CONSOLE_EDIT, true);
						gui.setState (KEY_CONSOLE, false, 0);
					}

					if (gui.keyDown (KEY_PAUSE))
					{
						gam_changePauseMode (MODE_GAME_PAUSE_ON);
						gui.setState (KEY_PAUSE, false, 0);
					}

					if (gui.keyDown (KEY_ESCAPE))
					{
						gui.setState (KEY_ESCAPE, false, 0);
						gui.setCurrentDialogbox (gui.getIndex (GUI_OBJECT_DIALOGBOX, "quitGameDialogbox"));
						gui.setActiveObjectDialogbox (gui.getCurrentDialogbox (), GUI_OBJECT_BUTTON, "quitGameDialogbox.denyButton");
					}
				}
				else
				{
					if (gui.keyDown (KEY_PAUSE))
					{
						gam_changePauseMode (MODE_GAME_PAUSE_OFF);
						gui.setState (KEY_PAUSE, false, 0);
					}
				}
			}
			else    // Only read dialogbox keyboard
			{

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
				gui.setState (i, true, 0);
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