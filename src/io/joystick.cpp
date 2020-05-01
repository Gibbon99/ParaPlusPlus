#include <io/console.h>
#include "io/joystick.h"

#define AXIS_X 0
#define AXIS_Y 1

int joystickDeadzone = 100;

struct __JOYSTICK_STATE
{
	int joyAxisX;
	int joyAxisY;
};

__JOYSTICK_STATE joystickState;

SDL_Joystick *joy;

//----------------------------------------------------------------------------------------------------------------------
//
// Init a joystick if found
void io_initJoystick ()
//----------------------------------------------------------------------------------------------------------------------
{

// Initialize the joystick subsystem
//	SDL_InitSubSystem (SDL_INIT_JOYSTICK);

// Check for joystick
	if (SDL_NumJoysticks () > 0)
	{
// Open joystick
		joy = SDL_JoystickOpen (0);

		if (joy)
		{
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, "Opened Joystick 0");
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Name: %s\n", SDL_JoystickNameForIndex (0)));
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Number of Axes: %d\n", SDL_JoystickNumAxes (joy)));
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Number of Buttons: %d\n", SDL_JoystickNumButtons (joy)));
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Number of Balls: %d\n", SDL_JoystickNumBalls (joy)));
		}
		else
		{
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Couldn't open Joystick 0\n"));
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Close any open joysticks
void io_closeJoystick()
//----------------------------------------------------------------------------------------------------------------------
{
	// Close if opened
	if (SDL_JoystickGetAttached (joy))
	{
//			SDL_JoystickClose (joy);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Got a joystick movement - map it to a input event and push into input state - called from event pump
//
// X axis usually axis 0, Y axis usually 1
void io_joyMovement (SDL_JoystickID whichJoystick, Uint8 whichAxis, Sint16 axisValue)
//----------------------------------------------------------------------------------------------------------------------
{
	joystickState.joyAxisY = 0;
	joystickState.joyAxisX = 0;

	if (whichAxis == AXIS_X)
	{
		if (axisValue < (0 - joystickDeadzone))  // Move Left
			joystickState.joyAxisX = -1;
		else if (axisValue > (0 + joystickDeadzone))
			joystickState.joyAxisX = 1;
	}

	if (whichAxis == AXIS_Y)
	{
		if (axisValue < (0 - joystickDeadzone))     // Move Up
			joystickState.joyAxisY = -1;
		else if (axisValue > (0 + joystickDeadzone))
			joystickState.joyAxisY = 1;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Map the current joystick values to the input state
void io_mapJoyToInput ()
//----------------------------------------------------------------------------------------------------------------------
{
	if (joystickState.joyAxisX != 0)
	{
		if (joystickState.joyAxisX < 0)
			input.setState (KEY_LEFT, true);

		if (joystickState.joyAxisX > 0)
			input.setState (KEY_RIGHT, true);
	}

	if (joystickState.joyAxisY != 0)
	{
		if (joystickState.joyAxisY < 0)
			input.setState (KEY_UP, true);

		if (joystickState.joyAxisY > 0)
			input.setState (KEY_DOWN, true);
	}

	if (SDL_JoystickGetButton(joy, 0) == 1)
	{
		input.setState(KEY_ACTION, true);
	}
}
