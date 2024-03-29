#include "gui/guiHighScore.h"
#include "game/score.h"
#include "game/alertLevel.h"

int          redAlertLevel;
int          yellowAlertLevel;
static int   currentScore       = 0;
static int   printableScore     = 0;
float        updateScoreDelay   = 0.4f;
static float updateScoreCounter = 0.0f;

//------------------------------------------------------------------------------------------------------
//
// Modify the score buffer - send in positive or negative amount
void gam_modifyScore(int modifyAmount)
//------------------------------------------------------------------------------------------------------
{
	currentScore += modifyAmount;
}

//------------------------------------------------------------------------------------------------------
//
// Process the scoreDelay to see if the printableScore needs updating
void gam_processScore()
//------------------------------------------------------------------------------------------------------
{
	if (currentScore == printableScore)
		return;     // Nothing to process

	updateScoreCounter += 1.0f * updateScoreDelay;
	if (updateScoreCounter > 1.0f)
	{
		updateScoreCounter = 0.0f;
		if (currentScore > 0)
		{
			currentScore--;
			printableScore++;
		}

		if (gam_getCurrentAlertLevel () == ALERT_YELLOW_TILE)
			gam_modifyScore (5);

		if (gam_getCurrentAlertLevel () == ALERT_RED_TILE)
			gam_modifyScore (10);
	}

	if (currentScore - printableScore > yellowAlertLevel)
	{
		if (gam_getCurrentAlertLevel () != ALERT_YELLOW_TILE)
			gam_setAlertLevel (ALERT_YELLOW_TILE);
	}
	else if (currentScore - printableScore > redAlertLevel)
	{
		if (gam_getCurrentAlertLevel () != ALERT_RED_TILE)
			gam_setAlertLevel (ALERT_RED_TILE);
	}
	else
	{
		if (gam_getCurrentAlertLevel () != ALERT_GREEN_TILE)
			gam_setAlertLevel (ALERT_GREEN_TILE);
	}
}

//------------------------------------------------------------------------------------------------------
//
// Set the final score to display on game over screens
void gam_setFinalScore()
//------------------------------------------------------------------------------------------------------
{
	printableScore += currentScore;
	currentScore = 0;
}

//------------------------------------------------------------------------------------------------------
//
// Return the actual score - used for game over
int gam_getCurrentScore()
//------------------------------------------------------------------------------------------------------
{
	return printableScore + currentScore;
}

//------------------------------------------------------------------------------------------------------
//
// Return the current score
int gam_getPrintableScore()
//------------------------------------------------------------------------------------------------------
{
	return printableScore;
}

//------------------------------------------------------------------------------------------------------
//
// Set the starting values for the score
void gam_initScoreValues()
//------------------------------------------------------------------------------------------------------
{
	currentScore   = 0;
	printableScore = 0;
}

//------------------------------------------------------------------------------------------------------
//
// See if we need to show the high score entry screen - or just the high score screen
void gam_decideScoreAction()
//------------------------------------------------------------------------------------------------------
{
	int finalScore;

	finalScore = currentScore + printableScore;

	if (finalScore >= highScores.lowestScore ()) // gui_getLowestScore ())
	{
		gui_showHighscoreEntry ();
	}
	else
	{
		gui_showHighscoreTable ();
	}
}