#pragma once

#include <map>
#include <SDL_mixer.h>
#include "../wrapper.h"
#include <string>

struct __level
{
	int *memLevel;
};

extern std::map<std::string, __level>  levels;