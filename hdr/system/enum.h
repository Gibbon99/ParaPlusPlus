#pragma once

enum variableTypes
{
	VAR_TYPE_INT = 0,
	VAR_TYPE_UINT,
	VAR_TYPE_FLOAT,
	VAR_TYPE_UFLOAT,
	VAR_TYPE_BOOL,
	VAR_TYPE_STRING
};

enum consoleFunctionSource
{
	CONSOLE_FUNCTION_SCRIPT = 0,
	CONSOLE_FUNCTION_BUILTIN
};

enum currentModes
{
	MODE_CONSOLE_EDIT = 0
};