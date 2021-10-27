#include <system/startup.h>
#include <system/util.h>
#include <game/hud.h>
#include <game/score.h>
#include "io/logFile.h"
#include "gui/guiHighScore.h"

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
void gui_showHighscoreTable()
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
std::string gui_getHighScoreValueByIndex(int highScoreIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	if ((highScoreIndex < 0) || (highScoreIndex > NUM_HIGHSCORE_ROWS))
		return "Invalid highscore index.";

	return sys_getString ("%i", highScores.getScoreFromIndex (highScoreIndex));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the string for the highscore name - by index
std::string gui_getHighScoreNameByIndex(int highScoreIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	if ((highScoreIndex < 0) || (highScoreIndex > NUM_HIGHSCORE_ROWS))
		return "Invalid highscore index.";

	return highScores.getNameFromIndex (highScoreIndex).substr (0, 3);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Insert a new score into the highscore table
void gui_insertNewScore(const std::string &newName)
//----------------------------------------------------------------------------------------------------------------------
{
	highScores.addNewScore (newName, gam_getCurrentScore ());

//	paraScriptInstance.run ("as_refreshHighscoreLabels", "");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the lastname used string
std::string gui_getLastNameUsed()
//----------------------------------------------------------------------------------------------------------------------
{
	return highScores.getLastNameUsed ();
}