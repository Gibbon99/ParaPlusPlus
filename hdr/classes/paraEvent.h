#ifndef PARA_PARAEVENT_H
#define PARA_PARAEVENT_H

#include <string>

class paraEvent
{
public:
	paraEvent (int actionType)
	{
		action  = actionType;
		counter = 0;
	}

	int action;
	int counter;
};

//------------------------------------------------------------------------------------------------------------------
//
// Console - detached thread
//
//------------------------------------------------------------------------------------------------------------------
class paraEventConsole
{
public:
	paraEventConsole (int newAction, const std::string &newLine)
	{
		action = newAction;
		newConsoleLine = newLine;
	}

	int action;
	std::string newConsoleLine;
};

//------------------------------------------------------------------------------------------------------------------
//
// Logfile - detached thread
//
//------------------------------------------------------------------------------------------------------------------
class paraEventLogfile : public paraEvent
{
public:
	paraEventLogfile (int newAction, std::string newText) : paraEvent (newAction)
	{
		newLogfileLine = newText;
	}

	std::string newLogfileLine;

private:

};

//------------------------------------------------------------------------------------------------------------------
//
// Game - main thread
//
//------------------------------------------------------------------------------------------------------------------
class paraEventGame
{
public:

	paraEventGame (int newAction, int newCounter, const std::string &newText)
	{
		action   = newAction;
		counter  = newCounter;
		gameText = newText;
	}

	std::string gameText;
	int         action;
	int         counter;
private:

};

//------------------------------------------------------------------------------------------------------------------
//
// Audio - detached thread
//
//------------------------------------------------------------------------------------------------------------------
class paraEventAudio
{
public:
	int         action;
	int         distance;
	int         pan;
	bool        loop;
	std::string keyName;
};

#endif //PARA_PARAEVENT_H
