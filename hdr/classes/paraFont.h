#ifndef PARA_PARAFONT_H
#define PARA_PARAFONT_H

#include <string>
#include "../wrapper.h"
#include "SDL_ttf.h"
#include "paraLogFile.h"

class paraFont
{
public:
	bool load(int fontSize, const std::string &fileName);

	void close();

	void setColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

	PARA_Surface *write(int X, int Y, const std::string &fontText);

//	void render(float posX, float posY, std::string renderText);

	int lineHeight;
	SDL_Rect pos;

private:
	bool                fontSystemAvailable = false;
	bool                available           = false;
	TTF_Font            *fontHandle;
	PARA_Surface        *surface;
	PARA_Color          color;
};

#endif //PARA_PARAFONT_H
