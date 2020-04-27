#pragma once

#include "../../hdr/main.h"

// Run an event queue on the same thread as the graphics drawing thread
void gam_processGameEventQueue ();

// Add a new event to the game queue - only added when mutex is free. ie: Thread is not accessing the queue
void gam_addEvent (int newAction, int newCounter, const std::string &newLine);
