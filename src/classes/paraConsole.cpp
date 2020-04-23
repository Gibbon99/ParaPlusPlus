#include <iostream>
#include <regex>

#include "../../hdr/classes/paraConsole.h"
#include "../../hdr/wrapper.h"
#include "../../hdr/main.h"

void paraConsole::add(const std::string &newLine)
{
	//
	// May need mutex guard here to stop adding while drawing
	// Event thread can generate new add outside mutex check
	//
	if (paraConsole::consoleText.size() < CONSOLE_MEM_SIZE)
		paraConsole::consoleText.push_back(newLine);
	else
		std::cout << "Error: Exceeded memory buffer for console text." << std::endl;
}

void paraConsole::addChar(const std::string &newChar)
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
	paraConsole::consoleText.push_back(paraConsole::enterLine);
	paraConsole::userBuffer.push_back(paraConsole::enterLine);

	paraConsole::processCommand(paraConsole::tokeniseEntryLine(paraConsole::enterLine));

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
	paraConsole::posX       = newPosX;
	paraConsole::posY       = newPosY;
}

void paraConsole::userBufferNext()
{
	if (paraConsole::userBufferIndex < (int) paraConsole::userBuffer.size() - 1)
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

void paraConsole::addVariable(const std::string &variableName, int variableType, void *variablePtr)
{
	_variables tempVar;

	if (variableName.empty()) // || (nullptr == variablePtr))
	{
		std::cout << "Adding a variable must include a valid name and pointer." << endl;
		return;
	}

	for (const auto& varItr : consoleVariables)
	{
		if (varItr.varName == variableName)
		{
			cout << "Variable [ " << variableName << " ] already exists." << endl;
			return;
		}
	}

	tempVar.varName = variableName;
	tempVar.varType = variableType;

	switch (variableType)
	{
		case VAR_TYPE_INT:
			tempVar.varPtrInt = (int *) variablePtr;
			break;

		case VAR_TYPE_FLOAT:
			tempVar.varPtrFloat = (float *) variablePtr;
			break;

		case VAR_TYPE_STRING:
			tempVar.varPtrString = (std::string *) variablePtr;
			break;

		case VAR_TYPE_BOOL:
			tempVar.varPtrBool = (bool *) variablePtr;
			break;

		default:
			// Unknown type
			break;
	}
	consoleVariables.push_back(tempVar);

	cout << "Var : " << variableName << " has been added." << endl;
}

void paraConsole::addCommand(const std::string &commandName, const std::string &functionName, const std::string &functionHelp)
{
	_consoleFunction tempFunction;

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


void paraConsole::setVarFunc(const std::string &varName, int variablePtr)
{
	for (auto &varItr : consoleVariables)
	{
		if (varName == varItr.varName)
		{
			*varItr.varPtrInt = variablePtr;
			return;
		}
		cout << "Var : " << varName << " not found." << endl;
	}
}

void paraConsole::setVarFunc(const std::string &varName, float variablePtr)
{
	for (auto &varItr : consoleVariables)
	{
		if (varName == varItr.varName)
		{
			*varItr.varPtrFloat = variablePtr;
			return;
		}
	}
	cout << "Var : " << varName << " not found." << endl;
}

void paraConsole::setVarFunc(const std::string &varName, std::string variablePtr)
{
	for (auto &varItr : consoleVariables)
	{
		if (varName == varItr.varName)
		{
			*varItr.varPtrString = variablePtr;
			return;
		}
	}
	cout << "Var : " << varName << " not found." << endl;
}

void paraConsole::setVarBool(const std::string &varName, bool variablePtr)
{
	for (auto &varItr : consoleVariables)
	{
		if (varName == varItr.varName)
		{
			*varItr.varPtrBool = variablePtr;
			return;
		}
	}
	cout << "Var : " << varName << " not found." << endl;
}

void paraConsole::setVar(const std::string &varName, const std::string &varParam)
{
	int varType;

	for (auto &varItr : consoleVariables)
	{
		if (varItr.varName == varName)
		{
			varType = varItr.varType;

			switch (varType)
			{
				case VAR_TYPE_BOOL:
					if ((varParam == "true") || (varParam == "1"))
					{
						setVarBool(varName, 1);
						return;
					}
					else
					{
						setVarBool(varName, 0);
						return;
					}
					break;

				case VAR_TYPE_INT:
					cout << "TYPE_INT" << endl;
					setVarFunc(varName, std::stoi(varParam));
					break;

				case VAR_TYPE_FLOAT:
					cout << "TYPE_FLOAT" << endl;
					setVarFunc(varName, std::stof(varParam));
					break;

				case VAR_TYPE_STRING:
					cout << "TYPE_STRING" << endl;
					setVarFunc(varName, varParam);
					break;
			}
		}
	}
	paraConsole::add(sys_getString("[ %s ] not found.", varName.c_str()));
}

void paraConsole::processVariable(std::vector<std::string> commandLine)
{
	if (commandLine[0] == "listVars")
	{
		for (const auto &varItr : consoleVariables)
		{
			paraConsole::add(sys_getString("[ %s ]", varItr.varName.c_str()));
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
		for (int i = 1; i < (int) commandLine.size(); i++)
		{
			for (const auto &varItr : consoleVariables)
			{
				if (varItr.varName == commandLine[i])
				{
					switch (varItr.varType)
					{
						case VAR_TYPE_INT:
							paraConsole::add(
									sys_getString("[ %s ] = [ %i ]", commandLine[i].c_str(), *(int *) varItr.varPtrInt));
							break;

						case VAR_TYPE_FLOAT:
							paraConsole::add(
									sys_getString("[ %s ] = [ %f ]", commandLine[i].c_str(), *(float *) varItr.varPtrFloat));
							break;

						case VAR_TYPE_BOOL:
							paraConsole::add(
									sys_getString("[ %s ] = [ %i ]", commandLine[i].c_str(), *(bool *) varItr.varPtrBool));
							break;

						case VAR_TYPE_STRING:
							paraConsole::add(
									sys_getString("[ %s ] = [ %s ]", commandLine[i].c_str(),
									              (std::string *) varItr.varPtrString->c_str()));
							break;
					}
				}
				else
				{
					paraConsole::add(sys_getString("[ %s ] not found.", commandLine[i].c_str()));
					return;
				}
			}
		}
	}
//
// Process changing the value of a variable
//
	if (commandLine[0] == "setVar")
	{
		std::string fullString;

		if (commandLine.size() < 3)
		{
			paraConsole::add(sys_getString("[ %s ] [ %s ] requires a valid argument.", commandLine[0].c_str(), commandLine[1].c_str()));
			return;
		}

		for (const auto& varItr : consoleVariables)
		{
			//
			// If the target variable is a string, then concatenate all the params back into a single string
			//
			if ((varItr.varType == VAR_TYPE_STRING) && (varItr.varName == commandLine[1]))
			{
				for (int i = 2; i != (int)commandLine.size(); i++)
				{
					fullString += commandLine[i] + " ";
				}
				//
				// Change the value to the full string
				setVar(commandLine[1], fullString);
				return;
			}
		}

		setVar(commandLine[1], commandLine[2]);
		return;
	}
}

void paraConsole::processCommand(std::vector<std::string> commandLine)
{
	std::map<std::string, _consoleFunction>::iterator localConsoleItr;
	std::string                                       parameterList;

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
		for (int i = 1; i != (int) commandLine.size(); i++)
		{
			parameterList += commandLine[i];
			parameterList += " ";
		}
	}
	paraConsole::add(sys_getString("Run function [ %s ] with [ %s ]", localConsoleItr->second.functionName.c_str(), parameterList.c_str()));
}