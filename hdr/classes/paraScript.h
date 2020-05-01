#ifndef PARA_PARASCRIPT_H
#define PARA_PARASCRIPT_H

#define MODULE_NAME                 "clientModule"

#include "../script/angelscript.h"
#include "../script/scriptbuilder.h"
#include "../script/scriptstdstring.h"

typedef int (*functionPtr) (...);

typedef void      (*scriptFunctionPtrStr) (int, std::string);

class paraScript
{
public:
	bool init (scriptFunctionPtrStr outputFunction, asFUNCTION_t scriptOutputFunction);

	void addHostVariable (const std::string &varName, void *varPtr);

	void addScriptFunction (const std::string &funcName, std::string hostCallName);

	void addHostFunction (const std::string &funcName, functionPtr funcPtr);

	void stop ();

	void debugState ();

	bool isScriptName(std::string scriptName);

	void restart();

	bool loadAndCompile ();

	void addScript (std::string sectionName, std::string scriptContents);

	void cacheFunctions ();

	void run (const std::string &functionName, const std::string &param);

	asIScriptEngine *scriptEngine = nullptr;

private:
	static std::string getScriptError (int errNo);

	std::string getContextState (int whichState);
	scriptFunctionPtrStr              funcOutput{};

//----------------------------------------------------------------------------------------------------------------------
//
// AngelScript engine
//
//----------------------------------------------------------------------------------------------------------------------

	asIScriptContext *context            = nullptr;
	CScriptBuilder   builder;
	bool             scriptEngineRunning = false;

//----------------------------------------------------------------------------------------------------------------------
//
// Structure to hold all the functions in the script
//
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
//
// This is used to hold the function ID and is used when creating functions
// from within a running script. A copy of scriptFunctionName is used as
// the source data.
//
//----------------------------------------------------------------------------------------------------------------------

	typedef struct
	{
		asIScriptFunction *funcID;
		bool              fromScript; // Is this created from a console script
		std::string       functionName;
		std::string       scriptName;
		bool              param1; // Does this function need something passed to it
	}                _scriptFunctionName;

	std::vector<_scriptFunctionName>  scriptFunctions;

//----------------------------------------------------------------------------------------------------------------------
//
// This is used to hold the variables that are shared between the host and script program
//
//----------------------------------------------------------------------------------------------------------------------

	typedef struct
	{
		std::string scriptFunctionName;
		void        *hostFunctionPtr = nullptr;
	}                                 _hostScriptFunctions;

	typedef struct
	{
		std::string scriptFunctionName;
		void        *hostFunctionPtr = nullptr;
	}                                 _hostScriptVariables;
	std::vector<_hostScriptVariables> hostVariables;

//----------------------------------------------------------------------------------------------------------------------
//
// Struct to hold all the script files found in the scripts directory
//
//----------------------------------------------------------------------------------------------------------------------

	typedef struct
	{
		std::string sectionName;
		std::string scriptContents;
	}                                 _scriptFileCache;

	std::vector<_scriptFileCache> scriptFileCache;

//----------------------------------------------------------------------------------------------------------------------
//
// Struct to hold host function mapping to script function names
//
//----------------------------------------------------------------------------------------------------------------------

	std::vector<_scriptFunctionName>  scriptFunctionName;
	std::vector<_hostScriptFunctions> hostScriptFunctions;
};

#endif //PARA_PARASCRIPT_H
