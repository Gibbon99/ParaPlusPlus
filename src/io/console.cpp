#include <queue>
#include "../../hdr/io/console.h"
#include "../../hdr/system/startup.h"
#include "../../hdr/classes/paraEvent.h"

std::queue<paraEventConsole *> consoleEvents;

//----------------------------------------------------------------------------------------------------------------------
//
// Process the contents of the console queue - run by detached thread
void con_processConsoleEventQueue(void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	paraEventConsole  *tempEvent;
	static PARA_Mutex *consoleMutex = nullptr;

	while (runThreads)
	{
		evt_setThreadReady(CONSOLE_THREAD_NAME);

		if (evt_shouldThreadRun(CONSOLE_THREAD_NAME))
		{
			PARA_Delay(THREAD_DELAY_MS);

			if (!consoleEvents.empty())     // events in the queue to process
			{
				if (nullptr == consoleMutex)
				{
					consoleMutex = evt_getMutex(CONSOLE_MUTEX_NAME);    // cache the mutex value
					if (nullptr == consoleMutex)
						sys_shutdownWithError(
								sys_getString("Unable to get Mutex value [ %s ] - [ %s ]", CONSOLE_MUTEX_NAME, SDL_GetError()));
				}
				PARA_LockMutex(consoleMutex);
					tempEvent = consoleEvents.front();
				PARA_UnlockMutex(consoleMutex);

				if (tempEvent->counter > 0)  // If not 0 - re-add to the queue with the reduced count
				{
					tempEvent->counter--;
					con_addEvent(tempEvent->counter, tempEvent->newConsoleLine);
				}
				else
				{
					switch (tempEvent->action)
					{
						case EVENT_ACTION_CONSOLE_WRITE:
							console.add(tempEvent->newConsoleLine);
							break;
					}

					PARA_LockMutex(consoleMutex);   // Blocks if the mutex is locked by another thread
						delete (consoleEvents.front());     // Free memory
						consoleEvents.pop();
					PARA_UnlockMutex(consoleMutex);

				}
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Add a new event to the console queue - only added when mutex is free. ie: Thread is not accessing the queue
void con_addEvent(int newCounter, const std::string &newLine)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_Mutex *tempMutex;

	tempMutex = evt_getMutex(CONSOLE_MUTEX_NAME);
	if (nullptr == tempMutex)
		sys_shutdownWithError(sys_getString("Unable to get mutex details [ %s ] [ %s ]", CONSOLE_MUTEX_NAME, SDL_GetError()));

	if (PARA_LockMutex(tempMutex) == 0)
	{
		consoleEvents.push(new paraEventConsole(EVENT_ACTION_CONSOLE_WRITE, newLine));
		PARA_UnlockMutex(tempMutex);
	}
	else
		logFile.write(sys_getString("Unable to lock mutex [ %s ] [ %s ]", CONSOLE_MUTEX_NAME, SDL_GetError()));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Start the console processing queue and thread
void con_initConsole()
//----------------------------------------------------------------------------------------------------------------------
{
	//
	// Start the console and processing thread
	evt_registerMutex(CONSOLE_MUTEX_NAME);
	evt_registerThread(reinterpret_cast<SDL_ThreadFunction>(con_processConsoleEventQueue), CONSOLE_THREAD_NAME);
	evt_setThreadState(true, CONSOLE_THREAD_NAME);

	while (!evt_isThreadReady(CONSOLE_THREAD_NAME))
	{
#ifdef MY_DEBUG
		cout << "Waiting for console thread to start..." << endl;
#endif
	}// Wait for thread to be ready to use

	sys_addEvent(EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_WRITE, 0, sys_getString("Console started [ %s ]", APP_NAME));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render the console to the screen
void con_renderConsole()
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_Surface *tempSurface = nullptr;
	PARA_Texture *tempTexture = nullptr;

	console.prepare(logicalWinHeight, consoleFont.lineHeight);
	for (; console.consoleItr != console.consoleText.rend(); ++console.consoleItr)
	{
		tempSurface = consoleFont.write(console.posX, console.posY, *console.consoleItr);  // Surface is freed within console class
		tempTexture = SDL_CreateTextureFromSurface(sys_getRenderer(), tempSurface);

		SDL_RenderCopy(sys_getRenderer(), tempTexture, nullptr, &consoleFont.pos);

		SDL_DestroyTexture(tempTexture);

		console.posY -= consoleFont.lineHeight;
	}

	tempSurface = consoleFont.write(1, 10,
	                                sys_getString("intoNextFrame : %f Think : %i FPS : %i", percentIntoNextFrame, thinkFPSPrint, fpsPrint));
	tempTexture = SDL_CreateTextureFromSurface(sys_getRenderer(), tempSurface);
	SDL_RenderCopy(sys_getRenderer(), tempTexture, nullptr, &consoleFont.pos);
	SDL_DestroyTexture(tempTexture);
}