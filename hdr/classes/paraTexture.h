#ifndef PARA_PARATEXTURE_H
#define PARA_PARATEXTURE_H

#include <string>
#include <wrapper.h>
#include <map>

typedef void      (*textureFunctionPtrStr) (int, std::string);

typedef SDL_RWops *(*textureFunctionPtrLoad) (std::string);

class paraTexture
{
public:

	void ReleaseRef ();

	void AddRef ();

	paraTexture(textureFunctionPtrStr outputFunction, textureFunctionPtrLoad loadFunction);

	std::string int_getString (std::string format, ...);

	bool load (std::string newFileName, std::string newKeyName);

	void render ();

	void render (SDL_Rect *destination);

	int getWidth();

	int getHeight();

	bool createMap();

	void loadMap();

	char pixelColor (int posX, int posY);

	void setFileName(std::string newFilename);

	PARA_Texture    *getTexture();

private:
	bool              loaded   = false;
	int               width;
	int               height;
	std::string       fileName = "";
	std::string       keyName  = "";
	std::vector<char> collisionMap;
	PARA_Texture *texture      = nullptr;
	PARA_Surface *surface      = nullptr;

	textureFunctionPtrStr            funcOutput{};
	textureFunctionPtrLoad           funcLoad{};
};

#endif //PARA_PARATEXTURE_H
