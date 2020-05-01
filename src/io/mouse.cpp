#include "io/mouse.h"

//
// This maps the mouse functions to the keybinding actions stored in the input class.
//
// Mousewheel emulates up and down buttons, and the left mouse button emulates the action key
//
// Mapping is done after the keyboard state is run, and only true values are written so that the key states
// from the keyboard are not overwritten

struct __MOUSE_STATE
{
	int  posX;
	int  posY;
	int  wheelDirection;
	bool mouseButtonDown;
	bool mouseWheelUsed;
};

__MOUSE_STATE mouseState;

//----------------------------------------------------------------------------------------------------------------------
//
// Handle a mouse motion event - also has button state for putting into input state for action key
void io_mouseMovement (Sint32 newPosX, Sint32 newPosY, Sint32 buttonState)
//----------------------------------------------------------------------------------------------------------------------
{
	mouseState.posX = newPosX;
	mouseState.posY = newPosY;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Mouse scroll wheel was used - emulate the up and down keys
void io_mouseWheel (int whichKey)
//----------------------------------------------------------------------------------------------------------------------
{
	mouseState.mouseWheelUsed = true;
	mouseState.wheelDirection = whichKey;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Getting the mouse button state is too fast using mouse motion event
void io_mouseButtonDown ()
//----------------------------------------------------------------------------------------------------------------------
{
	mouseState.mouseButtonDown = true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Map the mouse state into the input state - overriding keyboard and joystick events
void io_mapMouseToInput ()
//----------------------------------------------------------------------------------------------------------------------
{
	gui.setMouse (mouseState.posX, mouseState.posY);
	if (mouseState.mouseWheelUsed)
		gui.setState (mouseState.wheelDirection, true, KEY_ACTION_NO_SOURCE);

	if (mouseState.mouseButtonDown)
		gui.setState (KEY_ACTION, true, KEY_ACTION_MOUSE);

	mouseState.mouseWheelUsed  = false;
	mouseState.mouseButtonDown = false;
}