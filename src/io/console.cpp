#include <queue>
#include "../../hdr/io/console.h"
#include "../../hdr/system/startup.h"
#include "../../hdr/classes/paraEvent.h"
#include "../../hdr/io/logFile.h"
#include "../main.h"
#include "../../hdr/io/consoleFunctions.h"

std::queue<paraEventConsole *> consoleEvents;

int testVar = 100;

//----------------------------------------------------------------------------------------------------------------------
//
// Process the contents of the console queue - run by detached thread
void con_processConsoleEventQueue (void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	paraEventConsole  *tempEvent;
	static PARA_Mutex *consoleMutex = nullptr;

	while (runThreads)
	{
		evt_setThreadReady (CONSOLE_THREAD_NAME);

		if (evt_shouldThreadRun (CONSOLE_THREAD_NAME))
		{
			PARA_Delay (THREAD_DELAY_MS);

			if (!consoleEvents.empty ())     // events in the queue to process
			{
				if (nullptr == consoleMutex)
				{
					consoleMutex = evt_getMutex (CONSOLE_MUTEX_NAME);    // cache the mutex value
					if (nullptr == consoleMutex)
						sys_shutdownWithError (sys_getString ("Unable to get Mutex value [ %s ] - [ %s ]", CONSOLE_MUTEX_NAME, SDL_GetError ()));
				}
				PARA_LockMutex (consoleMutex);
				tempEvent = consoleEvents.front ();
				PARA_UnlockMutex (consoleMutex);

				if (tempEvent->counter > 0)  // If not 0 - re-add to the queue with the reduced count
				{
					tempEvent->counter--;
					con_addEvent (tempEvent->counter, tempEvent->newConsoleLine);    // TODO - need to add counter as parameter - currently newAction
				}
				else
				{
					switch (tempEvent->action)
					{
						case EVENT_ACTION_CONSOLE_ADD_LINE:
							console.add (tempEvent->newConsoleLine);

							std::cout << "Console text : " << tempEvent->newConsoleLine << std::endl;

							break;

						case EVENT_ACTION_CONSOLE_ADD_CHAR:
							console.addChar (tempEvent->newConsoleLine);
							break;

						case EVENT_ACTION_CONSOLE_DELETE_CHAR:
							console.deleteChar ();
							break;

						case EVENT_ACTION_CONSOLE_ADD_CHAR_LINE:
							console.addCharLine ();
							break;
					}

					PARA_LockMutex (consoleMutex);           // Blocks if the mutex is locked by another thread
					delete (consoleEvents.front ());         // Free memory
					consoleEvents.pop ();
					PARA_UnlockMutex (consoleMutex);
				}
			}
		}
	}
	cout << "CONSOLE thread stopped." << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Add a new event to the console queue - only added when mutex is free. ie: Thread is not accessing the queue
//
// -1 is passed in from classes to add a line to avoid including the additional header
void con_addEvent (int newAction, const std::string &newLine)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_Mutex *tempMutex;

	if (newAction == -1)
		newAction = EVENT_ACTION_CONSOLE_ADD_LINE;

	tempMutex = evt_getMutex (CONSOLE_MUTEX_NAME);
	if (nullptr == tempMutex)
		sys_shutdownWithError (sys_getString ("Unable to get mutex details [ %s ] [ %s ]", CONSOLE_MUTEX_NAME, SDL_GetError ()));

	if (PARA_LockMutex (tempMutex) == 0)
	{
		consoleEvents.push (new paraEventConsole (newAction, newLine));
		PARA_UnlockMutex (tempMutex);
	}
	else
		logFile.write (sys_getString ("Unable to lock mutex [ %s ] [ %s ]", CONSOLE_MUTEX_NAME, SDL_GetError ()));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render the console to the screen
void con_renderConsole ()
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_Surface      *tempSurface  = nullptr;
	PARA_Texture      *tempTexture  = nullptr;
	static PARA_Mutex *consoleMutex = nullptr;

	//
	// Block the thread from inserting new text while the iterator is rendering the array
	if (nullptr == consoleMutex)
	{
		consoleMutex = evt_getMutex (CONSOLE_MUTEX_NAME);    // cache the mutex value
		if (nullptr == consoleMutex)
			sys_shutdownWithError (sys_getString ("Unable to get Mutex value to render console [ %s ] - [ %s ]", CONSOLE_MUTEX_NAME, SDL_GetError ()));
	}

	PARA_LockMutex (consoleMutex);

	console.isDrawing = true;

	console.prepare (console.getDefaultPosX (), (float) consoleVirtualHeight - (consoleFont.lineHeight * 2));
	for (; console.consoleItr != console.consoleText.rend (); ++console.consoleItr)
	{
		consoleFont.setColor (console.consoleItr->red, console.consoleItr->green, console.consoleItr->blue, console.consoleItr->alpha);
		tempSurface = consoleFont.write (console.consoleItr->posX, console.posY, console.consoleItr->lineText);  // Surface is freed within console class
		if (nullptr == tempSurface)
		{
			log_addEvent ("Unable to create temp surface when rendering console.");
			return;
		}
		tempTexture = SDL_CreateTextureFromSurface (renderer.renderer, tempSurface);
		if (nullptr == tempTexture)
		{
			log_addEvent ("Unable to create temp texture when rendering console.");
			return;
		}

		SDL_RenderCopy (renderer.renderer, tempTexture, nullptr, &consoleFont.pos);

		SDL_DestroyTexture (tempTexture);

		if (console.consoleItr->posX < console.getDefaultPosX () * 4)
			console.posY -= consoleFont.lineHeight;

		if (console.posY < 0)
			break;
	}

	//
	// Render the current input entry line
	console.prepare (1, (float) consoleVirtualHeight - consoleFont.lineHeight);
	consoleFont.setColor (console.getDefaultRed (), console.getDefaultGreen (), console.getDefaultBlue (), console.getDefaultAlpha ());
	tempSurface = consoleFont.write (console.posX, console.posY, console.entryLine ());
	if (nullptr == tempSurface)
	{
		log_addEvent ("Unable to create temp surface when rendering console entry line.");
		return;
	}
	tempTexture = SDL_CreateTextureFromSurface (renderer.renderer, tempSurface);
	if (nullptr == tempTexture)
	{
		log_addEvent ("Unable to create temp texture when rendering console.");
		return;
	}
	SDL_RenderCopy (renderer.renderer, tempTexture, nullptr, &consoleFont.pos);
	SDL_DestroyTexture (tempTexture);

	console.isDrawing = false;

	PARA_UnlockMutex (consoleMutex);

	//
	// Show performance stats
	consoleFont.setColor (255, 0, 255, 255);
	tempSurface = consoleFont.write (1, 10, sys_getString ("intoNextFrame : %f Think : %i FPS : %i", percentIntoNextFrame, thinkFPSPrint, fpsPrint));
	if (nullptr == tempSurface)
	{
		log_addEvent ("Unable to create temp surface when rendering console.");
		return;
	}
	tempTexture = SDL_CreateTextureFromSurface (renderer.renderer, tempSurface);
	if (nullptr == tempTexture)
	{
		log_addEvent ("Unable to create temp texture when rendering console.");
		return;
	}
	SDL_RenderCopy (renderer.renderer, tempTexture, nullptr, &consoleFont.pos);
	SDL_DestroyTexture (tempTexture);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Start the console processing queue and thread
void con_initConsole ()
//----------------------------------------------------------------------------------------------------------------------
{
	console.setScreenSize (consoleVirtualWidth, consoleVirtualHeight);
	console.setNumVarColumns (consoleNumColumns);
	//
	// Start the console and processing thread
	evt_registerMutex (CONSOLE_MUTEX_NAME);
	evt_registerThread (reinterpret_cast<SDL_ThreadFunction>(con_processConsoleEventQueue), CONSOLE_THREAD_NAME);

	evt_setThreadState (true, CONSOLE_THREAD_NAME);

	while (!evt_isThreadReady (CONSOLE_THREAD_NAME))
	{
#ifdef MY_DEBUG
		cout << "Waiting for console thread to start..." << endl;
#endif
	}// Wait for thread to be ready to use

	sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, sys_getString ("Console started [ %s ]", APP_NAME));

	console.addCommand ("help", "Show available commands", "functionHelp");
	console.addCommand ("exit", "Shutdown", "functionQuit");

	console.addCommand ("d_showCurrentBackingTexture", "Show backing texture information.", debug_getCurrentBackingTexture);
	console.addCommand ("d_showAllBackingTextures", "List all backing textures", debug_getAllBackingTextures);

	console.addCommand ("d_showCurrentRendererInfo", "Show available renderer information.", debug_getRenderInfo);
	console.addCommand ("d_showAllRenderers", "Shows information on all renderers available", debug_getAllRenderers);

	console.addCommand ("d_useRenderer", "Use a new renderer - pass in index", "as_useNewRenderer");
	console.addCommand ("testScript", "test", "as_testFunction");

	console.addCommand ("d_getOS", "Show which OS is in use.", sys_getOS);
	console.addCommand ("quit", "Quit the game.", sys_shutdown);

	console.addVariable ("quitLoop", VAR_TYPE_BOOL, &quitLoop);
	console.addVariable ("height", VAR_TYPE_INT, &testVar);
	console.addVariable ("testString", VAR_TYPE_STRING, &testVarString);
	console.addVariable ("testFloat", VAR_TYPE_FLOAT, &testVarFloat);
}
