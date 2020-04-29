#include <string>
#include <classes/paraEvent.h>
#include <queue>
#include <system/startup.h>
#include <io/console.h>
#include "game/audio.h"

std::queue<paraEventAudio *> audioEventQueue;

//-----------------------------------------------------------------------------------------------------------------------
//
// Load function callback for the audio class - this function loads a file and returns a pointer to memory
SDL_RWops *io_loadAudioFile (std::string fileName)
//-----------------------------------------------------------------------------------------------------------------------
{
	SDL_RWops *memFilePtr;
	int  fileSize;
	char *soundFileData;

	if (!fileSystem.doesFileExist (fileName))
	{
		sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, sys_getString ("File not found [ %s ]", fileName.c_str ()));
		return nullptr;
	}
	//
	// How much memory does it need
	fileSize = fileSystem.getFileSize (fileName);
	if (-1 == fileSize)
	{
		sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, sys_getString ("Unable to get file size [ %s ]", fileName.c_str ()));
		return nullptr;
	}

	soundFileData = sys_malloc (fileSize, fileName);
	if (nullptr == soundFileData)
	{
		sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, sys_getString ("Unable to get memory to hold file [ %s ]", fileName.c_str ()));
		sys_shutdownWithError (sys_getString ("Unable to get memory to hold file [ %s ]", fileName.c_str ()));
	}
	//
	// Load into memory
	fileSystem.getFileIntoMemory (fileName, soundFileData);

	memFilePtr = SDL_RWFromMem (soundFileData, fileSize);

	return memFilePtr;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Put a new audio event onto the queue
void gam_addAudioEvent(int action, bool loop, int distance, int pan, std::string keyName)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_Mutex *tempMutex;
	auto tempAudioEvent = new paraEventAudio;

	tempAudioEvent->action = action;
	tempAudioEvent->loop = loop;
	tempAudioEvent->distance = distance;
	tempAudioEvent->pan = pan;
	tempAudioEvent->keyName = keyName;

	tempMutex = evt_getMutex(AUDIO_MUTEX_NAME);

	if (nullptr == tempMutex)
		sys_shutdownWithError(sys_getString("Unable to get mutex details [ %s ] [ %s ]", AUDIO_MUTEX_NAME, SDL_GetError()));

	//
	// Put the new event onto the logfile queue
	PARA_LockMutex(evt_getMutex(AUDIO_MUTEX_NAME));   // Blocks if the mutex is locked by another thread
	audioEventQueue.push(tempAudioEvent);
	PARA_UnlockMutex(evt_getMutex(AUDIO_MUTEX_NAME));

}

//----------------------------------------------------------------------------------------------------------------------
//
// Detached thread to run audio events on
void gam_processAudioEventQueue ()
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
					audioMutex = evt_getMutex (AUDIO_MUTEX_NAME);    // Cache getting the mutex value
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
						audio.init (maxNumChannels, reinterpret_cast<functionPtrStr>(con_addEvent), io_loadAudioFile);
						audio.setMasterVolume(volumeLevel); // Loaded from config file
						paraScriptInstance.run("as_loadAudioResources", "");
						break;

					case EVENT_ACTION_AUDIO_PLAY:
						audio.play (audioEvent->keyName, audioEvent->loop, audioEvent->distance, audioEvent->pan);
						break;

					case EVENT_ACTION_AUDIO_STOP_ALL:
						audio.stopAllChannels();
						break;

					case EVENT_ACTION_AUDIO_LOAD:
						audio.load (audioEvent->keyName);
						break;

					case EVENT_ACTION_AUDIO_SET_MASTER_VOLUME:
						audio.setMasterVolume(volumeLevel); // Loaded from config file
						break;

					default:
						cout << "ERROR: Unknown audio action [ " << audioEvent->action << " ]" << audioEvent->keyName << endl;
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
	std::cout << "AUDIO thread stopped." << std::endl;
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

	gam_addAudioEvent(EVENT_ACTION_AUDIO_INIT, false, 0, 0, "");
}
