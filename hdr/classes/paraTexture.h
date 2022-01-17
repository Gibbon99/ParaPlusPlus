#pragma once

#include <string>
#include <map>
#include <memory>
#include "wrapper.h"

typedef void      (*textureFunctionPtrStr)(int, std::string);

typedef SDL_RWops *(*textureFunctionPtrLoad)(std::string);

class paraTexture
{
public:

	// Deconstructor for a texture - free memory
	~paraTexture();

	void ReleaseRef();

	void AddRef();

	paraTexture(textureFunctionPtrStr outputFunction, textureFunctionPtrLoad loadFunction);

	std::string int_getString(std::string format, ...);

	bool load(std::string newFileName, std::string newKeyName);

	void destroy();

	void render();

	void render(SDL_Rect *destination);

	int getWidth() const;

	int getHeight() const;

	char pixelColor(int posX, int posY);

	void setFileName(std::string newFilename);

	bool isLoaded();

	PARA_Texture *getTexture();
	PARA_Surface *getSurface();

private:
	bool              loaded {false};
	int               width {};
	int               height {};
	std::string       fileName {};
	std::string       keyName {};
	std::vector<char> collisionMap;
	PARA_Texture *texture = nullptr;
	PARA_Surface *surface = nullptr;

	textureFunctionPtrStr  funcOutput {};
	textureFunctionPtrLoad funcLoad {};
};