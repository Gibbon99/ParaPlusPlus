#include <gui/guiLanguage.h>
#include <gui/guiInput.h>
#include <io/joystick.h>
#include "io/keyboard.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Process the keyboard state and populate the keybindings array
void io_processKeyboardState()
//----------------------------------------------------------------------------------------------------------------------
{
	input.update();
	io_mapJoyToInput();

	switch (currentMode)
	{
		case MODE_GUI:
			gui_processKeyboard();
			break;

		case MODE_SHOW_SPLASH:
			break;

		default:
			break;
	}
}