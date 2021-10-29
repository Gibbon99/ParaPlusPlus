#include <queue>
#include <string>
#include "classes/paraBullet.h"
#include "game/hud.h"
#include "game/score.h"
#include "game/texture.h"
#include "game/shipDecks.h"
#include "game/particles.h"
#include "game/lightMaps.h"
#include "game/transfer.h"
#include "game/alertLevel.h"
#include "gui/guiWonScreen.h"
#include "gui/guiDeckView.h"
#include "io/logFile.h"
#include "system/gameEvents.h"
#include "system/util.h"

std::queue<paraEventGame *> gameEvents;

//----------------------------------------------------------------------------------------------------------------------
//
// Add a new event to the game queue - only added when mutex is free. ie: Thread is not accessing the queue
void gam_addEvent(int newAction, int newCounter, const string &newLine)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_Mutex    *tempMutex;
	paraEventGame *tempEventGame;
	std::string   newLineCopy;
	std::string   stringText1;
	std::string   stringText2;
	std::string   stringText3;
	std::string   delimiter = "|";
	size_t        position;

	newLineCopy = newLine;
	//
	// Split the passed in string into the parts needed for a Game Event
	position    = newLineCopy.find (delimiter);
	stringText1 = newLineCopy.substr (0, position);
	newLineCopy.erase (0, position + delimiter.length ());

	position    = newLineCopy.find (delimiter);
	stringText2 = newLineCopy.substr (0, position);
	newLineCopy.erase (0, position + delimiter.length ());

	stringText3 = newLineCopy; //.substr (position + delimiter.length (), newLine.length ());

	tempEventGame = new paraEventGame (newAction, newCounter, stringText1, stringText2, stringText3);

	tempMutex = evt_getMutex (GAME_MUTEX_NAME);
	if (nullptr == tempMutex)
		sys_shutdownWithError (sys_getString ("Unable to get mutex details [ %s ] [ %s ]", GAME_MUTEX_NAME, SDL_GetError ()));

	if (PARA_LockMutex (tempMutex) == 0)
	{
		gameEvents.push (tempEventGame);
		PARA_UnlockMutex (tempMutex);
	}
	else
		log_addEvent (sys_getString ("Unable to lock mutex [ %s ] [ %s ]", GAME_MUTEX_NAME, SDL_GetError ()));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Returns the current size of the game event queue
