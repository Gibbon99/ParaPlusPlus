#include "classes/paraHighScore.h"

#include <utility>
#include <fstream>
#include <cstring>
#include <io/logFile.h>
#include <tinyxml2/tinyxml2.h>
#include <system/util.h>

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

		strncpy_s (lastNameUsed, defaultName.c_str (), sizeof (defaultName));

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

	strncpy_s (lastNameUsed, newName.c_str (), sizeof (lastNameUsed));

	saveFile ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Write the highscore table to external file
void paraHighScore::saveFile()
//----------------------------------------------------------------------------------------------------------------------
{
	tinyxml2::XMLDocument* xmlFileSave{};
	tinyxml2::XMLNode* rootNode{};
	tinyxml2::XMLElement* elementPtrSave{};
	//
	// Create new XML file to use
	xmlFileSave = new tinyxml2::XMLDocument;
	//
	// Create pointer to the root node
	rootNode = xmlFileSave->NewElement("Root");
	//
	// Attach root node to the file
	xmlFileSave->InsertFirstChild(rootNode);

	elementPtrSave = xmlFileSave->NewElement("Names");
	//
	// Write out the names
	for (auto fileItr = highScores.begin (); fileItr != highScores.end (); fileItr++)
	{
		tinyxml2::XMLElement* scoreNamePtr = xmlFileSave->NewElement("scoreName");
		scoreNamePtr->SetText(fileItr->name);
		elementPtrSave->InsertEndChild(scoreNamePtr);
		rootNode->InsertEndChild(elementPtrSave);
	}
	//
	// Now write out the scores
	elementPtrSave = xmlFileSave->NewElement("Scores");

	for (auto fileItr = highScores.begin(); fileItr != highScores.end(); fileItr++)
	{
		tinyxml2::XMLElement* scoreScorePtr = xmlFileSave->NewElement("scoreScore");
		scoreScorePtr->SetText(fileItr->score);
		elementPtrSave->InsertEndChild(scoreScorePtr);
		rootNode->InsertEndChild(elementPtrSave);
	}
	//
	// Save the file
	tinyxml2::XMLError eResult = xmlFileSave->SaveFile(highScoreFileName.c_str());
	if (eResult != tinyxml2::XML_SUCCESS)
		log_addEvent(sys_getString("Error writing high score file. Code [ %i ]\n", eResult));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Load the highscore table from file
void paraHighScore::loadFile()
//----------------------------------------------------------------------------------------------------------------------
{
	highScore tempScore ("DAB", 0);

	highScores.clear ();

	tinyxml2::XMLDocument* xmlFileLoad{};
	tinyxml2::XMLNode* rootNodeLoad{};
	tinyxml2::XMLElement* elementPtrLoad{};

	xmlFileLoad = new tinyxml2::XMLDocument;

	tinyxml2::XMLError eResult = xmlFileLoad->LoadFile(highScoreFileName.c_str());
	if (eResult != tinyxml2::XML_SUCCESS)
	{
		log_addEvent(sys_getString("Error reading high score file. Code [ %i ]\n", eResult));
		return;
	}
	//
	// Get root node
	rootNodeLoad = xmlFileLoad->FirstChild();
	if (nullptr == rootNodeLoad)
	{
		log_addEvent(sys_getString("Unable to load XML file [ %s ]", highScoreFileName.c_str()));
		return;
	}
	//
	// Get all the names for the high score table first
	elementPtrLoad = rootNodeLoad->FirstChildElement("Names");
	if (nullptr == elementPtrLoad)
	{
		log_addEvent(sys_getString("Unable to find node 'Names' in file [ %s ]", highScoreFileName.c_str()));
		return;
	}
	const char* tempScorename;
	std::vector<std::string> tempScoreNames{};

	tinyxml2::XMLElement* listElementPtr = elementPtrLoad->FirstChildElement("scoreName");
	while (listElementPtr != nullptr)
	{
		tempScorename = listElementPtr->GetText();
		tempScoreNames.emplace_back(tempScorename);
		listElementPtr = listElementPtr->NextSiblingElement("scoreName");
	}
	//
	// Now get each of the scores for the name
	int tempIntScore{0};
	std::vector<int> tempScoreScore{};

	elementPtrLoad = rootNodeLoad->FirstChildElement("Scores");
	if (nullptr == elementPtrLoad)
	{
		log_addEvent(sys_getString("Unable to find node 'Scores' in file [ %s ]", highScoreFileName.c_str()));
		return;
	}
	listElementPtr = elementPtrLoad->FirstChildElement("scoreScore");
	while (listElementPtr != nullptr)
	{
		eResult = listElementPtr->QueryIntText(&tempIntScore);
		if (eResult != tinyxml2::XML_SUCCESS)
		{
			log_addEvent(sys_getString("Error reading high score file. Code [ %i ]\n", eResult));
			return;
		}
		listElementPtr = listElementPtr->NextSiblingElement("scoreScore");

		tempScoreScore.push_back(tempIntScore);
	}
	//
	// Check the number of entries are the same
	if (tempScoreScore.size() != tempScoreNames.size())
	{
		log_addEvent(sys_getString("Size mismatch reading in high score entires."));
		return;
	}
	//
	// Store the values into the table
	highScores.clear();
	int scoreIndex = 0;
	for (auto& tableItr : tempScoreNames)
	{
		highScores.insert(highScore(tableItr, tempScoreScore[scoreIndex++]));
	}
}