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
class paraEventConsole : public paraEvent
{
public:
	paraEventConsole (int newAction, const std::string &newLine) : paraEvent (newAction)
	{
		newConsoleLine = newLine;
	}

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
class paraEventGame : public paraEvent
{
public:
	paraEventGame (int newAction, int newCounter, const std::string &newText) : paraEvent (newAction)
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
