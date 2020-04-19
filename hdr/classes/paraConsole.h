#ifndef PARA_PARACONSOLE_H
#define PARA_PARACONSOLE_H

#include <string>
#include <vector>
#include <map>

#define CONSOLE_MEM_SIZE	1000	// Max number of lines in vector

enum consoleVariableTypes
{
	VAR_TYPE_INT = 0,
	VAR_TYPE_FLOAT,
	VAR_TYPE_BOOL,
	VAR_TYPE_STRING
};

enum consoleFunctionSource
{
	CONSOLE_FUNCTION_SCRIPT = 0,
	CONSOLE_FUNCTION_BUILTIN
};

struct _consoleFunction
{
	std::string		functionName;
	std::string		functionHelp;
};

struct _consoleVariable
{
	int				variableType;
	void			*variablePtr;
};

class paraConsole
{
public:
	void add(const std::string& newLine);

	void prepare(int newPosX, int newPosY);

	void addChar(const std::string& newChar);

	void deleteChar();

	void addCharLine();

	void userBufferNext();

	void userBufferPrevious();

	std::vector<std::string> tokeniseEntryLine(std::string entryLine);

	void processCommand(std::vector<std::string> commandLine);

	void processVariable(std::vector<std::string> commandLine);

	void addCommand(const std::string& commandName, const std::string& functionName, const std::string& functionHelp);

	void addVariable(const std::string& variableName, int variableType, void* variablePtr);

	std::string entryLine();	

	// TODO: bool entryMode - false means don't allow text entry

	std::vector<std::string> consoleText;
	std::vector<std::string> userBuffer;		// Remember commands entered
	int posX = 0;
	int posY = 0;
	int userBufferIndex = 0;
	std::vector<std::string>::reverse_iterator  consoleItr;
	std::map<std::string, _consoleFunction>		consoleFunctions;
	std::map<std::string, _consoleVariable>		consoleVariables;

private:
	std::string enterLine;
	
};


#endif //PARA_PARACONSOLE_H
