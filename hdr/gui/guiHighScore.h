#pragma once

#include "main.h"

extern float tableSpacingY;

// Save the highscores to a file
void gui_writeHighScore();

// Read the highscore file into array
void gui_readHighScore();

// Prepare to render the high score screen
void gui_prepareHighScoreScreen();

// Render the high score screen
void gui_renderHighScoreScreen();

// Insert a new score into the highscore table
void gui_insertNewScore(int newScore);