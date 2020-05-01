#include <io/keyboard.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Process the keyboard state
void gui_processKeyboard()
//----------------------------------------------------------------------------------------------------------------------
{
	if (gui.keyDown (KEY_LEFT))
	{
		std::cout << "GUI key left is down" << std::endl;
	}

	if (gui.keyDown (KEY_RIGHT))
	{
		std::cout << "GUI key right is down" << std::endl;
	}

	if (gui.keyDown (KEY_UP))
	{
		std::cout << "GUI key UP is down" << std::endl;
	}

	if (gui.keyDown (KEY_DOWN))
	{
		std::cout << "GUI key DOWN is down" << std::endl;
	}

	if (gui.keyDown(KEY_ACTION))
		std::cout << "GUI key ACTION is down" << std::endl;
}
