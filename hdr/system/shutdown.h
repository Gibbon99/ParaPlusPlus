#pragma once

#include "main.h"
#include "io/configFile.h"
#include "io/joystick.h"

// Shutdown with an error
void sys_shutdownWithError(std::string errorMessage);

// Shutdown with an error
void sys_shutdown();