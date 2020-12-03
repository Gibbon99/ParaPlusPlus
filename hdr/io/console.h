#pragma once

#include "../main.h"

// Render the console to the screen
void con_renderConsole();

// Add a new event to the console queue
void con_addEvent(int newAction, std::string newLine);

// Start the console processing queue and thread
void con_initConsole();

// Show the commands and help text
void consoleShowHelp ();