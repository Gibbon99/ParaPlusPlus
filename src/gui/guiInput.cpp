#include <io/keyboard.h>

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

		std::cout << "ACTION key" << std::endl;

	}
}
