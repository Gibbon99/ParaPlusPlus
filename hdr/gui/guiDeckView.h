#pragma once

#include "main.h"

// Show the entire current deck from the terminal
void gui_renderTerminalDeck ();

// Create and start the timer for player blink
void gui_startBlinkTimer(int blinkRateInMS);

// Stop the blink timer
void gui_stopBlinkTimer();