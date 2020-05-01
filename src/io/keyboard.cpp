#include <gui/guiLanguage.h>
#include <gui/guiInput.h>
#include <io/joystick.h>
#include <io/mouse.h>
#include "io/keyboard.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Callback for filtering events
int filterEvents(void *userData, SDL_Event  *event)
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
void io_processKeyboardState()
//----------------------------------------------------------------------------------------------------------------------
{
	static bool funcDone = false;

	if (!funcDone)
	{
		funcDone = true;
//		SDL_SetEventFilter(filterEvents, nullptr);
	}

	gui.keyboardState = SDL_GetKeyboardState (nullptr);
	gui.update();
	io_mapJoyToInput();

	io_mapMouseToInput();

	switch (currentMode)
	{
		case MODE_GUI:
			gui.process();
			gui_processKeyboard();
			break;

		case MODE_SHOW_SPLASH:
			break;

		default:
			break;
	}
}