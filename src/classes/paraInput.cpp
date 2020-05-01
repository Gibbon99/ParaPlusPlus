#include <physfs.h>
#include <vector>
#include <iostream>
#include "classes/paraInput.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Pass in string and parameters to format and return a string
// https://stackoverflow.com/questions/19009094/c-variable-arguments-with-stdstring-only
std::string paraInput::int_getString (std::string format, ...)
//----------------------------------------------------------------------------------------------------------------------
{
	const char *const zcFormat = format.c_str ();

	// initialize use of the variable argument array
	va_list vaArgs;
	va_start(vaArgs, format);

	// reliably acquire the size from a copy of the variable argument array
	// and a functionally reliable call to mock the formatting
	va_list vaCopy;
	va_copy(vaCopy, vaArgs);
	const int iLen = std::vsnprintf (nullptr, 0, zcFormat, vaCopy);
	va_end(vaCopy);

	// return a formatted string without risking memory mismanagement and without assuming any compiler
	// or platform specific behavior
	std::vector<char> zc (iLen + 1);
	std::vsnprintf (zc.data (), zc.size (), zcFormat, vaArgs);
	va_end(vaArgs);

	return std::string (zc.data ());
}

//----------------------------------------------------------------------------------------------------------------------
//
// Write the keyboard layout to disk - currently using physfs library - assuming its been started
//
// TODO: read / write to platforms native byte order
void paraInput::save ()
//----------------------------------------------------------------------------------------------------------------------
{
	PHYSFS_file   *fileHandle;
	PHYSFS_sint64 returnCode;

	if (!PHYSFS_isInit ())
	{
		funcOutput (-1, int_getString ("PHYSFS has not been started."));
		return;
	}

	fileHandle = PHYSFS_openWrite (fileName.c_str ());
	if (nullptr == fileHandle)
	{
		funcOutput (-1, int_getString ("Unable to open keybinding file for writing [ %s ] - [ %s ]", fileName.c_str (), PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
		return;
	}

	for (auto keyIndex = 0; keyIndex != KEY_NUMBER_ACTIONS; keyIndex++)
	{
		returnCode = PHYSFS_writeBytes (fileHandle, &keyBinding[keyIndex].keyValue, sizeof (keyBinding[keyIndex].keyValue));
		if (returnCode < (PHYSFS_sint64) sizeof (keyBinding[keyIndex].keyValue))
			funcOutput (-1, int_getString ("Unable to write keybinding file [ %s ] - [ %s ]", fileName.c_str (), PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
	}

	if (PHYSFS_close (fileHandle) == 0)
		funcOutput (-1, int_getString ("Unable to close keybinding file [ %s ] - [ %s ]", fileName.c_str (), PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Read in the keyboard binding file - currently using physfs library - assuming its been started
void paraInput::load ()
//----------------------------------------------------------------------------------------------------------------------
{
	PHYSFS_file   *fileHandle;
	PHYSFS_sint64 returnCode;

	if (!PHYSFS_isInit ())
	{
		funcOutput (-1, int_getString ("PHYSFS has not been started."));
		return;
	}

	fileHandle = PHYSFS_openRead (fileName.c_str ());
	if (nullptr == fileHandle)
	{
		funcOutput (-1, int_getString ("Unable to open keybinding file [ %s ] - [ %s ]", fileName.c_str (), PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
		setDefaultKeybindings ();
		setKeyDescription ();
		return;
	}

	for (auto keyIndex = 0; keyIndex != KEY_NUMBER_ACTIONS; keyIndex++)
	{
		returnCode = PHYSFS_readBytes (fileHandle, &keyBinding[keyIndex].keyValue, sizeof (keyBinding[keyIndex].keyValue));
		if (returnCode < 0)
		{
			funcOutput (-1, int_getString ("Unable to read keybinding file [ %s ] - [ %s ]", fileName.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
			return;
		}
	}

	setKeyDescription ();   // Get text for current language after loading the key values

	if (PHYSFS_close (fileHandle) == 0)
		funcOutput (-1, int_getString ("Unable to close keybinding file [ %s ] - [ %s ]", fileName.c_str (), PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Display the keybinding table
void paraInput::print()
//----------------------------------------------------------------------------------------------------------------------
{
	funcOutput(-1, int_getString("--- Key Binding Table ---"));

	for (auto keyCounter = 0; keyCounter != KEY_NUMBER_ACTIONS; keyCounter++)
	{
		funcOutput(-1, int_getString("Key [ %i ] - [ %s ]", keyBinding[keyCounter].keyValue, keyBinding[keyCounter].text.c_str()));
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Update the state of the keyboard mappings from the system keyboard state
void paraInput::update ()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto keyCounter = 0; keyCounter != KEY_NUMBER_ACTIONS; keyCounter++)
	{
		keyboardState[keyBinding[keyCounter].keyValue] ? keyBinding[keyCounter].currentlyPressed = true : keyBinding[keyCounter].currentlyPressed = false;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the state of a key based on PARA_KEY
bool paraInput::keyDown (int whichKey)
//----------------------------------------------------------------------------------------------------------------------
{
	if ((whichKey < KEY_FIRST) || (whichKey > KEY_NUMBER_ACTIONS))
	{
		funcOutput (-1, "Invalid key index requesting key state.");
		return false;
	}

	return keyBinding[whichKey].currentlyPressed;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the state of a key based on PARA_Key index. This will be overwritten next frame when the array is updated
void paraInput::setState (int whichKey, bool newState)
//----------------------------------------------------------------------------------------------------------------------
{
	if ((whichKey < KEY_FIRST) || (whichKey > KEY_NUMBER_ACTIONS))
	{
		funcOutput (-1, "Invalid key index requesting key state.");
		return;
	}

	keyBinding[whichKey].currentlyPressed = newState;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Init the input class
void paraInput::init (std::string newFileName, functionPtrOut outputFuncPtr, funcStrIn getStringFunc)
//----------------------------------------------------------------------------------------------------------------------
{
	funcOutput    = outputFuncPtr;
	funcGetString = getStringFunc;
	fileName      = newFileName;
	load ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Assign the text description for each key
void paraInput::setKeyDescription ()
//----------------------------------------------------------------------------------------------------------------------
{
	keyBinding[KEY_LEFT].text       = funcGetString ("gameLeft");
	keyBinding[KEY_RIGHT].text      = funcGetString ("gameRight");
	keyBinding[KEY_DOWN].text       = funcGetString ("gameDown");
	keyBinding[KEY_UP].text         = funcGetString ("gameUp");
	keyBinding[KEY_PAUSE].text      = funcGetString ("gamePause");
	keyBinding[KEY_ACTION].text     = funcGetString ("gameAction");
	keyBinding[KEY_ESCAPE].text     = funcGetString ("gameEscape");
	keyBinding[KEY_CONSOLE].text    = funcGetString ("consoleAction");
	keyBinding[KEY_SCREENSHOT].text = funcGetString ("gameScreenShot");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the default values for keybindings
void paraInput::setDefaultKeybindings ()
//----------------------------------------------------------------------------------------------------------------------
{
	keyBinding[KEY_LEFT].keyValue       = SDL_SCANCODE_LEFT;
	keyBinding[KEY_RIGHT].keyValue      = SDL_SCANCODE_RIGHT;
	keyBinding[KEY_DOWN].keyValue       = SDL_SCANCODE_DOWN;
	keyBinding[KEY_UP].keyValue         = SDL_SCANCODE_UP;
	keyBinding[KEY_PAUSE].keyValue      = SDL_SCANCODE_P;
	keyBinding[KEY_ACTION].keyValue     = SDL_SCANCODE_LCTRL;
	keyBinding[KEY_ESCAPE].keyValue     = SDL_SCANCODE_ESCAPE;
	keyBinding[KEY_CONSOLE].keyValue    = SDL_SCANCODE_GRAVE;
	keyBinding[KEY_SCREENSHOT].keyValue = SDL_SCANCODE_S;
}
