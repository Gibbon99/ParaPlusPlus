#include "classes/paraHighScore.h"

#include <utility>
#include <fstream>
#include <cstring>
#include <io/logFile.h>

//----------------------------------------------------------------------------------------------------------------------
//
// Constructor
paraHighScore::paraHighScore(std::string filePath, int numScores, const std::string &defaultName, int startScore, int stepScore)
//----------------------------------------------------------------------------------------------------------------------
{
	int tableScore {};
	m_numScores       = numScores;
	highScoreFileName = std::move (filePath);
	//
	// Check if the highscore table file exists or not - fill it with default data and save if not
	std::ifstream file (highScoreFileName, std::ios::in | std::ios::binary);
	if (!file.is_open ())
	{
		tableScore = startScore;
		for (int i = 0; i < m_numScores; i++)
		{
			highScores.insert (highScore (defaultName, tableScore));
			tableScore -= stepScore;
		}

		strncpy (lastNameUsed, defaultName.c_str (), sizeof (lastNameUsed));

		saveFile ();
	}
	else
		file.close ();

	loadFile ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the lastname used as a string
std::string paraHighScore::getLastNameUsed()
//----------------------------------------------------------------------------------------------------------------------
{
	return lastNameUsed;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the lowest score from the table
int paraHighScore::lowestScore()
//----------------------------------------------------------------------------------------------------------------------
{
	auto lowestScore = highScores.end ();

	lowestScore--;

	return lowestScore->score;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the name from the table at index
std::string paraHighScore::getNameFromIndex(int whichIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	int i {};

	for (auto rowItr: highScores)
	{
		if (i == whichIndex)
			return rowItr.name;
		i++;
	}
	return "ERR";
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the score from the table at index
int paraHighScore::getScoreFromIndex(int whichIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	int i {};

	for (auto rowItr: highScores)
	{
		if (i == whichIndex)
			return rowItr.score;
		i++;
	}
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Add a new score to the table
void paraHighScore::addNewScore(const std::string &newName, int newScore)
//----------------------------------------------------------------------------------------------------------------------
{
	highScores.insert (highScore (newName, newScore));

	auto removeLast = highScores.end ();
	removeLast--;
	highScores.erase (removeLast);

	strncpy (lastNameUsed, newName.c_str (), sizeof (lastNameUsed));

	saveFile ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Write the highscore table to external file
void paraHighScore::saveFile()
//----------------------------------------------------------------------------------------------------------------------
{
	std::ofstream writeFileHandle (highScoreFileName, std::ios::out | std::ios::binary);
	if (!writeFileHandle)
	{
		printf ("Error - Unable to open highScore file for writing.\n");
		return;
	}

	writeFileHandle.write ((const char *) &lastNameUsed, sizeof (lastNameUsed));

	for (auto fileItr = highScores.begin (); fileItr != highScores.end (); fileItr++)
	{
		writeFileHandle.write ((const char *) &fileItr->name, sizeof (fileItr->name));
		writeFileHandle.write ((const char *) &fileItr->score, sizeof (fileItr->score));
	}
	writeFileHandle.close ();

	if (!writeFileHandle.good ())
		logFile.write ("Error occurred while writing highScore file.\n");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Load the highscore table from file
void paraHighScore::loadFile()
//----------------------------------------------------------------------------------------------------------------------
{
	highScore tempScore ("DAB", 0);

	highScores.clear ();

	std::ifstream readFileHandle (highScoreFileName, std::ios::in | std::ios::binary);
	if (!readFileHandle.is_open ())
	{
		printf ("Error - Unable to open highScore file for reading.\n");
		return;
	}

	readFileHandle.read ((char *) &lastNameUsed, sizeof (lastNameUsed));

	for (int i = 0; i < m_numScores; i++)
	{
		readFileHandle.read ((char *) &tempScore.name, sizeof (tempScore.name));
		readFileHandle.read ((char *) &tempScore.score, sizeof (tempScore.score));

		highScores.insert (highScore (tempScore.name, tempScore.score));
	}
	readFileHandle.close ();
	if (!readFileHandle.good ())
		printf ("Error occurred reading highScore file.\n");
}