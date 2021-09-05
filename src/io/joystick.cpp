#include <io/console.h>
#include <system/util.h>
#include "io/joystick.h"

#define AXIS_X 0
#define AXIS_Y 1

int joystickDeadzone = 400;

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
	//
	// Check for joystick
	if (SDL_NumJoysticks () > 0)
	{
		//
		// Open joystick - currently only the first one
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
void io_closeJoystick ()
//----------------------------------------------------------------------------------------------------------------------
{
	if (SDL_JoystickGetAttached (joy))
	{
		SDL_JoystickClose (joy);
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
	static float joystickRateLimit{1.0f};

	joystickState.joyAxisY = 0;
	joystickState.joyAxisX = 0;

	joystickRateLimit -= 1.0f * 0.1f;
	if (joystickRateLimit < 0.0f)
	{
		if (axisValue == 0)
			return;

		printf ("Axis value [ %i ]\n", axisValue);

		if (whichAxis == AXIS_X)
		{
			joystickRateLimit = 1.0f;

			if (axisValue < (0 - joystickDeadzone))  // Move Left
				joystickState.joyAxisX = -1;
			else if (axisValue > (0 + joystickDeadzone))
				joystickState.joyAxisX = 1;
		}

		if (whichAxis == AXIS_Y)
		{
			joystickRateLimit = 1.0f;

			if (axisValue < (0 - joystickDeadzone))     // Move Up
				joystickState.joyAxisY = -1;
			else if (axisValue > (0 + joystickDeadzone))
				joystickState.joyAxisY = 1;
		}
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
			gui.setState (KEY_LEFT, true, KEY_ACTION_NO_SOURCE);

		if (joystickState.joyAxisX > 0)
			gui.setState (KEY_RIGHT, true, KEY_ACTION_NO_SOURCE);
	}

	if (joystickState.joyAxisY != 0)
	{
		if (joystickState.joyAxisY < 0)
			gui.setState (KEY_UP, true, KEY_ACTION_NO_SOURCE);

		if (joystickState.joyAxisY > 0)
			gui.setState (KEY_DOWN, true, KEY_ACTION_NO_SOURCE);
	}

	if (SDL_JoystickGetButton (joy, 0) == 1)     // Only update if actually down
		gui.setState (KEY_ACTION, true, KEY_ACTION_JOYSTICK);
}
