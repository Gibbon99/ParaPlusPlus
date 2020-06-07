#include <queue>
#include <string>
#include <game/texture.h>
#include <game/shipDecks.h>
#include <system/util.h>
#include <game/bullet.h>
#include <game/particles.h>
#include <game/lightMaps.h>
#include <game/transfer.h>
#include "../../hdr/system/gameEvents.h"
#include "../../hdr/classes/paraEvent.h"

std::queue<paraEventGame *> gameEvents;

//----------------------------------------------------------------------------------------------------------------------
//
// Add a new event to the game queue - only added when mutex is free. ie: Thread is not accessing the queue
void gam_addEvent (int newAction, int newCounter, const string &newLine)
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
		logFile.write (sys_getString ("Unable to lock mutex [ %s ] [ %s ]", GAME_MUTEX_NAME, SDL_GetError ()));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Returns the current size of the game event queue
int gam_gameEventQueueSize()
//----------------------------------------------------------------------------------------------------------------------
{
	return gameEvents.size();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Run an event queue on the same thread as the graphics drawing thread
void gam_processGameEventQueue ()
//----------------------------------------------------------------------------------------------------------------------
{
	paraEventGame     *tempEvent;
	static PARA_Mutex *gameMutex = nullptr;

	while (!gameEvents.empty ())     // events in the queue to process
//	if (!gameEvents.empty ())     // events in the queue to process
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
			gam_addEvent (tempEvent->action, tempEvent->counter, tempEvent->gameText1+"|"+tempEvent->gameText2+"|"+tempEvent->gameText3);

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

				case EVENT_ACTION_GAME_LOAD_MAP:
					gam_createCollisionMap (tempEvent->gameText1);
					break;

				case EVENT_ACTION_GAME_LOAD_FONT:
					fontClass.load (sys_convertToInt (tempEvent->gameText3), tempEvent->gameText2, tempEvent->gameText1);
					break;

				case EVENT_ACTION_GAME_CHANGE_MODE:
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
					gam_removeBullet(sys_convertToInt(tempEvent->gameText1));
					break;

				case EVENT_ACTION_ADD_EMITTER:
					gam_addEmitter(b2Vec2(sys_convertToInt(tempEvent->gameText1), sys_convertToInt(tempEvent->gameText2)), sys_convertToInt(tempEvent->gameText3), 0);
					break;

				case EVENT_ACTION_ADD_LIGHTMAP:
					gam_addNewLightmap(b2Vec2(sys_convertToInt(tempEvent->gameText1), sys_convertToInt(tempEvent->gameText2)), sys_convertToInt(tempEvent->gameText3), 0);
					break;

				case EVENT_ACTION_GO_TRANSFER_MODE:
					trn_initTransferValues(sys_convertToInt(tempEvent->gameText1));
					break;

				case EVENT_ACTION_GO_TRANSFER_TWO:
					trn_initTransferScreenTwo();
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
	while (!gameEvents.empty())
	{
		delete (gameEvents.front ());         // Free memory
		gameEvents.pop ();
	}
	PARA_UnlockMutex (gameMutex);
}