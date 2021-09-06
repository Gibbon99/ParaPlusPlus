#include <queue>
#include <game/pathFind.h>
#include <system/frameRender.h>
#include "system/physics.h"
#include "system/util.h"
#include "classes/paraEvent.h"
#include "game/bullet.h"
#include "io/logFile.h"
#include "io/consoleFunctions.h"

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
	cout << "CONSOLE thread stopped." << endl;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Add a new event to the console queue - only added when mutex is free. ie: Thread is not accessing the queue
//
// -1 is passed in from classes to add a line to avoid including the additional header
//
// Cache the value for the Mutex on first run
void con_addEvent (int newAction, string newLine)
//----------------------------------------------------------------------------------------------------------------------
{
	static PARA_Mutex *tempMutex = nullptr;
	int               mutexStatus;
	paraEventConsole  *tempEventConsole; //(newAction, newLine);

	if (nullptr == tempMutex)
	{
		tempMutex = evt_getMutex (CONSOLE_MUTEX_NAME);
		if (nullptr == tempMutex)
			sys_shutdownWithError (sys_getString ("Unable to get mutex details [ %s ] [ %s ]", CONSOLE_MUTEX_NAME, SDL_GetError ()));
	}

	mutexStatus = SDL_TryLockMutex (tempMutex);

	if (mutexStatus == 0)
	{
		if (newAction == -1)
			newAction = EVENT_ACTION_CONSOLE_ADD_LINE;

		tempEventConsole = new paraEventConsole (newAction, newLine);

		consoleEvents.push (tempEventConsole);
		PARA_UnlockMutex (tempMutex);
	}
	else if (mutexStatus == SDL_MUTEX_TIMEDOUT)
	{
		printf ("Timed out waiting for console mutex to unlock [ %s ] [ %s ]\n", CONSOLE_MUTEX_NAME, SDL_GetError ());
	}
	else
	{
		printf ("Unable to lock mutex [ %s ] [ %s ]\n", CONSOLE_MUTEX_NAME, SDL_GetError ());
		logFile.write (sys_getString ("Unable to lock mutex [ %s ] [ %s ]", CONSOLE_MUTEX_NAME, SDL_GetError ()));
	}
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

	static int errorCount = 0;

	SDL_Delay (100);     // Slow down the mutex locking a bit

	if (console.consoleText.size () == 0)
		return;
	//
	// Block the thread from inserting new text while the iterator is rendering the array
	if (nullptr == consoleMutex)
	{
		consoleMutex = evt_getMutex (CONSOLE_MUTEX_NAME);    // cache the mutex value
		if (nullptr == consoleMutex)
			sys_shutdownWithError (sys_getString ("Unable to get Mutex value to render console [ %s ] - [ %s ]", CONSOLE_MUTEX_NAME, SDL_GetError ()));
	}

	fontClass.use ("consoleFont");
	PARA_LockMutex (consoleMutex);

	console.isDrawing = true;

	console.prepare (console.getDefaultPosX (), (float) hiresVirtualHeight - (fontClass.height () * 2));
	for (; console.consoleItr < console.consoleText.rend (); ++console.consoleItr)
	{
		fontClass.setColor (console.consoleItr->red, console.consoleItr->green, console.consoleItr->blue, console.consoleItr->alpha);
		tempSurface = fontClass.write (console.consoleItr->posX, console.posY, console.consoleItr->lineText);  // Surface is freed within console class
		if (nullptr == tempSurface)
		{
			log_addEvent (sys_getString ("%s", "1. Unable to create temp surface when rendering console."));

			printf ("Temp surface error count [ %i ]\n", errorCount);
			errorCount++;
			return;
		}
		tempTexture = SDL_CreateTextureFromSurface (renderer.renderer, tempSurface);
		if (nullptr == tempTexture)
		{
			log_addEvent (sys_getString ("%s", "2. Unable to create temp texture when rendering console."));
			return;
		}

		SDL_RenderCopyF (renderer.renderer, tempTexture, nullptr, &fontClass.pos);

		SDL_DestroyTexture (tempTexture);

		if (console.consoleItr->posX < console.getDefaultPosX () * 4)
			console.posY -= fontClass.height ();

		if (console.posY < 0)
			break;
	}

	//
	// Render the current input entry line
	console.prepare (1, (float) hiresVirtualHeight - fontClass.height ());
	fontClass.setColor (console.getDefaultRed (), console.getDefaultGreen (), console.getDefaultBlue (), console.getDefaultAlpha ());
	tempSurface = fontClass.write (console.posX, console.posY, console.entryLine ());
	if (nullptr == tempSurface)
	{
		log_addEvent (sys_getString ("%s", "3. Unable to create temp surface when rendering console entry line."));
		return;
	}
	tempTexture = SDL_CreateTextureFromSurface (renderer.renderer, tempSurface);
	if (nullptr == tempTexture)
	{
		log_addEvent (sys_getString ("%s", "4. Unable to create temp texture when rendering console."));
		return;
	}
	SDL_RenderCopyF (renderer.renderer, tempTexture, nullptr, &fontClass.pos);
	SDL_DestroyTexture (tempTexture);

	console.isDrawing = false;

	PARA_UnlockMutex (consoleMutex);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Start the console processing queue and thread
void con_initConsole ()
//----------------------------------------------------------------------------------------------------------------------
{
	console.setScreenSize (hiresVirtualWidth, hiresVirtualHeight);
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

	console.addCommand ("help", "Show available commands", con_showHelp);
	console.addCommand ("quit", "Quit the game.", sys_shutdown);

	console.addCommand ("d_showCurrentBackingTexture", "Show backing texture information.", debug_getCurrentBackingTexture);
	console.addCommand ("d_showAllBackingTextures", "List all backing textures", debug_getAllBackingTextures);

	console.addCommand ("d_showCurrentRendererInfo", "Show available renderer information.", debug_getRenderInfo);
	console.addCommand ("d_showAllRenderers", "Shows information on all renderers available", debug_getAllRenderers);

	console.addCommand ("d_showAIValues", "Show AI Values for all droids", debug_showAIValues);

	console.addCommand ("d_bullets", "Show info on bullet array", gam_debugBullets);

	console.addCommand ("d_useRenderer", "Use a new renderer - pass in index", "as_useNewRenderer");
	console.addCommand ("d_audioSpecs", "Show the audio device specs in use.", "as_getAudioSpecs");
	console.addCommand ("loadAudioResources", "Load all the audio files.", "as_loadAudioResources");
	console.addCommand ("setVolume", "Change Volume level.", "as_setVolume");
	console.addCommand ("getVolume", "View the current Volume level.", "as_getVolume");

	console.addCommand ("d_getOS", "Show which OS is in use.", sys_getOS);

	console.addCommand ("testScript", "test", "as_testFunction");
	console.addCommand ("testPlay", "Play sound", testPlay);
	//
	// Variables accessible from the console
	//
	console.addVariable ("quitLoop", VAR_TYPE_BOOL, &quitLoop);
	console.addVariable ("d_showPhysics", VAR_TYPE_BOOL, &d_showPhysics);
	console.addVariable ("d_doWallCollisions", VAR_TYPE_BOOL, &doWallCollisions);
	console.addVariable ("d_showInfluenceMap", VAR_TYPE_BOOL, &d_showInfluenceMap);
	console.addVariable ("d_showNodeArrays", VAR_TYPE_BOOL, &d_showNodeArrays);
	console.addVariable ("d_showAStarPath", VAR_TYPE_BOOL, &d_showAStarPath);
	console.addVariable ("d_showPerfStats", VAR_TYPE_BOOL, &d_showPerfStats);
	console.addVariable ("d_showPathIndex", VAR_TYPE_INT, &d_showPathIndex);
	console.addVariable ("d_showWaypoints", VAR_TYPE_BOOL, &d_showWaypoints);

	console.addVariable ("height", VAR_TYPE_INT, &testVar);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Display the list of commands to the console
void con_showHelp ()
//----------------------------------------------------------------------------------------------------------------------
{
	std::string allCommands;

	for (const auto &consoleListItr: console.consoleCommands)
	{
		allCommands = "[ " + consoleListItr.second.commandName + " ] : [ " + consoleListItr.second.commandHelp + " ]";
		console.add (allCommands);
	}
}