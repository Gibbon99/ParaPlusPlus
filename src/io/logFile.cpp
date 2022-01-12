#include <queue>
#include <utility>
#include "system/util.h"
#include "io/logFile.h"

std::queue<paraEventLogfile *> loggingEventQueue {};

//----------------------------------------------------------------------------------------------------------------------
//
// Push an event onto the logging queue
//void log_addEvent(std::string & newLine)
void log_addEvent(std::string newLine)
//----------------------------------------------------------------------------------------------------------------------
{
	PARA_Mutex *tempMutex;

	tempMutex = evt_getMutex (LOGGING_MUTEX_NAME);

	if (nullptr == tempMutex)
		sys_shutdownWithError (sys_getString ("Unable to get mutex details [ %s ] [ %s ]", LOGGING_MUTEX_NAME, SDL_GetError ()));

	//
	// Put the new event onto the logfile queue
	PARA_LockMutex (evt_getMutex (LOGGING_MUTEX_NAME));   // Blocks if the mutex is locked by another thread
	loggingEventQueue.push (new paraEventLogfile (EVENT_ACTION_LOGFILE_WRITE, std::move (newLine)));
	PARA_UnlockMutex (evt_getMutex (LOGGING_MUTEX_NAME));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Process the events put onto the logging queue - run by detached thread
void io_processLoggingEventQueue()
//----------------------------------------------------------------------------------------------------------------------
{
	paraEventLogfile  *logFileEvent;
	static PARA_Mutex *logfileMutex = nullptr;

	while (runThreads)
	{
		evt_setThreadReady (LOGGING_THREAD_NAME);

		if (evt_shouldThreadRun (LOGGING_THREAD_NAME))
		{
			PARA_Delay (THREAD_DELAY_MS);

			if (!loggingEventQueue.empty ())   // stuff in the queue to process
			{
				if (nullptr == logfileMutex)
				{
					logfileMutex = evt_getMutex (LOGGING_MUTEX_NAME);    // Cache getting the mutex value
					if (nullptr == logfileMutex)
					{
						sys_shutdownWithError (sys_getString ("Unable to locate logfile mutex - [ %s ]", LOGGING_MUTEX_NAME));
					}
				}
				PARA_LockMutex (logfileMutex);   // Blocks if the mutex is locked by another thread
				logFileEvent = loggingEventQueue.front ();
				PARA_UnlockMutex (logfileMutex);

				if (logFileEvent->counter > 0)  // If not 0 - re-add to the queue with the reduced count
				{
//					logFileEvent->counter--;
//					sys_addEvent(EVENT_TYPE_LOGFILE, EVENT_ACTION_LOGFILE_WRITE, logFileEvent->counter, logFileEvent->newLogfileLine);
				}
				else
				{
					switch (logFileEvent->action)
					{
						case EVENT_ACTION_LOGFILE_OPEN:
							if (!logFile.open (logFileEvent->newLogfileLine))
								sys_shutdownWithError ("Error: Could not start paraLogFile. Check write permissions on folder.");
							break;

						case EVENT_ACTION_LOGFILE_CLOSE:
							logFile.close ();
							break;

						case EVENT_ACTION_LOGFILE_WRITE:
							logFile.write (logFileEvent->newLogfileLine);
							break;

						default:
							std::cout << "ERROR: Unknown logfile action [ " << logFileEvent->action << " ]" << logFileEvent->newLogfileLine << std::endl;
							break;
					}

					PARA_LockMutex (logfileMutex);   // Blocks if the mutex is locked by another thread
					delete (loggingEventQueue.front ());     // Free memory
					loggingEventQueue.pop ();
					PARA_UnlockMutex (logfileMutex);
				}
			}
		}
	}
#ifdef MY_DEBUG
	std::cout << sys_getString ("[ %s ] LOGGING thread stopped.", __func__) << std::endl;
#endif
}

//----------------------------------------------------------------------------------------------------------------------
//
// Start the logfile processing queue and thread
void io_initLogFile()
//----------------------------------------------------------------------------------------------------------------------
{
	//
	// Start the logfile and processing thread
	evt_registerMutex (LOGGING_MUTEX_NAME);
	evt_registerThread (reinterpret_cast<SDL_ThreadFunction>(io_processLoggingEventQueue), LOGGING_THREAD_NAME);
	evt_setThreadState (true, LOGGING_THREAD_NAME);

	while (!evt_isThreadReady (LOGGING_THREAD_NAME))
	{
#ifdef MY_DEBUG
		std::cout << sys_getString ("[ %s ] Waiting for logfile thread to start...", __func__) << std::endl;
#endif
	}// Wait for thread to be ready to use
	sys_addEvent (EVENT_TYPE_LOGFILE, EVENT_ACTION_LOGFILE_OPEN, 0, "./paraLogfile.txt");
}