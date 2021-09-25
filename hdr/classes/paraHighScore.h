#ifndef PARA_PARAHIGHSCORE_H
#define PARA_PARAHIGHSCORE_H

#include <string>
#include <set>
#include <cstring>

struct highScore
{
	char name[3];
	int  score {};

	bool operator> (highScore const &s) const
	{
		return score > s.score;
	}

	highScore (std::string newName, int newScore)
	{
		strncpy (name, newName.c_str (), 3);
		score = newScore;
	};
};

class paraHighScore
{
public:
	paraHighScore (std::string filePath, int numScores, const std::string defaultName, int startScore, int stepScore);

	void addNewScore (const std::string &newName, int newScore);

	void saveFile ();

	void loadFile ();

	int getScoreFromIndex (int whichIndex);

	std::string getNameFromIndex (int whichIndex);

	int lowestScore ();

private:
	int                                                m_numScores {};
	char                                               lastNameUsed[3];
	std::string                                        highScoreFileName {};
	std::multiset<highScore, std::greater<highScore> > highScores;

};


#endif //PARA_PARAHIGHSCORE_H
