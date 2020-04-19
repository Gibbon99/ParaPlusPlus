#include <iostream>
#include <regex>

#include "../../hdr/classes/paraConsole.h"
#include "../../hdr/wrapper.h"
#include "../../hdr/main.h"

void paraConsole::add(const std::string& newLine)
{
	//
	// May need mutex gaurd here to stop adding while drawing
	// Event thread can generate new add outside mutex check
	//
	if (paraConsole::consoleText.size() < CONSOLE_MEM_SIZE)
		paraConsole::consoleText.push_back(newLine);
	else
		std::cout << "Error: Exceeded memory buffer for console text." << std::endl;
}

void paraConsole::addChar(const std::string& newChar)
{
	paraConsole::enterLine += newChar;
}

void paraConsole::deleteChar()
{
	if (!paraConsole::enterLine.empty())
	{
		paraConsole::enterLine.resize(paraConsole::enterLine.size() - 1);
	}
}

void paraConsole::addCharLine()
{
	paraConsole::processCommand(paraConsole::tokeniseEntryLine(paraConsole::enterLine));

	paraConsole::consoleText.push_back(paraConsole::enterLine);
	paraConsole::userBuffer.push_back(paraConsole::enterLine);
	paraConsole::enterLine.clear();
	paraConsole::userBufferIndex = userBuffer.size();
}

std::string paraConsole::entryLine()
{
	return "> " + paraConsole::enterLine;
}

void paraConsole::prepare(int newPosX, int newPosY)
{
	paraConsole::consoleItr = paraConsole::consoleText.rbegin();
	paraConsole::posX = newPosX;
	paraConsole::posY = newPosY;
}

void paraConsole::userBufferNext()
{
	if (paraConsole::userBufferIndex < (int)paraConsole::userBuffer.size() - 1)
	{
		paraConsole::userBufferIndex++;
		paraConsole::enterLine = paraConsole::userBuffer[userBufferIndex];
	}
}

void paraConsole::userBufferPrevious()
{
	if (paraConsole::userBufferIndex > 0)
	{
		paraConsole::userBufferIndex--;
		paraConsole::enterLine = paraConsole::userBuffer[userBufferIndex];
	}
}

void paraConsole::addVariable(const std::string& variableName, int variableType, void* variablePtr)
{
	_consoleVariable tempVariable{};

	if (variableName.empty()) // || (nullptr == variablePtr))
	{
		std::cout << "Adding a variable must include a valid name and pointer." << endl;
		return;
	}

	tempVariable.variableType = variableType;
	tempVariable.variablePtr = variablePtr;

	paraConsole::consoleVariables.insert(std::pair<std::string, _consoleVariable>(variableName, tempVariable));
}

void paraConsole::addCommand(const std::string& commandName, const std::string& functionName, const std::string& functionHelp)
{
	_consoleFunction	tempFunction;

	tempFunction.functionName = functionName;
	tempFunction.functionHelp = functionHelp;

	if ((!commandName.empty() || !functionName.empty()))
	{
		paraConsole::consoleFunctions.insert(std::pair<std::string, _consoleFunction>(commandName, tempFunction));
		return;
	}
	std::cout << "Adding console commands must include a valid commandName and functionName." << endl;
}

std::vector<std::string> paraConsole::tokeniseEntryLine(std::string entryLine)
{
	//
	// https://stackoverflow.com/a/27468529/4621054
	std::regex reg("\\s+");

	std::sregex_token_iterator iter(entryLine.begin(), entryLine.end(), reg, -1);
	std::sregex_token_iterator end;

	std::vector<std::string> enteredCommand(iter, end);

	return enteredCommand;
}

void paraConsole::processVariable(std::vector<std::string> commandLine)
{
	std::map<std::string, _consoleVariable>::iterator variableItr;

	if (commandLine[0] == "listVars")
	{
		for (const auto& varItr : consoleVariables)
		{
			paraConsole::add(sys_getString("[ %s ]", varItr.first.c_str()));
		}
		return;
	}

	if (commandLine[0] == "getVar")
	{
		if (commandLine.size() < 2)
		{
			paraConsole::add(sys_getString("[ %s ] requires an argument supplied.", commandLine[0].c_str()));
			return;
		}
		//
		// find any matching variable names
		for (int i = 1; i < (int)commandLine.size(); i++)
		{
			variableItr = consoleVariables.find(commandLine[i]);
			if (variableItr == consoleVariables.end())
			{
				paraConsole::add(sys_getString("Variable [ %s ] not found", commandLine[i].c_str()));
			}
			else
			{
				switch (variableItr->second.variableType)
				{
				case VAR_TYPE_INT:
					paraConsole::add(sys_getString("[ %s ] = [ %i ]", commandLine[i].c_str(), *(int*)variableItr->second.variablePtr));

					std::cout << "print INT" << endl;

					break;

				case VAR_TYPE_FLOAT:
					paraConsole::add(sys_getString("[ %s ] = [ %f ]", commandLine[i].c_str(), *(float*)variableItr->second.variablePtr));
					break;

				case VAR_TYPE_BOOL:
					paraConsole::add(sys_getString("[ %s ] = [ %i ]", commandLine[i].c_str(), *(bool*)variableItr->second.variablePtr));

					std::cout << "print BOOL" << endl;

					break;

				case VAR_TYPE_STRING:
					paraConsole::add(sys_getString("[ %s ] = [ %s ]", commandLine[i].c_str(), *(std::string*)variableItr->second.variablePtr));
					break;
				}
			}
		}
	}

	variableItr = consoleVariables.find(commandLine[1]);
	if (variableItr == consoleVariables.end())
	{
		paraConsole::add(sys_getString("Variable [ %s ] not found.", commandLine[1].c_str()));
		return;
	}
}

void paraConsole::processCommand(std::vector<std::string> commandLine)
{
	std::map<std::string, _consoleFunction>::iterator localConsoleItr;
	std::string parameterList;

	//
	// Handle variables on their own
	if ((commandLine[0] == "getVar") ||
		(commandLine[0] == "setVar") ||
		(commandLine[0] == "listVars"))
	{
		paraConsole::processVariable(commandLine);
		return;
	}

	localConsoleItr = consoleFunctions.find(commandLine[0]);
	if (localConsoleItr == consoleFunctions.end())
	{
		paraConsole::add(sys_getString("Command [ %s ] not found.", commandLine[0].c_str()));
		return;
	}

	if (commandLine.size() > 1)
	{
		for (int i = 1; i != (int)commandLine.size(); i++)
		{
			parameterList += commandLine[i];
			parameterList += " ";
		}
	}
	paraConsole::add(sys_getString("Run function [ %s ] with [ %s ]", localConsoleItr->second.functionName.c_str(), parameterList.c_str()));
}