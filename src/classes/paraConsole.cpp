#include <iostream>
#include <regex>

#include "../../hdr/classes/paraConsole.h"
#include "../../hdr/wrapper.h"
#include "../../hdr/main.h"
#include "../../hdr/system/gameEvents.h"

paraConsole::paraConsole(float defaultPosX, int red, int green, int blue, int alpha)
{
	paraConsole::defaultRed   = red;
	paraConsole::defaultGreen = green;
	paraConsole::defaultBlue  = blue;
	paraConsole::defaultAlpha = alpha;
	paraConsole::defaultPosX  = defaultPosX;
}

void paraConsole::setScreenSize(int consoleWinWidth, int consoleWinHeight)
{
	paraConsole::screenWidth  = consoleWinWidth;
	paraConsole::screenHeight = consoleWinHeight;
}

float paraConsole::getDefaultPosX() const
{
	return defaultPosX;
}

int paraConsole::getDefaultRed() const
{
	return defaultRed;
};

int paraConsole::getDefaultGreen() const
{
	return defaultGreen;
};

int paraConsole::getDefaultBlue() const
{
	return defaultBlue;
};

int paraConsole::getDefaultAlpha() const
{
	return defaultAlpha;
};

void paraConsole::addTempLine(const _consoleLine &newTempLine)
{
	while (isDrawing)  // Wait until any drawing operation is completed
	{
		cout << "Cant add while drawing in operation" << endl;  // TODO - Remove after testing
	}

	if (paraConsole::consoleText.size() < CONSOLE_MEM_SIZE)
		paraConsole::consoleText.push_back(newTempLine);
	else
		std::cout << "Error: Exceeded memory buffer for console text." << std::endl;
}

void paraConsole::add(float linePosX, int red, int green, int blue, int alpha, const std::string &newLine)
{
	_consoleLine tempLine;

	tempLine.lineText = newLine;
	tempLine.posX     = linePosX;
	tempLine.red      = red;
	tempLine.green    = green;
	tempLine.blue     = blue;
	tempLine.alpha    = alpha;

	addTempLine(tempLine);
}

void paraConsole::add(float linePosX, const std::string &newLine)
{
	_consoleLine tempLine;

	tempLine.lineText = newLine;
	tempLine.posX     = linePosX;
	tempLine.red      = paraConsole::defaultRed;
	tempLine.green    = paraConsole::defaultGreen;
	tempLine.blue     = paraConsole::defaultBlue;
	tempLine.alpha    = paraConsole::defaultAlpha;

	addTempLine(tempLine);
}

void paraConsole::add(const std::string &newLine)
{
	_consoleLine tempLine;

	tempLine.lineText = newLine;
	tempLine.posX     = paraConsole::defaultPosX;
	tempLine.red      = paraConsole::defaultRed;
	tempLine.green    = paraConsole::defaultGreen;
	tempLine.blue     = paraConsole::defaultBlue;
	tempLine.alpha    = paraConsole::defaultAlpha;

	addTempLine(tempLine);
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
	_consoleLine tempLine;

	if (paraConsole::enterLine.empty())
		return;

	tempLine.lineText = paraConsole::enterLine;
	tempLine.posX     = paraConsole::posX;
	tempLine.red      = paraConsole::defaultRed;
	tempLine.green    = paraConsole::defaultGreen;
	tempLine.blue     = paraConsole::defaultBlue;
	tempLine.alpha    = paraConsole::defaultAlpha;

	paraConsole::consoleText.push_back(tempLine);
	paraConsole::userBuffer.push_back(paraConsole::enterLine);

	paraConsole::processCommand(paraConsole::tokeniseEntryLine(paraConsole::enterLine));

	paraConsole::enterLine.clear();
	paraConsole::userBufferIndex = userBuffer.size();

	paraConsole::scrollbackOffset = 0;
}

std::string paraConsole::entryLine()
{
	return "> " + paraConsole::enterLine;
}

