#ifndef PARA_PARAINPUT_H
#define PARA_PARAINPUT_H

#include <string>
#include <wrapper.h>
#include "../../data/scripts/enum.h"

typedef void        (*functionPtrOut) (int, std::string);

typedef std::string (*funcStrIn) (std::string);     // Function to provide the key descriptions

struct __KeyBindings
{
	std::string text;
	PARA_Scancode keyValue;
	bool        currentlyPressed;
};

//----------------------------------------------------------------------------------------------------------------------
//
// Central class to handle all the input devices and consolidate into one state
class paraInput
//----------------------------------------------------------------------------------------------------------------------
{
public:

	void init (std::string newFileName, functionPtrOut outputFuncPtr, funcStrIn getStringFunc);

	void setDefaultKeybindings ();

	void setKeyDescription ();

	void load ();

	void save ();

	void update ();

	void print();

	bool keyDown (int whichKey);

	void setState (int whichKey, bool newState);

	std::string int_getString (std::string format, ...);

	const Uint8 *keyboardState;

private:
	__KeyBindings  keyBinding[KEY_NUMBER_ACTIONS];
	std::string    fileName;
	functionPtrOut funcOutput;
	funcStrIn      funcGetString;

};

#endif //PARA_PARAINPUT_H