#ifndef PARA_PARATEXTURE_H
#define PARA_PARATEXTURE_H

#include <string>
#include <wrapper.h>
#include <map>

typedef void      (*textureFunctionPtrStr) (int, std::string);

typedef SDL_RWops *(*textureFunctionPtrLoad) (std::string);


struct __texture
{
	bool              loaded   = false;
	int               width;
	int               height;
	std::string       fileName = "";
	std::string       keyName  = "";
	std::vector<char> collisionMap;
	PARA_Texture *texture      = nullptr;
	PARA_Surface *surface      = nullptr;
};

class paraTexture
{
public:

	void ReleaseRef ();

	void AddRef ();

	int init (textureFunctionPtrStr outputFunction, textureFunctionPtrLoad loadFunction);

	std::string int_getString (std::string format, ...);

	bool load (std::string fileName);

	void render (std::string keyName);

	void render (std::string keyName, SDL_Rect *destination);

	int width(const std::string &keyName);

	int height(const std::string &keyName);

	bool createMap(std::string textureName);

	void loadMap(std::string textureName);

	char pixelColor (std::string textureName, int posX, int posY);

private:
	std::map<std::string, __texture> texture;
	textureFunctionPtrStr            funcOutput{};
	textureFunctionPtrLoad           funcLoad{};
};


#endif //PARA_PARATEXTURE_H