void paraConsole::prepare(float newPosX, float newPosY)
{
	paraConsole::consoleItr = paraConsole::consoleText.rbegin() + scrollbackOffset;
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

	if ((variableName.empty()) || (nullptr == variablePtr))
	{
		std::cout << "Adding a variable must include a valid name and pointer." << endl;
		return;
	}

	if ((variableType < 0) || (variableType > VAR_TYPE_LAST))
	{
		std::cout << "Invalid variableType for [ " << variableName << " ]" << endl;
		return;
	}

	for (const auto &varItr : consoleVariables)
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

void paraConsole::addCommand(const std::string &commandName, const std::string &commandHelp, funcPtr commandPtr)
{
	_consoleCommand tempCommand;

	tempCommand.commandName = commandName;
	tempCommand.commandHelp = commandHelp;
	tempCommand.commandPtr  = commandPtr;

	if ((!commandName.empty()) || (!commandHelp.empty()))
	{
		paraConsole::consoleCommands.insert(std::pair<std::string, _consoleCommand>(commandName, tempCommand));
		return;
	}
	console.add(console.defaultPosX, 255, 0, 0, 0, "Add console commands must include a valid commandName and functionName.");
}

void paraConsole::addCommand (const std::string &commandName, const std::string &functionHelp, const std::string &functionName)
{
	_consoleCommand tempCommand;

	tempCommand.commandName = functionName;
	tempCommand.commandHelp = functionHelp;
	tempCommand.commandPtr  = nullptr;

	if ((!commandName.empty () || !functionName.empty ()))
	{
		paraConsole::consoleCommands.insert (std::pair<std::string, _consoleCommand> (commandName, tempCommand));
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
					if ((varParam == "true") || (varParam == "1") || (varParam == "True") || (varParam == "on") || (varParam == "On"))
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
					setVarFunc(varName, std::stoi(varParam));
					break;

				case VAR_TYPE_FLOAT:
					setVarFunc(varName, std::stof(varParam));
					break;

				case VAR_TYPE_STRING:
					setVarFunc(varName, varParam);
					break;
			}
			return;
		}
	}
	paraConsole::add(1.0, 255, 0, 0, 255, sys_getString("setVar [ %s ] not found.", varName.c_str()));
}

void paraConsole::setNumVarColumns(int newNumVarColumns)
{
	numVarColumns = newNumVarColumns;
}

void paraConsole::displayInColumns(const std::vector<std::string> &displayText)
{
	float columnPosX     = 3;
	float columnVarWidth = (float) (screenWidth / numVarColumns);

	for (const auto &varItr : displayText)
	{
		paraConsole::add(columnPosX, sys_getString("[ %s ]", varItr.c_str()));
		columnPosX += columnVarWidth;
		if (columnPosX > (columnVarWidth * (numVarColumns)))
		{
			columnPosX = 3;
		}
	}
}

void paraConsole::listVariables()
{
	std::string allVariables;

	for (const auto &varItr : consoleVariables)
	{
		allVariables += varItr.varName + " ";
	}
	displayInColumns(tokeniseEntryLine(allVariables));
}

void paraConsole::getVariable(const std::vector<std::string> &commandLine)
{
	bool matchFound = false;

	if (commandLine.size() < 2)
	{
		paraConsole::add(sys_getString("[ %s ] requires an argument supplied.", commandLine[0].c_str()));
		return;
	}
	//
	// find any matching variable names
	for (int i = 1; i < (int) commandLine.size(); i++)
	{
		matchFound = false;
		for (const auto &varItr : consoleVariables)
		{
			if (varItr.varName == commandLine[i])
			{
				switch (varItr.varType)
				{
					case VAR_TYPE_INT:
						paraConsole::add(
								sys_getString("[ %s ] = [ %i ]", commandLine[i].c_str(), *(int *) varItr.varPtrInt));
						matchFound = true;
						break;

					case VAR_TYPE_FLOAT:
						paraConsole::add(
								sys_getString("[ %s ] = [ %f ]", commandLine[i].c_str(), *(float *) varItr.varPtrFloat));
						matchFound = true;
						break;

					case VAR_TYPE_BOOL:
						paraConsole::add(
								sys_getString("[ %s ] = [ %i ]", commandLine[i].c_str(), *(bool *) varItr.varPtrBool));
						matchFound = true;
						break;

					case VAR_TYPE_STRING:
						paraConsole::add(
								sys_getString("[ %s ] = [ %s ]", commandLine[i].c_str(),
								              (std::string *) varItr.varPtrString->c_str()));
						matchFound = true;
						break;
				}
			}
		}
		if (!matchFound)
			paraConsole::add(2, 255, 0, 0, 255, sys_getString("[ %s ] not found.", commandLine[i].c_str()));
	}
}

