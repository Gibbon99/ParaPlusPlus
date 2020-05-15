#include <gui/guiLanguage.h>
#include <gui/guiInput.h>
#include <io/joystick.h>
#include <io/mouse.h>
#include <game/player.h>
#include <game/lifts.h>
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
//	gui.keyboardState = SDL_GetKeyboardState (nullptr);
//	gui.update();

//io_mapJoyToInput();

	io_mapMouseToInput ();

	switch (currentMode)
	{
		case MODE_GUI_MAINMENU:
		case MODE_GUI_TERMINAL:
		case MODE_GUI_DATABASE:
		case MODE_GUI_DECKVIEW:
		case MODE_GUI_SHIPVIEW:
			gui.processGuiInput();
			gui_processKeyboard ();
			break;

		case MODE_GUI_LIFTVIEW:
//			gui.process();
			if (gui.keyDown (KEY_UP))
			{
				gam_moveLift (1);
				gui.setState(KEY_UP, false, 0);
			}

			if (gui.keyDown (KEY_DOWN))
			{
				gam_moveLift (2);
				gui.setState(KEY_DOWN, false, 0);
			}

			if (gui.keyDown ((KEY_ACTION)))
			{
				gam_moveLift (3);
				gui.setState(KEY_ACTION, false, 0);
			}
			break;

		case MODE_SHOW_SPLASH:
			break;

		case MODE_GAME:
			gam_processPlayerMovement ();
			if (gui.keyDown (KEY_ACTION))
				gam_processActionKey ();
			break;

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