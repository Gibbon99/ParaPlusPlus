#include <string>
#include <queue>
#include "classes/paraEvent.h"
#include "system/startup.h"
#include "system/util.h"
#include "io/console.h"
#include "io/fileSystem.h"
#include "io/logFile.h"
#include "game/audio.h"

std::queue<paraEventAudio *> audioEventQueue;

//----------------------------------------------------------------------------------------------------------------------
//
// Put a new audio event onto the queue
void gam_addAudioEvent(int action, bool loop, int distance, int pan, const std::string &keyName)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_Mutex     *tempMutex;
	paraEventAudio *tempAudioEvent;

	tempAudioEvent = new paraEventAudio (action, distance, pan, loop, keyName);

	tempMutex = evt_findMutex (AUDIO_MUTEX_NAME);

	if (nullptr == tempMutex)
		sys_shutdownWithError (sys_getString ("Unable to get mutex details [ %s ] [ %s ]", AUDIO_MUTEX_NAME, SDL_GetError ()));

	//
	// Put the new event onto the logfile queue
	PARA_LockMutex (evt_findMutex (AUDIO_MUTEX_NAME));   // Blocks if the mutex is locked by another thread
	audioEventQueue.push (tempAudioEvent);
	PARA_UnlockMutex (evt_findMutex (AUDIO_MUTEX_NAME));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Detached thread to run audio events on
void gam_processAudioEventQueue()
//----------------------------------------------------------------------------------------------------------------------
{
	paraEventAudio    *audioEvent;
	static PARA_Mutex *audioMutex = nullptr;

	while (runThreads)
	{
		evt_setThreadReady (AUDIO_THREAD_NAME);

		if (evt_shouldThreadRun (AUDIO_THREAD_NAME))
		{
			PARA_Delay (THREAD_DELAY_MS);

			if (!audioEventQueue.empty ())   // stuff in the queue to process
			{
				if (nullptr == audioMutex)
				{
					audioMutex = evt_findMutex (AUDIO_MUTEX_NAME);    // Cache getting the mutex value
					if (nullptr == audioMutex)
					{
						sys_shutdownWithError (sys_getString ("Unable to locate audio mutex - [ %s ]", AUDIO_MUTEX_NAME));
					}
				}
				PARA_LockMutex (audioMutex);   // Blocks if the mutex is locked by another thread
				audioEvent = audioEventQueue.front ();
				PARA_UnlockMutex (audioMutex);

				switch (audioEvent->action)
				{
					case EVENT_ACTION_AUDIO_INIT:
						audio.init (maxNumChannels, con_addEvent, io_loadRawFile);
						audio.setMasterVolume (g_volumeLevel); // Loaded from config file
						SDL_Delay (500);
						paraScriptInstance.run ("as_loadAudioResources", "");
						break;

					case EVENT_ACTION_AUDIO_PLAY:
						audio.play (audioEvent->keyName, audioEvent->loop, audioEvent->distance, audioEvent->pan);
						break;

					case EVENT_ACTION_AUDIO_STOP_ALL:
						audio.stopAllChannels ();
						break;

					case EVENT_ACTION_AUDIO_LOAD:
						audio.load (audioEvent->keyName);
						break;

					case EVENT_ACTION_AUDIO_SET_MASTER_VOLUME:
						audio.setMasterVolume (g_volumeLevel); // Loaded from config file
						break;

					case EVENT_ACTION_AUDIO_STOP:
						audio.stop (audioEvent->keyName);
						break;

					default:
						log_addEvent (sys_getString ("[ %s ] ERROR: Unknown audio action [ %i ] for [ %s ].", __func__, audioEvent->action, audioEvent->keyName.c_str ()));
						break;
				}

				PARA_LockMutex (audioMutex);   // Blocks if the mutex is locked by another thread
				delete (audioEventQueue.front ());     // Free memory
				audioEventQueue.pop ();
				PARA_UnlockMutex (audioMutex);
			}
		}
	}
#ifdef MY_DEBUG
	log_addEvent (sys_getString ("[ %s ] AUDIO thread stopped.", __func__));
#endif
}

//----------------------------------------------------------------------------------------------------------------------
//
// Start the audio processing thread
void gam_initAudio()
//----------------------------------------------------------------------------------------------------------------------
{
	evt_registerMutex (AUDIO_MUTEX_NAME);
	evt_registerThread (reinterpret_cast<SDL_ThreadFunction>(gam_processAudioEventQueue), AUDIO_THREAD_NAME);
	evt_setThreadState (true, AUDIO_THREAD_NAME);

	gam_addAudioEvent (EVENT_ACTION_AUDIO_INIT, false, 0, 0, "");
}
