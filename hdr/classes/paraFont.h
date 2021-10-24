#pragma once

#include <string>
#include <map>
#include "../wrapper.h"
#include "SDL_ttf.h"

typedef void (*funcPtrIntStr)(int, std::string);

struct __PARA_FONT
{
	bool     available;
	int      lineHeight;
	int      descent;
	TTF_Font *handle;
};


class paraFont
{
public:

	paraFont();

	~paraFont();

	bool load(int fontSize, const std::string &keyName, const std::string &fileName);

	void close();

	void setColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);

	std::string int_getString(std::string format, ...);

	void setOutputFunction(funcPtrIntStr outputFunction);

	PARA_Surface *write(double X, double Y, const std::string &fontText);

	void use(const std::string &keyName);

	int width(const std::string &fontText);

	int height();

	int descent();

	void render(SDL_Renderer *whichRenderer, double posX, double posY, int r, int g, int b, int a, std::string text);

	SDL_FRect pos {};

private:
	bool                               fontSystemAvailable = false;
	TTF_Font                           *fontHandle         = nullptr;
	PARA_Surface *surface                                  = nullptr;
	PARA_Color   color                                     = {0, 0, 0, 0};
	funcPtrIntStr                      funcOutput {};
	std::string                        currentFont;
	std::map<std::string, __PARA_FONT> fonts;
};