#ifndef PARA_PARAFONT_H
#define PARA_PARAFONT_H

#include <string>
#include "../wrapper.h"
#include "SDL_ttf.h"

typedef void (*functionPtrStr)(std::string);

class paraFont
{
public:
	bool load(int fontSize, const std::string &fileName);

	void close();

	void setColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

	std::string int_getString(std::string format, ...);

	void setOutputFunction(functionPtrStr outputFunction);

	PARA_Surface *write(float X, float Y, const std::string &fontText);

	float    lineHeight = 0;
	SDL_Rect pos;

private:
	bool           fontSystemAvailable = false;
	bool           available           = false;
	TTF_Font       *fontHandle         = nullptr;
	PARA_Surface *surface              = nullptr;
	PARA_Color   color                 = {0, 0, 0, 0};
	functionPtrStr funcOutput;
};

#endif //PARA_PARAFONT_H