int gam_gameEventQueueSize()
//----------------------------------------------------------------------------------------------------------------------
{
	return gameEvents.size ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Run an event queue on the same thread as the graphics drawing thread
void gam_processGameEventQueue()
//----------------------------------------------------------------------------------------------------------------------
{
	cpVect            tempVect;
	paraEventGame     *tempEvent;
	static PARA_Mutex *gameMutex = nullptr;

	//
	// Don't process any game events until fade is finished
	if (renderer.currentFadeState != FADE_STATE_NONE)
		return;

	while (!gameEvents.empty ())     // events in the queue to process
	{

#ifdef MY_DEBUG
		//		if (gameEvents.size() > 60)
		//			sys_shutdownWithError(sys_getString("Too many events on the game event queue [ %i ].", gameEvents.size()));
#endif
		if (nullptr == gameMutex)
		{
			gameMutex = evt_getMutex (GAME_MUTEX_NAME);    // cache the mutex value
			if (nullptr == gameMutex)
				sys_shutdownWithError (sys_getString ("Unable to get Mutex value [ %s ] - [ %s ]", GAME_MUTEX_NAME, SDL_GetError ()));
		}
		PARA_LockMutex (gameMutex);
		tempEvent = gameEvents.front ();
		PARA_UnlockMutex (gameMutex);

		if (tempEvent->counter > 0)  // If not 0 - re-add to the queue with the reduced count
		{
			tempEvent->counter--;
			gam_addEvent (tempEvent->action, tempEvent->counter, tempEvent->gameText1 + "|" + tempEvent->gameText2 + "|" + tempEvent->gameText3);

			PARA_LockMutex (gameMutex);           // Blocks if the mutex is locked by another thread
			delete (gameEvents.front ());         // Free memory
			gameEvents.pop ();
			PARA_UnlockMutex (gameMutex);

			return;
		}
		else
		{
			switch (tempEvent->action)
			{
				case EVENT_ACTION_GAME_USE_NEW_RENDERER:
					renderer.useNewRenderer (std::stoi (tempEvent->gameText1));
					break;

				case EVENT_ACTION_GAME_SCRIPT_RESTART:
					paraScriptInstance.restart ();
					gui.restart ();
					paraScriptInstance.run ("as_createGUI", "");
					break;

				case EVENT_ACTION_GAME_LOAD_TEXTURE:
					gam_loadTexture (tempEvent->gameText1, tempEvent->gameText2);
					break;

				case EVENT_ACTION_GAME_LOAD_DECK:
					gam_loadShipDeck (tempEvent->gameText1);
					break;

				case EVENT_ACTION_GAME_LOAD_FONT:
					fontClass.load (sys_convertToInt (tempEvent->gameText3), tempEvent->gameText2, tempEvent->gameText1);
					break;

				case EVENT_ACTION_GAME_CHANGE_MODE:
					gam_setHudText ("hudMainMenu");
					sys_setNewMode (sys_convertToInt (tempEvent->gameText1), static_cast<bool>(sys_convertToInt (tempEvent->gameText2)));
					break;

				case EVENT_ACTION_DAMAGE_TO_DROID:
					gam_damageToDroid (sys_convertToInt (tempEvent->gameText1), sys_convertToInt (tempEvent->gameText2), sys_convertToInt (tempEvent->gameText3));
					break;

				case EVENT_ACTION_DROID_COLLISION:
					gam_processCollision (sys_convertToInt (tempEvent->gameText1));
					break;

				case EVENT_ACTION_CREATE_BULLET:
//					gam_addBullet (sys_convertToInt (tempEvent->gameText1));
					break;

				case EVENT_ACTION_REMOVE_BULLET:
					gam_removeBullet (sys_convertToInt (tempEvent->gameText1));
					break;

				case EVENT_ACTION_ADD_EMITTER:
					tempVect.x = static_cast<cpFloat>(sys_convertToInt (tempEvent->gameText1));
					tempVect.y = static_cast<cpFloat>(sys_convertToInt (tempEvent->gameText2));
					gam_addEmitter (tempVect, sys_convertToInt (tempEvent->gameText3), 0);
					break;

				case EVENT_ACTION_ADD_LIGHTMAP:
					tempVect.x = static_cast<cpFloat>(sys_convertToInt (tempEvent->gameText1));
					tempVect.y = static_cast<cpFloat>(sys_convertToInt (tempEvent->gameText2));
					gam_addNewLightmap (tempVect, sys_convertToInt (tempEvent->gameText3), 0);
					break;

				case EVENT_ACTION_INIT_TRANSFER_MODE:
					trn_initTransferValues (sys_convertToInt (tempEvent->gameText1));
					break;

				case EVENT_ACTION_INIT_TRANSFER_TWO:
					trn_initTransferScreenTwo ();
					break;

				case EVENT_ACTION_GAME_CHECK_DECK_CLEAR:
					gam_checkAllLevels ();
					break;

				case EVENT_ACTION_AUDIO_START_BACKGROUND:
					gam_startAlertLevelSound (gam_getCurrentAlertLevel ());
					break;

				case EVENT_ACTION_GAME_WON:
					gui_prepareWonScreen ();
					break;

				case EVENT_ACTION_GAME_OVER:
//					gam_processGameOver();
					break;

				case MODE_END_PRE_LOST_SCREEN:
					sys_setNewMode (MODE_END_LOST_SCREEN, true);
					break;

				case EVENT_ACTION_END_LOST_SCREEN:
					audio.stopAllChannels ();
					gam_decideScoreAction ();
					break;

				case EVENT_ACTION_STOP_BLINK_TIMER:
					gui_stopBlinkTimer ();
					break;

				case EVENT_ACTION_START_BLINK_TIMER:
					gui_startBlinkTimer (500);
					break;

				case EVENT_ACTION_START_BACKGROUND_SOUND:
					gam_startAlertLevelSound (gam_getCurrentAlertLevel ());
					break;
			}

			PARA_LockMutex (gameMutex);           // Blocks if the mutex is locked by another thread
			delete (gameEvents.front ());         // Free memory
			gameEvents.pop ();
			PARA_UnlockMutex (gameMutex);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Changing decks, to clear all the events from previous deck
void gam_clearGameEvents()
//----------------------------------------------------------------------------------------------------------------------
{
	static PARA_Mutex *gameMutex = nullptr;

	if (nullptr == gameMutex)
	{
		gameMutex = evt_getMutex (GAME_MUTEX_NAME);    // cache the mutex value
		if (nullptr == gameMutex)
			sys_shutdownWithError (sys_getString ("Unable to get Mutex value [ %s ] - [ %s ]", GAME_MUTEX_NAME, SDL_GetError ()));
	}

	PARA_LockMutex (gameMutex);
	while (!gameEvents.empty ())
	{
		delete (gameEvents.front ());         // Free memory
		gameEvents.pop ();
	}
	PARA_UnlockMutex (gameMutex);
}