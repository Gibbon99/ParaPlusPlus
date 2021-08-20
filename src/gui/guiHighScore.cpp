#include <array>
#include <system/startup.h>
#include <system/util.h>
#include <game/hud.h>
#include <game/score.h>
#include "io/logFile.h"
#include "gui/guiHighScore.h"

struct highScore
{
	public:
		int  scoreValue{};
		char nameValue[4]{};
};

std::string                               highScoreFilename = "highscore.dat";
std::array<highScore, NUM_HIGHSCORE_ROWS> highScoreTable{};

//----------------------------------------------------------------------------------------------------------------------
//
// Return the current high score from the table
int gui_getLowestScore()
//----------------------------------------------------------------------------------------------------------------------
{
	return highScoreTable[NUM_HIGHSCORE_ROWS - 1].scoreValue;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Sort function for highscoretable
bool gui_scoreSortFunction (highScore const &lhs, highScore const &rhs)
//----------------------------------------------------------------------------------------------------------------------
{
	return lhs.scoreValue > rhs.scoreValue;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Change to high score entry screen
void gui_showHighscoreEntry()
//----------------------------------------------------------------------------------------------------------------------
{
	sys_setNewMode (MODE_GUI_HIGHSCORE_ENTRY, true);

	gam_setHudText ("hudHighscore");

	gui.setCurrentScreen (gui.getIndex (GUI_OBJECT_SCREEN, "highScoreEntry"));
	// Set active object
	gui.setActiveObject (gui.getCurrentScreen (), GUI_OBJECT_BUTTON, "highScoreEntry.backButton");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Change to GUI mode and display highscore table screen
void gui_showHighscoreTable ()
//----------------------------------------------------------------------------------------------------------------------
{
	sys_setNewMode (MODE_GUI_HIGHSCORE_DISPLAY, true);

	gam_setHudText ("hudHighscore");
	gui.setCurrentScreen (gui.getIndex (GUI_OBJECT_SCREEN, "highScoreDisplay"));
	// Set active object
	gui.setActiveObject (gui.getCurrentScreen (), GUI_OBJECT_BUTTON, "highScoreDisplay.backButton");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the string representation of the highscore - by index
std::string gui_getHighScoreValueByIndex (int highScoreIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	if ((highScoreIndex < 0) || (highScoreIndex > NUM_HIGHSCORE_ROWS))
		return "Invalid highscore index.";

	return sys_getString ("%i", highScoreTable[highScoreIndex].scoreValue);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the string for the highscore name - by index
std::string gui_getHighScoreNameByIndex (int highScoreIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	if ((highScoreIndex < 0) || (highScoreIndex > NUM_HIGHSCORE_ROWS))
		return "Invalid highscore index.";

	return highScoreTable[highScoreIndex].nameValue;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Save the highscores to a file
void gui_writeHighScore ()
//----------------------------------------------------------------------------------------------------------------------
{
	if (!fileSystem.save (highScoreFilename, &highScoreTable, highScoreTable.size () * sizeof (highScore)))
	{
		log_addEvent ("Unable to write highscore file.");
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Read the highscore file into array - if the file can't be found init array with starting values and save it
void gui_readHighScore ()
//----------------------------------------------------------------------------------------------------------------------
{
	if (fileSystem.doesFileExist (highScoreFilename))
	{
		fileSystem.getFileIntoMemory (highScoreFilename, &highScoreTable);
	}
	else
	{
		for (int i = 0; i != NUM_HIGHSCORE_ROWS; i++)
		{
			highScoreTable[i].scoreValue = i * 1000;
#ifdef __WIN32__
			strcpy_s (highScoreTable[i].nameValue, "DAB");
#else
			strcpy (highScoreTable[i].nameValue, "DAB");
#endif
		}
		gui_writeHighScore ();
	}

	std::sort (highScoreTable.begin (), highScoreTable.end (), &gui_scoreSortFunction);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Insert a new score into the highscore table
void gui_insertNewScore (string newName)
//----------------------------------------------------------------------------------------------------------------------
{
	int scoreIndex = 0;
	int newScore;

	newScore = gam_getCurrentScore();

	std::sort (highScoreTable.begin (), highScoreTable.end (), &gui_scoreSortFunction);

	if (newScore <= highScoreTable.end ()->scoreValue)
		return; // Didn't make it onto the table

	//
	// Handle new highest score, push other scores down
	if (newScore >= highScoreTable.begin ()->scoreValue)
	{
		for (scoreIndex = highScoreTable.size (); scoreIndex != 0; scoreIndex--)
		{
			highScoreTable[scoreIndex] = highScoreTable[scoreIndex - 1];
		}

		highScoreTable.begin ()->scoreValue = newScore;
#ifdef __WIN32__
		strcpy_s (highScoreTable.begin ()->nameValue, newName.c_str ());
#else
		strcpy (highScoreTable.begin ()->nameValue, newName.c_str ());
#endif

		paraScriptInstance.run ("as_refreshHighscoreLabels", "");
		return;
	}

	//
	// Insert a new score into the table, push other scores down
	for (scoreIndex = highScoreTable.size (); scoreIndex != 0; scoreIndex--)
	{
		if ((newScore > highScoreTable[scoreIndex].scoreValue) && (newScore > highScoreTable[scoreIndex - 1].scoreValue))
		{
			highScoreTable[scoreIndex] = highScoreTable[scoreIndex - 1];
		}
		else
		{
			highScoreTable[scoreIndex].scoreValue = newScore;
#ifdef __WIN32__
			strcpy_s (highScoreTable[scoreIndex].nameValue, newName.c_str ());
#else
			strcpy (highScoreTable[scoreIndex].nameValue, newName.c_str ());
#endif
			paraScriptInstance.run ("as_refreshHighscoreLabels", "");
			return;
		}
	}
}