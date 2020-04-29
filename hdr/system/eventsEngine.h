#pragma once

#define CONSOLE_MUTEX_NAME        "consoleMutex"
#define CONSOLE_THREAD_NAME        "consoleThread"

#define LOGGING_MUTEX_NAME      "logfileMutex"
#define LOGGING_THREAD_NAME     "logfileThread"

#define AUDIO_MUTEX_NAME        "audioMutex"
#define AUDIO_THREAD_NAME       "audioThread"

#define GAME_MUTEX_NAME         "gameThread"

#define THREAD_DELAY_MS         1       // in MS

extern bool runThreads;     // Master flag to control state of detached threads

// Keep a list of threads that have been created.
void evt_registerThread (SDL_ThreadFunction threadFunction, const std::string &threadName);

// Keep a list of mutexes that have been created, and remove at shutdown
void evt_registerMutex (const std::string &mutexName);

// Add a new event to its queue
void sys_addEvent(int eventType, int eventAction, int eventDelay, const std::string& eventText);

// Find a mutex based on its name
PARA_Mutex *evt_getMutex(const std::string& mutexName);

// Destroy all the mutexes
void evt_destroyMutexes();

// Control thread running and processing based on thread name
void evt_setThreadState(int newState, const std::string& threadName);

// Stop threads
void evt_stopThreads();

// Should this thread be running
bool evt_shouldThreadRun(const std::string& threadName);

// See if the thread is ready or not
bool evt_isThreadReady(const std::string& threadName);

// Set thread ready
void evt_setThreadReady(const std::string& threadName);
