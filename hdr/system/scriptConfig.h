#pragma once

#include "main.h"

// This is how we call a script from the Host program : Name in Script : Name to call from host
void sys_scriptInitScriptFunctions();

// Setup all the globals functions that the scripts can call for action in the host
void sys_scriptInitFunctions();

// Setup all the global variables to be shared between host and scripts
void sys_scriptInitVariables();