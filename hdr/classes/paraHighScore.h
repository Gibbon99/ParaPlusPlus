#pragma once

#define NUM_CHARS 4		// 3 + null termination

#include <string>
#include <set>
#include <cstring>

struct highScore
{
	char name[NUM_CHARS] {};
	int  score {};

	bool operator>(highScore const &s) const
	{
		return score > s.score;
	}

	highScore(const std::string &newName, int newScore)
	{
		strncpy(name, newName.c_str(), sizeof(name));
//		strncpy (name, newName.c_str (), 3);
		score = newScore;
	};
};

class paraHighScore
{
public:
	void init(std::string filePath, int numScores, const std::string &defaultName, int startScore, int stepScore);

	void addNewScore(const std::string &newName, int newScore);

	void saveFile();

	void loadFile();

	int getScoreFromIndex(int whichIndex);

	std::string getNameFromIndex(int whichIndex);

	std::string getLastNameUsed();

	int lowestScore();

private:
	int                                                m_numScores {};
	char                                               lastNameUsed[NUM_CHARS];
	std::string                                        highScoreFileName {};
	std::multiset<highScore, std::greater<highScore> > highScores;
};