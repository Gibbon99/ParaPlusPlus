
#include <game/droids.h>
#include <game/bullet.h>
#include <game/particles.h>
#include <game/lightMaps.h>
#include <game/transfer.h>
#include <game/pauseMode.h>
#include <gui/guiLostScreen.h>
#include <game/game.h>
#include <gui/guiHighScore.h>
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
#include "game/score.h"
#include "game/transferGame.h"

SDL_Event evt;

//----------------------------------------------------------------------------------------------------------------------
//
// Handle system events and populate the keyboard state array
void sys_processInputEvents ()
//----------------------------------------------------------------------------------------------------------------------
{
	while (SDL_PollEvent (&evt) != 0)
	{
		//
		// Check for OS Windowing events
		if (evt.type == SDL_WINDOWEVENT)
		{
			switch (evt.window.event)
			{
				case SDL_RENDER_DEVICE_RESET:
				case SDL_RENDER_TARGETS_RESET:
					// Reload all textures - UNTESTED - Seems to reload on PC ok.
					paraScriptInstance.run ("as_loadTextureResources", "");
					break;

				case SDL_WINDOWEVENT_FOCUS_LOST:
					gam_changePauseMode (MODE_GAME_PAUSE_ON);
					break;

				case SDL_WINDOWEVENT_FOCUS_GAINED:
					break;

				case SDL_WINDOWEVENT_SHOWN:
					break;

				case SDL_WINDOWEVENT_CLOSE:
					quitLoop = true;
					break;
			}
		}

		switch (evt.type)
		{
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
				if (currentMode == MODE_SHOW_SPLASH)
				{
					sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_CHANGE_MODE, 0, to_string(MODE_GUI_MAINMENU)+"|"+to_string(true));
					return;
				}

				if (currentMode == MODE_GUI_KEYCODE_ENTRY)
				{
					io_setNewKeycodeValue(evt.key.keysym.scancode);
					return;
				}

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

				if (evt.key.keysym.sym == SDLK_F3)
					gui_showHighscoreEntry();

				if (evt.key.keysym.sym == SDLK_F5)
				{
					sys_setNewMode (MODE_GUI_WON_SCREEN, true);
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
				{
					sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_CHAR, 0, evt.text.text);
				}
				break;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Run a frame once
void sys_gameTickRun ()
//----------------------------------------------------------------------------------------------------------------------
{
	if (renderer.currentFadeState != FADE_STATE_NONE)
		renderer.updateFade ();

	io_checkFileWatcher ();

	if (currentMode == MODE_GUI_KEYCODE_ENTRY)
	{
		sys_processInputEvents ();
		return;
	}

	sys_processInputEvents ();

	gam_processGameEventQueue();

	io_processKeyboardState ();

	switch (currentMode)
	{
		case MODE_PRE_GAME:
			gam_startNewGame ();
			sys_setNewMode (MODE_GAME, true);
			break;

		case MODE_GAME:
			if (gui.getCurrentDialogbox() != NO_DIALOG_BOX)
			{
				io_mapMouseToInput ();
				gui.processGuiInput();
				return;
			}

			if (!gam_pauseModeOn())
			{
				sys_processPhysics (TICKS_PER_SECOND);
				gam_animateHealing ();
				playerDroid.sprite.animate ();
				gam_animateDroids();
				gam_doorCheckTriggerAreas ();
				gam_doorProcessActions ();
				gam_animateParticles();
				gam_removeDeadEmitters();
				gam_animateLightmaps();
				gam_processBullets ();

				gam_populateInfluenceMap(playerDroid.worldPosInPixels);

				gam_processAI();

				gam_removeDroids (false);
				gam_processScore();
				gam_processHealingTile();
				gam_processInfluenceTime ();

				gam_checkPlayerHealth ();
				gam_weaponRechargePlayer ();

				debug_getNumberOfShapes();
			}
			else
			{
				gam_animateDroids();
				gam_animateLightmaps();
				gam_animateHealing ();
				playerDroid.sprite.animate ();
			}

			break;

		case MODE_GUI_DECKVIEW:
			gam_animateHealing ();
			break;

		case MODE_GUI_SHIPVIEW:
		case MODE_GUI_LIFTVIEW:
			gui_animateStarfield ();
			break;

		case MODE_GUI_INTROSCROLL:
			gui_scrollScrollBox ("introScreen.scrollbox");
			break;

		case MODE_GUI_WON_SCREEN:
			gui_scrollScrollBox ("wonScreen.scrollbox");
			break;

		case MODE_GUI_TERMINAL:
			break;

		case MODE_GUI_DATABASE:
			gui_scrollScrollBox ("databaseScreen.scrollbox");
			databaseSprite.animate ();
			break;

		case MODE_TRANSFER_PRE_SCREEN_TWO:
			gam_addEvent (EVENT_ACTION_INIT_TRANSFER_TWO, 0, "");
			break;

		case MODE_TRANSFER_SCREEN_ONE:
		case MODE_TRANSFER_SCREEN_TWO:
//			trn_checkTransferScreenSounds();
			break;

		case MODE_PRE_TRANSFER_CHOOSE_SIDE:
			trn_prepareTransferCountDown ();
			break;

		case MODE_TRANSFER_CHOOSE_SIDE:
			trn_processTransferCountDown();
			break;

		case MODE_PRE_TRANSFER_GAME:
			trn_prepareTransferGame();
			break;

		case MODE_TRANSFER_GAME:
			trn_processTransferGame();
			break;

		case MODE_TRANSFER_DEADLOCK:
			trn_processDeadlock();
			break;

		case MODE_END_PRE_LOST_SCREEN:
			gui_preRenderLostScreen();
			break;

		case MODE_END_LOST_SCREEN:
			gui_processLostScreen();
			break;

		case MODE_GAME_OVER:
			gam_processGameOver();
			break;

		default:
//			sys_shutdownWithError(sys_getString("Fatal error: Unknown game mode. %i", currentMode));
			break;
	}
}