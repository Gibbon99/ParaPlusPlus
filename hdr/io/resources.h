#pragma once

#include <map>
#include <SDL_mixer.h>
//#include "../main.h"
#include "../wrapper.h"

struct __resourceCommon
{
	bool        loaded = false;
	std::string fileName = "";
};

struct __bitmap
{
	PARA_Texture    *texture = nullptr;
	std::string      fileName = "";
	int              width;
	int              height;
	__resourceCommon common;
};

typedef struct {
	int *memLevel;
} __level;

typedef struct {
	PARA_Texture    *texture;
	int              numFrames;
	int              frameWidth;
	int              frameHeight;
	int              renderOffsetX;
	int              renderOffsetY;
	__resourceCommon common;
} __sprite;

extern std::map<std::string, __bitmap> bitmap;
extern std::map<std::string, __level>  levels;
//extern std::map<std::string, __highScore> highScore;
extern std::map<std::string, __sprite> sprites;
//extern std::map<std::string, __tiles> tiles;

// Called from script to load a resource
void sys_loadResource (const std::string &key, const std::string &fileName, int type, int numFrames, int size);
