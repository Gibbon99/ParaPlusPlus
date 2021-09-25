#pragma once

#include "main.h"

// Insert a new score into the highscore table
void gui_insertNewScore (const string &newName);

// Change to high score entry screen
void gui_showHighscoreEntry ();

// Change to GUI mode and display highscore table screen
void gui_showHighscoreTable ();

// Return the string representation of the highscore - by index
std::string gui_getHighScoreValueByIndex (int highScoreIndex);

// Get the string for the highscore name - by index
std::string gui_getHighScoreNameByIndex (int highScoreIndex);