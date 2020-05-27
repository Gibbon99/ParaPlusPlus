#include <game/pathFind.h>
#include <game/droids.h>
#include <game/bullet.h>
#include "io/fileWatch.h"
#include "io/keyboard.h"
#include "io/joystick.h"
#include "io/mouse.h"
#include "gui/guiSideview.h"
#include "gui/guiScrollbox.h"
#include "game/shipDecks.h"
#include "game/healing.h"
#include "game/player.h"
#include "system/util.h"
#include "system/frameUpdate.h"
#include "game/doors.h"

SDL_Event evt;

//----------------------------------------------------------------------------------------------------------------------
//
// Handle system events and populate the keyboard state array
void sys_processInputEvents ()
//----------------------------------------------------------------------------------------------------------------------
{
	while (SDL_PollEvent (&evt) != 0)
	{
		switch (evt.type)
		{
			case SDL_RENDER_DEVICE_RESET:
			case SDL_RENDER_TARGETS_RESET:
				// Reload all textures
				break;

			case SDL_WINDOWEVENT_FOCUS_LOST:
				break;

			case SDL_WINDOWEVENT_FOCUS_GAINED:
				break;

			case SDL_WINDOWEVENT_SHOWN:
				break;

			case SDL_WINDOWEVENT_CLOSE:
			case SDL_QUIT:
				quitLoop = true;
				break;

			case SDL_JOYAXISMOTION:
				io_joyMovement (evt.jaxis.which, evt.jaxis.axis, evt.jaxis.value);
				break;

			case SDL_MOUSEMOTION:
				io_mouseMovement (evt.motion.x, evt.motion.y, evt.motion.state);
				break;

			case SDL_MOUSEWHEEL:
				if (evt.wheel.y > 0)    // Scroll Up
					io_mouseWheel (KEY_UP);

				if (evt.wheel.y < 0)    // Scroll down
					io_mouseWheel (KEY_DOWN);
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (evt.button.button == SDL_BUTTON_LEFT)
					io_mouseButtonDown ();
				break;

			case SDL_KEYDOWN:
				if (currentMode == MODE_CONSOLE_EDIT)
				{
					if (evt.key.keysym.sym == SDLK_ESCAPE)
						quitLoop = true;

					if (evt.key.keysym.sym == SDLK_BACKSPACE)
						sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_DELETE_CHAR, 0, "");

					if (evt.key.keysym.sym == SDLK_RETURN)
						sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, 0, "");

					if (evt.key.keysym.sym == SDLK_UP)
						console.userBufferNext ();

					if (evt.key.keysym.sym == SDLK_DOWN)
						console.userBufferPrevious ();

					if (evt.key.keysym.sym == SDLK_TAB)
						console.tabCompletion ();

					if (evt.key.keysym.sym == SDLK_PAGEUP)
						console.changeScrollBackOffset (-1);

					if (evt.key.keysym.sym == SDLK_PAGEDOWN)
						console.changeScrollBackOffset (1);
				}

				if (evt.key.keysym.sym == SDLK_F1)
					sys_setNewMode (MODE_SHOW_SPLASH, true);

				if (evt.key.keysym.sym == SDLK_F2)
				{
					sys_setNewMode (MODE_GUI_MAINMENU, true);
					gui.setCurrentScreen (gui.getIndex (GUI_OBJECT_SCREEN, "mainMenu"));
					gui.setActiveObject (gui.getCurrentScreen (), GUI_OBJECT_BUTTON, "mainMenu.startGameButton");
				}

				if (evt.key.keysym.sym == SDLK_F3)
					sys_setNewMode (MODE_CONSOLE_EDIT, true);

				if (evt.key.keysym.sym == SDLK_F4)
					sys_setNewMode (MODE_GUI_SHIPVIEW, true);

				if (evt.key.keysym.sym == SDLK_F6)
				{
					gam_changeToDeck ("Staterooms", 0);
				}

				if (evt.key.state == SDL_PRESSED)
				{
					io_setKeyboardState (evt.key.keysym.scancode, SDL_PRESSED, evt.key.repeat);
				}

				break;

			case SDL_KEYUP:
				if (evt.key.state == SDL_RELEASED)
				{
					io_setKeyboardState (evt.key.keysym.scancode, SDL_RELEASED, evt.key.repeat);
				}
				break;

			case SDL_TEXTINPUT:
				if (currentMode == MODE_CONSOLE_EDIT)
					sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_CHAR, 0, evt.text.text);
				break;
		}
	}
	io_processKeyboardState ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Run a frame once
void sys_gameTickRun ()
//----------------------------------------------------------------------------------------------------------------------
{
	gam_processGameEventQueue ();
	if (renderer.currentFadeState != FADE_STATE_NONE)
		renderer.updateFade ();

	io_checkFileWatcher ();

	sys_processInputEvents ();

	switch (currentMode)
	{
		case MODE_PRE_GAME:
			sys_setNewMode (MODE_GAME, true);
			break;

		case MODE_GAME:
			sys_processPhysics (TICKS_PER_SECOND);
			gam_animateHealing (gam_getCurrentDeckName ());
			playerDroid.sprite.animate ();
			gam_weaponRechargePlayer ();
			gam_animateDroids(gam_getCurrentDeckName());
			gam_doorCheckTriggerAreas ();
			gam_doorProcessActions ();
			gam_processBullets ();

			gam_populateInfluenceMap(playerDroid.worldPosInPixels);

			gam_processAI(gam_getCurrentDeckName());

//			gam_moveTestCircle ();

			break;

		case MODE_GUI_DECKVIEW:
			gam_animateHealing (gam_getCurrentDeckName ());
			break;

		case MODE_GUI_SHIPVIEW:
		case MODE_GUI_LIFTVIEW:
			gui_animateStarfield ();
			break;

		case MODE_GUI_INTROSCROLL:
			gui_scrollScrollBox ("introScreen.scrollbox");
			break;

		case MODE_GUI_TERMINAL:
			break;

		case MODE_GUI_DATABASE:
			gui_scrollScrollBox ("databaseScreen.scrollbox");
			databaseSprite.animate ();
			break;
	}
}