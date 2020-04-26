#ifndef PARA_PARACONSOLE_H
#define PARA_PARACONSOLE_H

#include <string>
#include <vector>
#include <map>
#include "../wrapper.h"

#define CONSOLE_MEM_SIZE    1000    // Max number of lines in vector

typedef void (*funcPtr)();      // Pointer to function with no args and returns void

//
// This runs a script command
struct _consoleCommand
{
	std::string commandName;
	std::string commandHelp;
	funcPtr     commandPtr;
};

//
// This runs a internal function via function pointer
struct _consoleFunction
{
	void        *functionPtr;
	std::string functionHelp;
};

struct _variables
{
	std::string varName;
	int         varType;
	int         *varPtrInt    = new int;
	bool        *varPtrBool   = new bool;
	float       *varPtrFloat  = new float;
	std::string *varPtrString = new std::string;
};

struct _consoleLine
{
	float       posX;
	int         red;
	int         green;
	int         blue;
	int         alpha;
	std::string lineText;
};

class paraConsole
{
public:
//
// Constructor
	paraConsole(float defaultPosX, int red, int green, int blue, int alpha);

//
// Add things

	void addTempLine(const _consoleLine &newTempLine);

	void add(const std::string &newLine);

	void add(float linePosX, const std::string &newLine);

	void add(float linePosX, int red, int green, int blue, int alpha, const std::string &newLine);

	void addChar(const std::string &newChar);

	void addCommand(const std::string &commandName, const std::string &functionName, const std::string &functionHelp);

	void addCommand(const std::string &commandName, const std::string &commandHelp, funcPtr commandPtr);

	void addVariable(const std::string &variableName, int variableType, void *variablePtr);

	void addCharLine();

//
// Set things
	void setScreenSize(int consoleWinWidth, int consoleWinHeight);

	void setNumVarColumns(int newNumVarColumns);

	void setVarFunc(const std::string &varName, int variablePtr);

	void setVarFunc(const std::string &varName, float variablePtr);

	void setVarFunc(const std::string &varName, std::string variablePtr);

	void setVarBool(const std::string &varName, bool variablePtr);

	void setVar(const std::string &varName, const std::string &varParam);

//
// Get things
	void getVariable(const std::vector<std::string> &commandLine);

	float getDefaultPosX() const;

	int getDefaultRed() const;

	int getDefaultGreen() const;

	int getDefaultBlue() const;

	int getDefaultAlpha() const;

//
// Process things
	void prepare(float newPosX, float newPosY);

	void processCommand(std::vector<std::string> commandLine);

	void processVariable(std::vector<std::string> commandLine);

	void listVariables();

	void changeScrollBackOffset(int value);

//
// Misc things
	void deleteChar();

	void userBufferNext();

	void userBufferPrevious();

	bool stringStartsWith(const std::string &lookIn, const std::string &lookFor);

	void tabCompletion();

	void displayInColumns(const std::vector<std::string> &displayText);

	std::vector<std::string> tokeniseEntryLine(std::string entryLine);

	std::string entryLine();

	// TODO: bool entryMode - false means don't allow text entry

	std::vector<_consoleLine>                   consoleText;
	std::vector<std::string>                    userBuffer;        // Remember commands entered
	float                                       posX             = 0;
	float                                       posY             = 0;
	int                                         userBufferIndex  = 0;
	std::vector<_consoleLine>::reverse_iterator consoleItr;
	std::map<std::string, _consoleCommand>      consoleCommands;
	std::vector<_variables>                     consoleVariables;
	int                                         scrollbackOffset = 0;
	bool                                        isDrawing        = false;

private:
	std::string enterLine;
	int         defaultRed;
	int         defaultGreen;
	int         defaultBlue;
	int         defaultAlpha;
	float       defaultPosX;
	int         screenWidth;
	int         screenHeight;
	int         numVarColumns;
};

#endif //PARA_PARACONSOLE_H
