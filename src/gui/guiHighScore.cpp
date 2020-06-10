#include <system/startup.h>
#include <system/util.h>
#include <game/hud.h>
#include <gui/guiLanguage.h>
#include "io/logFile.h"
#include "gui/guiHighScore.h"

std::string highScoreFilename = "highscore.dat";
std::array<int, 5> highScores = {10, 20, 30, 40, 50};

float headingPosX;
float headingPosY;
float tableSpacingY;
float rowNumberPosX;
float rowNumberScoreX;
float rowY;

std::string headingText = "High Scores";

//----------------------------------------------------------------------------------------------------------------------
//
// Save the highscores to a file
void gui_writeHighScore()
//----------------------------------------------------------------------------------------------------------------------
{
	if (!fileSystem.save(highScoreFilename, &highScores, highScores.size() * sizeof(int)))
	{
		log_addEvent("Unable to write highscore file.");
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Read the highscore file into array
void gui_readHighScore()
//----------------------------------------------------------------------------------------------------------------------
{
	if (fileSystem.doesFileExist(highScoreFilename))
	{
		fileSystem.getFileIntoMemory(highScoreFilename, &highScores);
	}

#ifdef MY_DEBUG
	for (auto scoreItr : highScores)
		std::cout << "Score : " << scoreItr << std::endl;
#endif

}

//----------------------------------------------------------------------------------------------------------------------
//
// Prepare to render the high score screen
void gui_prepareHighScoreScreen()
//----------------------------------------------------------------------------------------------------------------------
{
	//
	// Sort the table for presentation
	std::sort(highScores.begin(), highScores.end(), std::greater<int>());
	//
	// Get localised text
	headingText = gui_getString("highScore");

	headingPosX = (hiresVirtualWidth - fontClass.width (headingText)) / 2;
	headingPosY = textures.at ("hudNew").getHeight () + 50;

	rowNumberPosX = hiresVirtualWidth / 3;
	rowNumberScoreX = rowNumberPosX + fontClass.width ("00000");

	rowY = headingPosY + fontClass.height() * 2;

	gam_setHudText ("highScore");

	sys_setNewMode(MODE_GUI_HIGHSCORE_SCREEN, true);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render the high score screen
void gui_renderHighScoreScreen()
//----------------------------------------------------------------------------------------------------------------------
{
	int indexCount = 1;

	fontClass.use ("guiFont");

	fontClass.render (renderer.renderer, headingPosX, headingPosY, 255, 255, 255, 255, sys_getString ("%s", headingText.c_str ()));

	for (auto scoreItr : highScores)
	{
		fontClass.render (renderer.renderer, rowNumberPosX, rowY + (tableSpacingY * indexCount), 255, 255 - (10 * indexCount), 255 - (10 * indexCount), 255, sys_getString ("%i.", indexCount));
		fontClass.render(renderer.renderer, rowNumberScoreX, rowY + (tableSpacingY * indexCount), 255, 255 - (10 * indexCount), 255 - (10 * indexCount), 255, sys_getString("%i", scoreItr));
		indexCount++;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Insert a new score into the highscore table
void gui_insertNewScore(int newScore)
//----------------------------------------------------------------------------------------------------------------------
{
	int scoreIndex = 0;

	std::sort(highScores.begin(), highScores.end(), std::less<int>());

	if (newScore <= highScores[0])
		return; // Didn't make it onto the table

	//
	// Handle new highest score - shuffle other scores down
	if (newScore > highScores[highScores.size()])
	{
		for (scoreIndex = 0; scoreIndex < highScores.size() - 1; scoreIndex++)
		{
			highScores[scoreIndex] = highScores[scoreIndex + 1];
		}

		highScores[highScores.size () - 1] = newScore;

		return;
	}

	for (scoreIndex = 0; scoreIndex != highScores.size() - 1; scoreIndex++)
	{
		if (newScore > highScores[scoreIndex])
		{
			highScores[scoreIndex] - highScores[scoreIndex + 1];
		}
		else
		{
			highScores[scoreIndex] = newScore;
		}
	}
}