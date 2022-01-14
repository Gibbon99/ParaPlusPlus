#include <system/util.h>
#include "../main.h"
#include "../../hdr/io/console.h"
#include "../../hdr/io/logFile.h"
#include "../../hdr/system/gameEvents.h"

typedef struct paraRegisteredThreads
{
	PARA_Thread *thread = nullptr;
	bool        run     = false;
	bool        ready   = false;
	std::string name{};
} paraRegisteredThreads;

typedef struct paraRegisteredMutexes
{
	PARA_Mutex  *mutex = nullptr;
	std::string name{};
} paraRegisteredMutexes;

std::vector<paraRegisteredThreads> registeredThreads {};
std::vector<paraRegisteredMutexes> registeredMutexes {};

bool runThreads = true;     // Master flag to control state of detached threads

//----------------------------------------------------------------------------------------------------------------------
//
// Set thread ready
void evt_setThreadReady(const std::string &threadName)
//----------------------------------------------------------------------------------------------------------------------
{
	//
	// registerThreads.empty profiles at 9%;
	// registerThreads.size() == 0 profiles at < 1%
	if (registeredThreads.size () == 0)
		return;

	for (auto &threadItr: registeredThreads)
	{
		if (threadName == threadItr.name)
		{
			if (!threadItr.ready)
			{
				threadItr.ready = true;
#ifdef MY_DEBUG
				log_addEvent (sys_getString ("[ %s ] Thread [ %s ] set to ready = true", __func__, threadName.c_str ()));
#endif
				return;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// See if the thread is ready or not
bool evt_isThreadReady(const std::string &threadName)
//----------------------------------------------------------------------------------------------------------------------
{
	if (registeredThreads.size () == 0)
		return false;

	for (const auto &threadItr: registeredThreads)
	{
		if (threadName == threadItr.name)
		{
			return threadItr.ready;
		}
	}
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Should this thread be running
bool evt_shouldThreadRun(const std::string &threadName)
//----------------------------------------------------------------------------------------------------------------------
{
	if (registeredThreads.size () == 0)
	{
		log_addEvent (sys_getString ("[ %s ] Attempting to start thread [ %s ] but thread is not created.", __func__, threadName.c_str ()));
		PARA_Delay (100);     // Give thread time to startup
		return false;
	}

	for (const auto &threadItr: registeredThreads)
	{
		if (threadName == threadItr.name)
			return threadItr.run;
	}

	log_addEvent (sys_getString ("[ %s ] Attempting to start thread [ %s ] but thread is not created.", __func__, threadName.c_str ()));
	PARA_Delay (100);     // Give thread time to startup
	return false;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Control thread running and processing based on thread name
void evt_setThreadState(int newState, const std::string &threadName)
//----------------------------------------------------------------------------------------------------------------------
{
	if (registeredThreads.empty ())
	{
		log_addEvent (sys_getString ("[ %s ] Attempting to change the state of thread [ %s ] but thread is not created.", __func__, threadName.c_str ()));
		PARA_Delay (100);     // Give thread time to startup
		return;
	}

	for (auto &threadItr: registeredThreads)
	{
		if (threadName == threadItr.name)
		{
			threadItr.run = newState;
			return;
		}
	}
	log_addEvent (sys_getString ("[ %s ] Attempting to change the state of thread [ %s ] but thread is not created.", __func__, threadName.c_str ()));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Stop threads
void evt_stopThreads()
//----------------------------------------------------------------------------------------------------------------------
{
	runThreads = false;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Keep a list of threads that have been created.
void evt_registerThread(SDL_ThreadFunction threadFunction, const std::string &threadName)
//----------------------------------------------------------------------------------------------------------------------
{
	paraRegisteredThreads newThread;

	newThread.name  = threadName;
	newThread.run   = false;
	newThread.ready = false;
	registeredThreads.push_back (newThread);
	//
	// Create vector element first
	// Thread was being created too fast and running before the vector element was created
	// resulting in no elements being available when size() was tested
	registeredThreads[registeredThreads.size () - 1].thread = PARA_CreateThread (*threadFunction, threadName.c_str (), nullptr);
	if (nullptr == registeredThreads[registeredThreads.size () - 1].thread)
		sys_shutdownWithError (sys_getString ("Unable to create thread [ %s ] - [ %s ]", threadName.c_str (), SDL_GetError ()));

	SDL_DetachThread (registeredThreads[registeredThreads.size () - 1].thread);
	evt_setThreadReady (threadName);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Keep a list of mutexes that have been created, and remove at shutdown
void evt_registerMutex(const std::string &mutexName)
//----------------------------------------------------------------------------------------------------------------------
{
	paraRegisteredMutexes newMutex;

	newMutex.mutex = PARA_CreateMutex ();
	newMutex.name  = mutexName;
	if (nullptr == newMutex.mutex)
		sys_shutdownWithError (sys_getString ("Unable to create mutex [ %s ] - [ %s ]", newMutex.name.c_str (), SDL_GetError ()));

	registeredMutexes.push_back (newMutex);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Find a mutex based on its name
PARA_Mutex *evt_findMutex(const std::string &mutexName)
//----------------------------------------------------------------------------------------------------------------------
{
	if (registeredMutexes.size () == 0)
	{
//		log_addEvent (sys_getString ("[ %s ] Attempting to access mutex [ %s ] but mutex pool is empty.", "evt_findMutex", mutexName.c_str ()));
		return nullptr;
	}

	for (const auto &mutexItr: registeredMutexes)
	{
		if (mutexItr.name == mutexName)
		{
			return mutexItr.mutex;
		}
	}

//	sys_shutdownWithError (sys_getString ("Unable to locate mutex [ %s ]", mutexName.c_str ()));
	return nullptr;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Destroy all the mutexes
void evt_destroyMutexes()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &mutexItr: registeredMutexes)
	{
		PARA_DestroyMutex (mutexItr.mutex);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Add a new event to its queue
void sys_addEvent(int eventType, int eventAction, int eventDelay, const std::string &eventText)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_Mutex *tempMutex;

	switch (eventType)
	{
		case EVENT_TYPE_CONSOLE:
			con_addEvent (eventAction, eventText);
			break;

		case EVENT_TYPE_LOGFILE:
			tempMutex = evt_findMutex (LOGGING_MUTEX_NAME);
			if (nullptr == tempMutex)
				//sys_shutdownWithError (sys_getString ("Unable to find mutex [ %s ]", LOGGING_MUTEX_NAME));
				printf("Unable to find mutex [ %s ]", LOGGING_MUTEX_NAME);
			//
			// Put the new event onto the logfile queue
			PARA_LockMutex (evt_findMutex (LOGGING_MUTEX_NAME));   // Blocks if the mutex is locked by another thread
			loggingEventQueue.push (new paraEventLogfile (eventAction, eventText));
			PARA_UnlockMutex (evt_findMutex (LOGGING_MUTEX_NAME));
			break;

		case EVENT_TYPE_GAME:   // Push an event back into the main thread
			gam_addEvent (eventAction, eventDelay, eventText);
			return;

		default:
			log_addEvent (sys_getString ("Unknown event type used. [ %s ]", eventText.c_str ()));
			break;
	}
}