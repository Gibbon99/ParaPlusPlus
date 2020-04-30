//
// Created by dberry on 26/4/20.
//

#include <queue>
#include "../../hdr/system/gameEvents.h"
#include "../../hdr/classes/paraEvent.h"

std::queue<paraEventGame *> gameEvents;

//----------------------------------------------------------------------------------------------------------------------
//
// Add a new event to the game queue - only added when mutex is free. ie: Thread is not accessing the queue
void gam_addEvent (int newAction, int newCounter, const std::string &newLine)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_Mutex    *tempMutex;
	paraEventGame *tempEventGame;

	tempEventGame = new paraEventGame (newAction, newCounter, newLine);

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
// Run an event queue on the same thread as the graphics drawing thread
void gam_processGameEventQueue ()
//----------------------------------------------------------------------------------------------------------------------
{
	paraEventGame     *tempEvent;
	static PARA_Mutex *gameMutex = nullptr;

	{
		if (!gameEvents.empty ())     // events in the queue to process
		{
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
				gam_addEvent (tempEvent->action, tempEvent->counter, tempEvent->gameText);

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
						renderer.useNewRenderer (std::stoi (tempEvent->gameText));
						break;

					case EVENT_ACTION_GAME_SCRIPT_RESTART:
						paraScriptInstance.restart();
						gui.restart();
						paraScriptInstance.run ("as_createGUI", "");
						break;
				}

				PARA_LockMutex (gameMutex);           // Blocks if the mutex is locked by another thread
				delete (gameEvents.front ());         // Free memory
				gameEvents.pop ();
				PARA_UnlockMutex (gameMutex);
			}
		}
	}
}


