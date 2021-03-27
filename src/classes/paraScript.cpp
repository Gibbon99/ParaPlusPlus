#include "../../hdr/classes/paraScript.h"
#include <utility>
#include <system/scriptConfig.h>
#include <io/fileSystem.h>
#include <system/util.h>
#include "../main.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Restart the script engine - stop and free memory - clear all arrays
void paraScript::restart()
//----------------------------------------------------------------------------------------------------------------------
{
	scriptEngineRunning = false;

	stop();
	scriptFunctions.clear();
	hostVariables.clear();
	scriptFileCache.clear();
	scriptFunctionName.clear();
	hostScriptFunctions.clear();

	scriptEngine = asCreateScriptEngine (ANGELSCRIPT_VERSION);

	if (nullptr == scriptEngine)
	{
		scriptEngineRunning = false;
		funcOutput ( -1, sys_getString ("Script: Error: Failed to create script engine- [ %s ]", paraScript::getScriptError (0).c_str ()));
		return;
	}

	// The script compiler will write any compiler messages to the callback.
	scriptEngine->SetMessageCallback (asFUNCTION(funcOutput), nullptr, asCALL_CDECL);

	funcOutput ( -1, sys_getString ("Script: Script Engine restarted."));

	RegisterStdString (scriptEngine);

	sys_scriptInitScriptFunctions ();
	sys_scriptInitFunctions ();
	sys_scriptInitVariables ();
	io_getScriptFileNames ("scripts");
	paraScriptInstance.loadAndCompile ();
	paraScriptInstance.cacheFunctions ();

	scriptEngineRunning = true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Start the script engine
bool paraScript::init (scriptFunctionPtrStr outputFunction, asFUNCTION_t scriptOutputFunction)
//----------------------------------------------------------------------------------------------------------------------
{
	scriptEngineRunning = false;

	paraScript::funcOutput = outputFunction;

	scriptEngine = asCreateScriptEngine (ANGELSCRIPT_VERSION);

	if (nullptr == scriptEngine)
	{
		scriptEngineRunning = false;
		funcOutput ( -1, sys_getString ("Script: Error: Failed to create script engine- [ %s ]", paraScript::getScriptError (0).c_str ()));
	}

	// The script compiler will write any compiler messages to the callback.
	scriptEngine->SetMessageCallback (asFUNCTION(scriptOutputFunction), nullptr, asCALL_CDECL);

	funcOutput ( -1, sys_getString ("Script: Script Engine started."));

	RegisterStdString (scriptEngine);

//	RegisterScriptArray(scriptEngine, true);

	// What version are we running
	funcOutput ( -1, sys_getString ("Script: Script Engine version - [ %s ]", asGetLibraryVersion ()));

	// What options are compiled
	funcOutput ( -1, sys_getString ("Script: Options - [ %s ]", asGetLibraryOptions ()));

	scriptEngineRunning = true;

	return scriptEngineRunning;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Execute a script function from the host
void paraScript::run (const std::string &functionName, const std::string &param)
//----------------------------------------------------------------------------------------------------------------------
{
	int returnCode = 0;
	int testInt    = 0;

	if (!scriptEngineRunning)
		return;

	if (nullptr == context)
	{
		return;
	}

	for (const auto &funcItr : scriptFunctions)
	{
//		std::cout << sys_getString("Looking for function [ %s ] to [ %s ] to execute", functionName.c_str(), funcItr.scriptName.c_str()) << std::endl;

		if (scriptEngine != context->GetEngine ())
		{
			funcOutput ( -1, sys_getString ("Context DOES NOT belong to this engine."));
		}

		if (funcItr.scriptName == functionName)
		{
			if (scriptEngine != funcItr.funcID->GetEngine ())
			{
				funcOutput ( -1, sys_getString ("Function DOES NOT belong to this engine."));
			}

			context->Prepare (funcItr.funcID);
			//
			// Is there a parameter being passed for this function
			if (!param.empty ()) // TODO - Tokenize the parameter list and loop for all parameters
			{
				//
				// See if it's a number or not
				if (isdigit (param.c_str ()[0]))
				{
					//
					// Parameter is a number - convert before passing it in
					testInt = std::stoi (param, nullptr, 10);
					context->SetArgAddress (0, &testInt);
				}
				else
				{
					//
					// Send in a string
//					context->SetArgAddress (0, (void *) &param);
					context->SetArgAddress (0, static_cast<void *>(new std::string(param)));
				}
			}
#ifdef MY_DEBUG
//			std::cout << "Context state : " << getContextState (context->GetState ()) << std::endl;
			std::cout << "Execute : " << functionName << std::endl;
#endif
			if (asEXECUTION_PREPARED != context->GetState())
			{
				funcOutput (-1, sys_getString("The context is not ready to execute the function [ %s ].", functionName.c_str()));
				return;
			}

//			while (asEXECUTION_ACTIVE == context->GetState());

			returnCode = context->Execute ();
			if (returnCode != asEXECUTION_FINISHED)
			{
				//
				// The execution didn't finish as we had planned. Determine why.
				//
				if (returnCode == asEXECUTION_ABORTED)
				{
					funcOutput ( -1, sys_getString ("Script: The script was aborted before it could finish. Probably it timed out."));
				}
				else if (returnCode == asEXECUTION_EXCEPTION)
				{
					funcOutput ( -1, sys_getString ("Script: The script ended with an exception."));
					//
					// Write some information about the script exception
					//
					asIScriptFunction *func = context->GetExceptionFunction ();
					funcOutput ( -1, sys_getString ("Func: [ %s ]", func->GetDeclaration ()));
					funcOutput ( -1, sys_getString ("Module: [ %s ]", func->GetModuleName ()));
					funcOutput ( -1, sys_getString ("Section: [ %s ]", func->GetScriptSectionName ()));
					funcOutput ( -1, sys_getString ("Line: [ %i ]", context->GetExceptionLineNumber ()));
					funcOutput ( -1, sys_getString ("Desc: [ %s ]", context->GetExceptionString ()));
				}
				else
				{
					funcOutput ( -1, sys_getString ("The script ended for an unknown reason [ %i ] - [ %s ].", returnCode, getScriptError (returnCode).c_str ()));
				}
			}
			return;
		}
	}
	funcOutput ( -1, sys_getString ("Function [ %s ] not found in script", functionName.c_str ()));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Cache the functionID from functions in the scripts
void paraScript::cacheFunctions ()
//----------------------------------------------------------------------------------------------------------------------
{
	_scriptFunctionName tempFunctionName;
	asIScriptModule     *mod;

	mod = scriptEngine->GetModule (MODULE_NAME);

	//
	// Get function ID's for each function we will call in the script
	//
	for (const auto &funcItr : scriptFunctionName)
	{
		tempFunctionName.funcID = mod->GetFunctionByDecl (funcItr.functionName.c_str ());

		if (tempFunctionName.funcID == nullptr)
		{
			funcOutput ( -1, sys_getString ("Failed to get function ID for [ %s ]", funcItr.functionName.c_str ()));
		}
		else
		{
			funcOutput ( -1, sys_getString ("Func ID for [ %s ] - [ %i ]", funcItr.functionName.c_str (), tempFunctionName.funcID));

			//
			// Store the funcID and name to execute
			//
			tempFunctionName.param1       = funcItr.param1;
			tempFunctionName.functionName = funcItr.functionName;
			tempFunctionName.scriptName   = funcItr.scriptName;
			tempFunctionName.fromScript   = funcItr.fromScript;

			scriptFunctions.push_back (tempFunctionName);
		}
	}
	//
	// Do some preparation before execution
	context = scriptEngine->CreateContext ();

	scriptEngineRunning = true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Check if a filename is a valid script name
bool paraScript::isScriptName(std::string scriptName)
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto fileItr : scriptFileCache)
	{
		if (scriptName == fileItr.sectionName)
			return true;
	}
	return false;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Add a new script to the array to be compiled
void paraScript::addScript (std::string sectionName, std::string scriptContents)
//----------------------------------------------------------------------------------------------------------------------
{
	_scriptFileCache scriptFile;

	scriptFile.sectionName    = std::move (sectionName);
	scriptFile.scriptContents = std::move (scriptContents);

	scriptFileCache.push_back (scriptFile);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Compile the script files into a single module
bool paraScript::loadAndCompile ()
//----------------------------------------------------------------------------------------------------------------------
{
	int retCode = 0;

	retCode = builder.StartNewModule (scriptEngine, MODULE_NAME);

	if (retCode < 0)
	{
		funcOutput ( -1, sys_getString ("Failed to start script module."));
	}

	for (auto &scriptItr : scriptFileCache)
	{
//		printf("Inside add section loop\n");

//		scriptItr.scriptContents = fileSystem.getString(scriptItr.sectionName);
		retCode = builder.AddSectionFromMemory (scriptItr.sectionName.c_str (), scriptItr.scriptContents.c_str (), scriptItr.scriptContents.size (), 0);
		switch (retCode)
		{
			case 0:
				funcOutput ( -1, sys_getString ("Section with same name has already been added [ %s ]. Ignoring.", scriptItr.sectionName.c_str ()));
				break;

			case 1:
				funcOutput ( -1, sys_getString ("Section has been added [ %s ]", scriptItr.sectionName.c_str ()));
				break;

			default:
				funcOutput ( -1, sys_getString ("Failed to add script section [ %s ].", scriptItr.sectionName.c_str ()));
				break;
		}

		sys_freeMemory(scriptItr.sectionName);

	}

	SDL_Delay(200);

	printf("About to run BuildModule\n");

	//
	// Build the script from the loaded sections using ScriptBuilder
	//
	retCode = builder.BuildModule ();
	if (retCode < 0)
	{
		//sys_shutdownWithError ("Failed to build the script module.");
		return false;
	}

	printf("Finished buildModule\n");

//	funcOutput ( -1, sys_getString ("Compiled scripts."));

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Register all the functions to make available to the script
void paraScript::addHostFunction (const std::string &funcName, functionPtr funcPtr)
//----------------------------------------------------------------------------------------------------------------------
{
	int            returnCode;
	static asDWORD callType    = -1;
	static bool    callTypeSet = false;

	_hostScriptFunctions tempFunc;

	for (const auto &scriptItr : hostScriptFunctions)
	{
		if (scriptItr.scriptFunctionName == funcName)
		{
			funcOutput ( -1, sys_getString ("Function [ %s ] has already been added.", funcName.c_str ()));
			return;
		}
	}

	if (!callTypeSet)
	{
		if (!strstr (asGetLibraryOptions (), "AS_MAX_PORTABILTY"))
		{
			callType = asCALL_CDECL;
		}
		else
		{
			callType = asCALL_GENERIC;
		}
		callTypeSet = true;
	}

	/*
	if (callType < 0)
	{
		if (!strstr(asGetLibraryOptions(), "AS_MAX_PORTABILTY"))
		{
//			callType = asCALL_CDECL;
			callType = asCALL_STDCALL;
		}
		else
		{
			callType = asCALL_GENERIC;
			//callType = asCALL_STDCALL;
		}
	}
*/
	tempFunc.scriptFunctionName = funcName;
	tempFunc.hostFunctionPtr    = reinterpret_cast<int *>(funcPtr);

	returnCode = scriptEngine->RegisterGlobalFunction (funcName.c_str (), (asSFuncPtr &&) funcPtr, callType); //asCALL_CDECL);
	if (returnCode < 0)
	{
		funcOutput ( -1, sys_getString ("Failed to registerGlobalFunction [ %s ] - [ %s ]", funcName.c_str (), paraScript::getScriptError (returnCode).c_str ()));
	}

	funcOutput ( -1, sys_getString ("Script: Registered function - [ %s ]", funcName.c_str ()));
	hostScriptFunctions.push_back (tempFunc);
}


//----------------------------------------------------------------------------------------------------------------------
//
// Print out the variables that Angelscript knows about internally - DEBUGGING
void paraScript::debugState ()
//----------------------------------------------------------------------------------------------------------------------
{
	asUINT stackLevel = 0;

	if (!scriptEngineRunning)
		return;

	asIScriptContext *ctx    = asGetActiveContext ();
	asIScriptEngine  *engine = ctx->GetEngine ();

	int  typeId1      = ctx->GetThisTypeId ();
	void *varPointer1 = ctx->GetThisPointer ();
	if (typeId1)
	{
		funcOutput ( -1, sys_getString (" this = 0x%x\n", varPointer1));
	}

	asUINT      numVars = ctx->GetVarCount ();
	for (asUINT n       = 0; n < numVars; n++)
	{
		int  typeId      = ctx->GetVarTypeId (n);
		void *varPointer = ctx->GetAddressOfVar (n);
		if (typeId == engine->GetTypeIdByDecl ("int"))
		{
			funcOutput ( -1, sys_getString (" %s = %d\n", ctx->GetVarDeclaration (n, stackLevel), *(int *) varPointer));
		}
		else if (typeId == engine->GetTypeIdByDecl ("string"))
		{
			auto *str = (std::string *) varPointer;
			if (str)
			{
				funcOutput ( -1, sys_getString (" %s = '%s'\n", ctx->GetVarDeclaration (n, stackLevel), str->c_str ()));
			}
			else
			{
				funcOutput ( -1, sys_getString (" %s = <null>\n", ctx->GetVarDeclaration (n, stackLevel)));
			}
		}
		else
		{
//			auto *getValue = (b2Vec2 *) varPointer;
//			log_addEvent(sys_getString(" %s = {...}\n", ctx->GetVarDeclaration(n, stackLevel)));
//			log_addEvent(sys_getString("Debug [ %3.3f %3.3f ]\n", getValue->x, getValue->y));
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Shutdown the script engine and remove the instance, cleaning up memory
void paraScript::stop ()
//----------------------------------------------------------------------------------------------------------------------
{
	if (context != nullptr)
	{
		context->Suspend ();
		context->Unprepare ();
		context->Release ();
		context = nullptr;
	}

	if (scriptEngine != nullptr)
	{
		scriptEngine->ShutDownAndRelease ();
		scriptEngine = nullptr;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Add a new function that is defined in a script - create name to call it from host
void paraScript::addScriptFunction (const std::string &funcName, std::string hostCallName)
//----------------------------------------------------------------------------------------------------------------------
{
	_scriptFunctionName tempFunctionName;

	for (const auto &funcItr : scriptFunctionName)
	{
		if (funcItr.functionName == funcName)
		{
			funcOutput ( -1, sys_getString ("Function name [ %s ] has already been added.", funcName.c_str ()));
			return;
		}
	}

	tempFunctionName.fromScript   = true;
	tempFunctionName.funcID       = nullptr;
	tempFunctionName.functionName = funcName;
	tempFunctionName.param1       = false;
	tempFunctionName.scriptName   = std::move (hostCallName);

	scriptFunctionName.push_back (tempFunctionName);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Add a variable to the array holding all the script accessible variables
void paraScript::addHostVariable (const std::string &varName, void *varPtr)
//----------------------------------------------------------------------------------------------------------------------
{
	_hostScriptVariables tempVar;

	tempVar.scriptFunctionName = varName;
	tempVar.hostFunctionPtr    = varPtr;

	for (const auto &varItr : hostVariables)
	{
		if (varItr.scriptFunctionName == varName)
		{
			funcOutput ( -1, sys_getString ("Variable [ %s ] has already been added.", varName.c_str ()));
			return;
		}
	}

	if (scriptEngine->RegisterGlobalProperty (varName.c_str (), (void *) varPtr) < 0)
	{
		funcOutput ( -1, sys_getString ("Script: Error: Couldn't register variable - [ %s ]", varName.c_str ()));
	}

	funcOutput ( -1, sys_getString ("Script: Registered variable - [ %s ]", varName.c_str ()));
	hostVariables.push_back (tempVar);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the context state - return a string
std::string paraScript::getContextState (int whichState)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (whichState)
	{
		case asEXECUTION_FINISHED:
			return "The context has successfully completed the execution.";
			break;

		case asEXECUTION_SUSPENDED:
			return "The execution is suspended and can be resumed.";
			break;

		case asEXECUTION_ABORTED:
			return "The execution was aborted by the application.";
			break;

		case asEXECUTION_EXCEPTION:
			return "The execution was terminated by an unhandled script exception.";
			break;

		case asEXECUTION_PREPARED:
			return "The context has been prepared for a new execution.";
			break;

		case asEXECUTION_UNINITIALIZED:
			return "The context is not initialized.";
			break;

		case asEXECUTION_ACTIVE:
			return "The context is currently executing a function call.";
			break;

		case asEXECUTION_ERROR:
			return "The context has encountered an error and must be reinitialized.";
			break;
	}
	return "Unknown context from switch.";
}

//----------------------------------------------------------------------------------------------------------------------
//
// Error codes for AngelScript
std::string paraScript::getScriptError (int errNo)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (errNo)
	{
		case 0:
			return ("asSUCCESS");
			break;

		case -1:
			return ("asERROR");
			break;

		case -2:
			return ("asCONTEXT_ACTIVE");
			break;

		case -3:
			return ("asCONTEXT_NOT_FINISHED");
			break;

		case -4:
			return ("asCONTEXT_NOT_PREPARED");
			break;

		case -5:
			return ("asINVALID_ARG");
			break;

		case -6:
			return ("asNO_FUNCTION");
			break;

		case -7:
			return ("asNOT_SUPPORTED");
			break;

		case -8:
			return ("asINVALID_NAME");
			break;

		case -9:
			return ("asNAME_TAKEN");
			break;

		case -10:
			return ("asINVALID_DECLARATION");
			break;

		case -11:
			return ("asINVALID_OBJECT");
			break;

		case -12:
			return ("asINVALID_TYPE");
			break;

		case -13:
			return ("asALREADY_REGISTERED");
			break;

		case -14:
			return ("asMULTIPLE_FUNCTIONS");
			break;

		case -15:
			return ("asNO_MODULE");
			break;

		case -16:
			return ("asNO_GLOBAL_VAR");
			break;

		case -17:
			return ("asINVALID_CONFIGURATION");
			break;

		case -18:
			return ("asINVALID_INTERFACE");
			break;

		case -19:
			return ("asCANT_BIND_ALL_FUNCTIONS");
			break;

		case -20:
			return ("asLOWER_ARRAY_DIMENSION_NOT_REGISTERED");
			break;

		case -21:
			return ("asAPP_CANT_INTERFACE_DEFAULT_ARRAY");
			break;

		default:
			return ("Unknown error type.");
	}
}
