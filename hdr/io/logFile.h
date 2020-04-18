#pragma once

#include <queue>

#include "../../hdr/main.h"
#include "../classes/paraEvent.h"

extern std::queue<paraEventLogfile *> loggingEventQueue;

// Push an event onto the logging queue
void log_addEvent(const std::string& newLine);

// Start the logfile processing queue and thread
void io_initLogFile();