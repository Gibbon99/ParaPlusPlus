#pragma once

#include "main.h"

// Run an event queue on the same thread as the graphics drawing thread
void gam_processGameEventQueue();

// Add a new event to the game queue - only added when mutex is free. ie: Thread is not accessing the queue
void gam_addEvent(int newAction, int newCounter, const std::string &newLine);

// Returns the current size of the game event queue
int gam_gameEventQueueSize();

// Changing decks, to clear all the events from previous deck
void gam_clearGameEvents();