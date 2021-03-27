#pragma once

#include "main.h"

extern float tableSpacingY;

// Save the highscores to a file
void gui_writeHighScore();

// Read the highscore file into array
void gui_readHighScore();

// Insert a new score into the highscore table
void gui_insertNewScore (string newName);

// Change to GUI mode and display highscore table screen
void gui_showHighscoreTable();

// Return the string representation of the highscore - by index
std::string gui_getHighScoreValueByIndex(int highScoreIndex);

// Get the string for the highscore name - by index
std::string gui_getHighScoreNameByIndex(int highScoreIndex);

// Change to high score entry screen
void gui_showHighscoreEntry();

// Return the current high score from the table
int gui_getLowestScore();