void paraConsole::processVariable(std::vector<std::string> commandLine)
{
	if (commandLine[0] == "listVars")
	{
		listVariables();
		return;
	}

	if (commandLine[0] == "getVar")
	{
		getVariable(commandLine);
		return;
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

		for (const auto &varItr : consoleVariables)
		{
			//
			// If the target variable is a string, then concatenate all the params back into a single string
			//
			if ((varItr.varType == VAR_TYPE_STRING) && (varItr.varName == commandLine[1]))
			{
				for (int i = 2; i != (int) commandLine.size(); i++)
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
	std::map<std::string, _consoleCommand>::iterator localConsoleItr;
	std::string                                      parameterList;

	//
	// Handle variables on their own
	if ((commandLine[0] == "getVar") ||
	    (commandLine[0] == "setVar") ||
	    (commandLine[0] == "listVars"))
	{
		paraConsole::processVariable(commandLine);
		return;
	}
	//
	// See if the command exists
	localConsoleItr = consoleCommands.find(commandLine[0]);
	if (localConsoleItr == consoleCommands.end())
	{
		paraConsole::add(sys_getString("Command [ %s ] not found.", commandLine[0].c_str()));
		return;
	}
	//
	// Are there parameters on the command line
	if (commandLine.size() > 1)
	{
		for (int i = 1; i != (int) commandLine.size(); i++)
		{
			parameterList += commandLine[i];
			parameterList += " ";
		}
	}
	//
	//
	if (localConsoleItr->second.commandPtr != nullptr)      // Is this an internal function
		localConsoleItr->second.commandPtr ();
	else
	{
		paraScriptInstance.run (localConsoleItr->second.commandName, parameterList);
	}
}

bool paraConsole::stringStartsWith(const std::string &lookIn, const std::string &lookFor)
{
	if (lookIn.find(lookFor) == 0)
		return true;
	else
		return false;
}

void paraConsole::changeScrollBackOffset(int value)
{
	paraConsole::scrollbackOffset += value;
	if (paraConsole::scrollbackOffset < 0)
		paraConsole::scrollbackOffset = 0;

	if (paraConsole::scrollbackOffset > (int)paraConsole::consoleText.size() - 1)
		paraConsole::scrollbackOffset = (int)paraConsole::consoleText.size() - 1;
}

void paraConsole::tabCompletion()
{
	int                      numMatchingVariables = 0;
	int                      numMatchingGetSet    = 0;
	int                      numMatchingCommands  = 0;
	std::string              pushVariableString;
	std::string              pushCommandString;
	std::string              getSetVariableString;
	std::vector<std::string> completeVariable;

	for (const auto &varItr : consoleVariables)
	{
		if (stringStartsWith(varItr.varName, enterLine))
		{
			pushVariableString += varItr.varName + "";
			numMatchingVariables++;
		}
	}

	for (const auto &commandItr : consoleCommands)
	{
		if (stringStartsWith(commandItr.first, enterLine))
		{
			pushCommandString += commandItr.first + " ";
			numMatchingCommands++;
		}
	}
	//
	// Auto complete a variable if using a setVar or getVar
	//
	if (stringStartsWith(enterLine, "getVar") ||
	    stringStartsWith(enterLine, "setVar"))
	{
		numMatchingGetSet = 0;
		completeVariable  = tokeniseEntryLine(enterLine);
		for (const auto &varItr : consoleVariables)
		{
			if (stringStartsWith(varItr.varName, completeVariable[1]))
			{
				numMatchingGetSet++;
				getSetVariableString += varItr.varName + " " ;
			}
		}
		if (numMatchingGetSet > 1)
		{
			displayInColumns(tokeniseEntryLine(getSetVariableString));
			return;
		}
		else
		{
			enterLine = completeVariable[0] + " " + getSetVariableString;
			return;
		}
	}

	if (numMatchingCommands > 1)
	{
		if (!pushCommandString.empty())
		{
			paraConsole::add(" ");
			paraConsole::add("Available commands.");
			pushCommandString += "listVars getVar setVar";
			displayInColumns(tokeniseEntryLine(pushCommandString));
		}
	}
	else
	{
		enterLine = pushCommandString;
	}
}
