#ifndef PARA_PARATEXTURE_H
#define PARA_PARATEXTURE_H

#include <string>
#include <wrapper.h>
#include <map>

typedef void      (*textureFunctionPtrStr) (int, std::string);

typedef SDL_RWops *(*textureFunctionPtrLoad) (std::string);


struct __texture
{
	bool         loaded           = false;
	std::string  fileName         = "";
	std::string  keyName          = "";
	PARA_Texture *texture           = nullptr;
};

class paraTexture
{
public:

	void ReleaseRef ();
	void AddRef();

	int init (textureFunctionPtrStr outputFunction, textureFunctionPtrLoad loadFunction);

	std::string int_getString (std::string format, ...);

	bool load (std::string fileName);

	void render(std::string keyName);

private:
	std::map<std::string, __texture>   texture;
	textureFunctionPtrStr              funcOutput{};
	textureFunctionPtrLoad             funcLoad{};
};


#endif //PARA_PARATEXTURE_H
